#include "GameManager.h"
#include "DHNetWorkAPI.h"
#include "GameClientGameServerPacketDefine.h"
#include "GameProtocol/PlayerData_generated.h"
#include "DHTimer.h"

GameManager::GameManager()
{
	// ������ ������ ���� ��Ŷ ����(��Ȱ�� �� ����)
	_S2C_Msg_Loop = new S2C_Packet;
	_S2C_Msg_Main = new S2C_Packet;

	// Flatbuffer Builder
	Loop_Builder = new flatbuffers::FlatBufferBuilder;
	Main_Builder = new flatbuffers::FlatBufferBuilder;

	// Ÿ�̸� ����
	m_Timer = new DHTimer;
	m_Timer->SetFrame(60);

	// �κ� ������ ���(Ŭ���̾�Ʈ ����)
	//m_Lobby_Client = new DHNetWorkAPI();
	//m_Lobby_Client->Initialize(DHNetWork_Name::DHNet);
	//m_Lobby_Client->Connect(729, "127.0.0.1");

	// ����Ŭ���̾�Ʈ�� �ޱ� ���� ���Ӽ��� ����
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
	// ���Ӽ����� ��� ����ǹǷ� �� ������ ��� ����Ǿ�� �Ѵ�.
	while (true)
	{
		// ������ ���۵Ǿ�����
		while (Is_Game_Playing.load())
		{
			// 60 ������
			if (m_Timer->Ready_Frame())
			{
				// ����������.
				S2C_Player_Move();
			}

			Sleep(0);
		}

		// ���� ���Ӽ����� ����Ǿ� �÷������� Ŭ���̾�Ʈ�� �������� �ʴ´ٸ�..
		Sleep(0);
	}
}

void GameManager::GameLogic()
{
	int num = 0;

	while (!Is_Game_End.load())
	{
		// �ֱ������� ���� ��Ŷ�� ������ Ȯ���Ѵ�.
		if (m_Game_Server->Recv(Msg_Vec))
		{
			// Ŭ���̾�Ʈ�κ��� ���� �����͵�� ������ ó��..
			for (auto& Msg_Packet : Msg_Vec)
			{
				// Ŭ���̾�Ʈ�� ���� ��ȣ
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// Ŭ���̾�Ʈ�� ���� �޼���
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);

				// Ŭ���̾�Ʈ�� ���������� ���..
				if (C2S_Msg == NULL)
				{
					continue;
				}

				if (!Is_Game_Playing.load())
				{
					// Ŭ���̾�Ʈ�� �غ�Ǿ��ٴ� �޼����� ����.
					if (C2S_Msg->Packet_Type == C2S_LOADING_COMPLETE)
					{
						C2S_Player_Loading_Complete(_Recv_Socket_Num);
						// ���ӿ� �����ϱ����� �ο��� ��� �������� �����Ѵ�.
						if (Game_Max_User_Count == Connect_Player_List.size())
						{
							// ���� ����.
							Is_Game_Playing.exchange(true);
							Is_Max_User.exchange(true);
							S2C_Start_Game();

							// ����ѵ����� ����.
							delete Msg_Packet.Packet;
							Msg_Packet.Packet = nullptr;
							Msg_Vec.clear();
							continue;
						}
					}
					continue;
				}

				// ���� ������ ���۵Ǿ��µ� ������ �����ٸ�? �������� �㰡�Ѵ�.
				if (!Is_Max_User.load())
				{
					// Ŭ���̾�Ʈ�� �غ�Ǿ��ٴ� �޼����� ����.
					if (C2S_Msg->Packet_Type == C2S_LOADING_COMPLETE)
					{
						// ���� �α��μ������� ������ Key���� ���´ٸ� �׶� üũ�ؼ� �־��ִ� ������ ����� �� ����..

						// ...

						if (Game_Max_User_Count == Connect_Player_List.size())
						{
							printf_s("[Eater Game Server] �÷��̾� ������\n");
							// �ӽ÷� ���� ������ �������Ե� Start_Game �޼����� �����ش�...
							_S2C_Msg_Main->Packet_Type = S2C_START_GAME;
							_S2C_Msg_Main->Packet_Size = 1;
							_S2C_Msg_Main->Packet_Buffer[0] = true;

							// �ش� Ÿ�ٿ��� ������ �޼���.
							m_Game_Server->Send(_S2C_Msg_Main,SEND_TYPE_TARGET, _Recv_Socket_Num);
							printf_s("[Eater Game Server] [S2C_START_GAME] ��Ŷ ���ۿϷ�\n");

							Is_Max_User.exchange(true);

							// ����ѵ����� ����.
							delete Msg_Packet.Packet;
							Msg_Packet.Packet = nullptr;
							Msg_Vec.clear();
							continue;
						}
					}
				}

				// �÷��̾� �̵��� ���� ó��..
				if (C2S_Msg->Packet_Type == C2S_PLAYER_MOVE_REQ)
				{
					if (C2S_Player_Move(C2S_Msg) == false)
					{
						// ��������..
					}
				}

				// ������ ����ִ��� �˻��ϴ� ��Ŷ.
				if (C2S_Msg->Packet_Type == C2S_KEEP_ALIVE_CHECK_REQ)
				{
					if (C2S_Keep_Alive() == false)
					{
						// ��������..
					}
				}

				// ����ѵ����� ����.
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
	printf_s("[Eater Game Server] [S2C_START_GAME] ��Ŷ ���ۿϷ�\n");
}

void GameManager::S2C_Player_Move()
{
	// �÷��̾� ����Ʈ �ڷḦ ����� ���� Vector ( �÷��̾��� ������ ���� )
	std::vector<flatbuffers::Offset<Eater::PlayerData::Player>> _Player_List;
	int Number = 0;
	for (auto _Player : Connect_Player_List)
	{
		auto _Position = Eater::PlayerData::Vec3(0, 0, 0);
		auto _Rot = Eater::PlayerData::Vec3(0, 0, 0);
		int Type;
		//������ ��ü1,2,�� �����͸� �Ѱ��ش�
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
	// ������ ����ȭ �Ϸ�.
	Loop_Builder->Finish(Player_List_Data);

	// ��Ŷ ����� �ٿ� �����ش�.
	_S2C_Msg_Loop->Packet_Type = S2C_PLAYER_MOVE_RES;
	_S2C_Msg_Loop->Packet_Size = Loop_Builder->GetSize();
	memcpy_s(_S2C_Msg_Loop->Packet_Buffer, Loop_Builder->GetSize(), Loop_Builder->GetBufferPointer(), Loop_Builder->GetSize());

	// BroadCast
	if (m_Game_Server->Send(_S2C_Msg_Loop) != Game_Max_User_Count)
	{
		if (Is_Max_User.load())
		{
			printf_s("[Eater Game Server] �÷��̾� ���� ����\n");
			Is_Max_User.exchange(false);
		}
	}

	// ����� ���� �ʱ�ȭ.
	Loop_Builder->Clear();
}

BOOL GameManager::C2S_Player_Move(C2S_Packet* Packet)
{
	// ������ ĳ����.
	const uint8_t* Recv_Data_Ptr = (unsigned char*)Packet->Packet_Buffer;

	const Eater::PlayerData::Player* Recv_Player_List_Data = flatbuffers::GetRoot<Eater::PlayerData::Player>(Recv_Data_Ptr);

	auto Player_ID = Recv_Player_List_Data->identifier();


	//client 1 �� ������ ��ü1�������� ����
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
	// ���� ��ȣ�� �÷��̾� ����. (���Ŀ� ���� �����Ͱ� ��� �������� �ٷ� ã�� �� �ְ�)
	Connect_Player_List.insert({ _Socket_Num, (int)EATER_PLAYER_01 + Connect_Player_List.size() });

	_S2C_Msg_Main->Packet_Type = S2C_CLIENT_INFO;
	_S2C_Msg_Main->Packet_Size = 1;
	_S2C_Msg_Main->Packet_Buffer[0] = (int)Connect_Player_List.size();

	m_Game_Server->Send(_S2C_Msg_Main);
	printf_s("[Eater Game Server] �÷��̾� ����\n");

	return true;
}
