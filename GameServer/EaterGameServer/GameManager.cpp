#include "GameManager.h"
#include "DHNetWorkAPI.h"
#include "GameClientGameServerPacketDefine.h"
#include "GameProtocol/PlayerData_generated.h"
#include "DHTimer.h"

GameManager::GameManager()
{
	// 데이터 전송을 위한 패킷 생성(재활용 할 예정)
	_S2C_Msg_Loop = new S2C_Packet;
	_S2C_Msg_Main = new S2C_Packet;

	// Flatbuffer Builder
	Loop_Builder = new flatbuffers::FlatBufferBuilder;
	Main_Builder = new flatbuffers::FlatBufferBuilder;

	// 타이머 생성
	m_Timer = new DHTimer;
	m_Timer->SetFrame(60);

	// 로비 서버와 통신(클라이언트 입장)
	//m_Lobby_Client = new DHNetWorkAPI();
	//m_Lobby_Client->Initialize(DHNetWork_Name::DHNet);
	//m_Lobby_Client->Connect(729, "127.0.0.1");

	// 게임클라이언트를 받기 위한 게임서버 생성
	m_Game_Server = new DHNetWorkAPI();
	m_Game_Server->Initialize(DHNetWork_Name::DHNet, DHDEBUG_NONE);
	m_Game_Server->Accept(729, 4, 10);

	g_Packet_Send_Thread = new std::thread(std::bind(&GameManager::S2C_Loop, this));
}

GameManager::~GameManager()
{

}

void GameManager::S2C_Loop()
{
	// 게임서버는 계속 재사용되므로 이 로직은 계속 실행되어야 한다.
	while (true)
	{
		// 게임이 시작되었으면
		while (Is_Game_Playing.load())
		{
			// 60 프레임
			if (m_Timer->Ready_Frame())
			{
				// 움직임전송.
				S2C_Player_Move();
			}

			Sleep(0);
		}

		// 현재 게임서버에 연결되어 플레이중인 클라이언트가 존재하지 않는다면..
		Sleep(0);
	}
}

void GameManager::GameLogic()
{
	int num = 0;

	while (!Is_Game_End.load())
	{
		// 주기적으로 받은 패킷이 있으면 확인한다.
		if (m_Game_Server->Recv(Msg_Vec))
		{
			// 클라이언트로부터 받은 데이터들로 로직을 처리..
			for (auto& Msg_Packet : Msg_Vec)
			{
				// 클라이언트의 소켓 번호
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// 클라이언트가 보낸 메세지
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);

				// 클라이언트가 강제종료한 경우..
				if (C2S_Msg == NULL)
				{
					continue;
				}

				if (!Is_Game_Playing.load())
				{
					// 클라이언트가 준비되었다는 메세지가 들어옴.
					if (C2S_Msg->Packet_Type == C2S_LOADING_COMPLETE)
					{
						C2S_Player_Loading_Complete(_Recv_Socket_Num);
						// 게임에 시작하기위한 인원이 모두 들어왔으면 시작한다.
						if (Game_Max_User_Count == Connect_Player_List.size())
						{
							// 게임 시작.
							Is_Game_Playing.exchange(true);
							Is_Max_User.exchange(true);
							S2C_Start_Game();

							// 사용한데이터 해제.
							delete Msg_Packet.Packet;
							Msg_Packet.Packet = nullptr;
							Msg_Vec.clear();
							continue;
						}
					}
					continue;
				}

				// 만약 게임이 시작되었는데 누군가 나갔다면? 재접속을 허가한다.
				if (!Is_Max_User.load())
				{
					// 클라이언트가 준비되었다는 메세지가 들어옴.
					if (C2S_Msg->Packet_Type == C2S_LOADING_COMPLETE)
					{
						// 추후 로그인서버에서 유저의 Key값이 들어온다면 그때 체크해서 넣어주는 로직을 만들어 줄 예정..

						// ...

						if (Game_Max_User_Count == Connect_Player_List.size())
						{
							printf_s("[Eater Game Server] 플레이어 재접속\n");
							// 임시로 새로 접속한 유저에게도 Start_Game 메세지를 보내준다...
							_S2C_Msg_Main->Packet_Type = S2C_START_GAME;
							_S2C_Msg_Main->Packet_Size = 1;
							_S2C_Msg_Main->Packet_Buffer[0] = true;

							// 해당 타겟에게 보내는 메세지.
							m_Game_Server->Send(_S2C_Msg_Main,SEND_TYPE_TARGET, _Recv_Socket_Num);
							printf_s("[Eater Game Server] [S2C_START_GAME] 패킷 전송완료\n");

							Is_Max_User.exchange(true);

							// 사용한데이터 해제.
							delete Msg_Packet.Packet;
							Msg_Packet.Packet = nullptr;
							Msg_Vec.clear();
							continue;
						}
					}
				}

				// 플레이어 이동에 대한 처리..
				if (C2S_Msg->Packet_Type == C2S_PLAYER_MOVE_REQ)
				{
					if (C2S_Player_Move(C2S_Msg) == false)
					{
						// 로직실패..
					}
				}

				// 서버가 살아있는지 검사하는 패킷.
				if (C2S_Msg->Packet_Type == C2S_KEEP_ALIVE_CHECK_REQ)
				{
					if (C2S_Keep_Alive() == false)
					{
						// 로직실패..
					}
				}

				// 사용한데이터 해제.
				delete Msg_Packet.Packet;
				Msg_Packet.Packet = nullptr;
			}
			Msg_Vec.clear();
		}

		Sleep(0);
	}
}

void GameManager::S2C_Start_Game()
{	
	_S2C_Msg_Main->Packet_Type = S2C_START_GAME;
	_S2C_Msg_Main->Packet_Size = 1;
	_S2C_Msg_Main->Packet_Buffer[0] = true;

	// BroadCast
	m_Game_Server->Send(_S2C_Msg_Main);
	printf_s("[Eater Game Server] [S2C_START_GAME] 패킷 전송완료\n");
}

void GameManager::S2C_Player_Move()
{
	// 플레이어 리스트 자료를 만들기 위한 Vector ( 플레이어의 정보를 담음 )
	std::vector<flatbuffers::Offset<Eater::PlayerData::Player>> _Player_List;
	int Number = 0;
	for (auto _Player : Connect_Player_List)
	{
		auto _Position = Eater::PlayerData::Vec3(0, 0, 0);
		auto _Rot = Eater::PlayerData::Vec3(0, 0, 0);
		int Type;
		//각각의 객체1,2,의 데이터를 넘겨준다
		if (Number == 0)
		{
			_Position = Eater::PlayerData::Vec3(Player01_Pos_x, Player01_Pos_y, Player01_Pos_z);
			_Rot = Eater::PlayerData::Vec3(Player01_Rot_x, Player01_Rot_y, Player01_Rot_z);
			Type = Player01_AnimeType;
		}
		else
		{
			_Position = Eater::PlayerData::Vec3(Player02_Pos_x, Player02_Pos_y, Player02_Pos_z);
			_Rot = Eater::PlayerData::Vec3(Player02_Rot_x, Player02_Rot_y, Player02_Rot_z);
			Type = Player02_AnimeType;
		}

		auto Player_Data = Eater::PlayerData::CreatePlayer(*Loop_Builder,_Player.second,&_Position,&_Rot,100, Type);
		
		_Player_List.push_back(Player_Data);
		Number++;
	}

	auto Player_List_Data = CreatePlayerListDirect(*Loop_Builder, &_Player_List);
	// 데이터 직렬화 완료.
	Loop_Builder->Finish(Player_List_Data);

	// 패킷 헤더를 붙여 보내준다.
	_S2C_Msg_Loop->Packet_Type = S2C_PLAYER_MOVE_RES;
	_S2C_Msg_Loop->Packet_Size = Loop_Builder->GetSize();
	memcpy_s(_S2C_Msg_Loop->Packet_Buffer, Loop_Builder->GetSize(), Loop_Builder->GetBufferPointer(), Loop_Builder->GetSize());

	// BroadCast
	if (m_Game_Server->Send(_S2C_Msg_Loop) != Game_Max_User_Count)
	{
		if (Is_Max_User.load())
		{
			printf_s("[Eater Game Server] 플레이어 접속 종료\n");
			Is_Max_User.exchange(false);
		}
	}

	// 사용한 빌더 초기화.
	Loop_Builder->Clear();
}

BOOL GameManager::C2S_Player_Move(C2S_Packet* Packet)
{
	// 데이터 캐스팅.
	const uint8_t* Recv_Data_Ptr = (unsigned char*)Packet->Packet_Buffer;

	const Eater::PlayerData::Player* Recv_Player_List_Data = flatbuffers::GetRoot<Eater::PlayerData::Player>(Recv_Data_Ptr);

	auto Player_ID = Recv_Player_List_Data->identifier();


	//client 1 이 움직인 객체1의정보를 저장
	if (Player_ID == 1)
	{
		Player01_AnimeType = Recv_Player_List_Data->animation_type();
		Player01_Pos_x = Recv_Player_List_Data->pos()->x();
		Player01_Pos_y = Recv_Player_List_Data->pos()->y();
		Player01_Pos_z = Recv_Player_List_Data->pos()->z();

		Player01_Rot_x = Recv_Player_List_Data->mov_vector()->x();
		Player01_Rot_y = Recv_Player_List_Data->mov_vector()->y();
		Player01_Rot_z = Recv_Player_List_Data->mov_vector()->z();
	}
	else if (Player_ID == 2)
	{
		Player02_AnimeType = Recv_Player_List_Data->animation_type();
		Player02_Pos_x = Recv_Player_List_Data->pos()->x();
		Player02_Pos_y = Recv_Player_List_Data->pos()->y();
		Player02_Pos_z = Recv_Player_List_Data->pos()->z();

		Player02_Rot_x = Recv_Player_List_Data->mov_vector()->x();
		Player02_Rot_y = Recv_Player_List_Data->mov_vector()->y();
		Player02_Rot_z = Recv_Player_List_Data->mov_vector()->z();
	}



	//printf_s("[Recv_Player_Position] x : %.2f, y : %.2f, z : %.2f", Recv_Player_Data->pos()->x(), Recv_Player_Data->pos()->y(), Recv_Player_Data->pos()->z());
	//printf_s("[Recv_Player_Mov_Vector] x : %.2f, y : %.2f, z : %.2f", Recv_Player_Data->mov_vector()->x(), Recv_Player_Data->mov_vector()->y(), Recv_Player_Data->mov_vector()->z());

	return true;
}

BOOL GameManager::C2S_Keep_Alive()
{
	return true;
}

BOOL GameManager::C2S_Player_Loading_Complete(SOCKET _Socket_Num)
{
	// 소켓 번호와 플레이어 저장. (추후에 들어온 데이터가 몇번 유저인지 바로 찾을 수 있게)
	Connect_Player_List.insert({ _Socket_Num, (int)EATER_PLAYER_01 + Connect_Player_List.size() });

	_S2C_Msg_Main->Packet_Type = S2C_CLIENT_INFO;
	_S2C_Msg_Main->Packet_Size = 1;
	_S2C_Msg_Main->Packet_Buffer[0] = (int)Connect_Player_List.size();

	m_Game_Server->Send(_S2C_Msg_Main);
	printf_s("[Eater Game Server] 플레이어 접속\n");

	return true;
}
