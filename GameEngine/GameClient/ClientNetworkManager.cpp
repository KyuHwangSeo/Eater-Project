#include "ClientNetworkManager.h"
#include "NetworkComponent.h"
#include "NetwrokPlayer.h"
#include "GameClientGameServerPacketDefine.h"
#include "MainHeader.h"

#include "PlayerData_generated.h"


std::vector<NetwrokPlayer*> ClientNetworkManager::UserObjList;
ClientNetworkManager::ClientNetworkManager()
{
	ClientNumber = -1;
}

ClientNetworkManager::~ClientNetworkManager()
{

}

void ClientNetworkManager::EndUpdate()
{
	C2S_PLAYER_MOVE_SEND();
}

void ClientNetworkManager::AddPlayerObject(NetwrokPlayer* obj)
{
	UserObjList.push_back(obj);
}

void ClientNetworkManager::RECV(const uint8_t* Data, int type)
{
	switch(type)
	{
	case S2C_START_GAME: //게임 시작
		break;
	case S2C_PLAYER_MOVE_RES: //플레이어 움직여라
		S2C_PLAYER_MOVE_RECV(Data, type);
		break;
	case S2C_CLIENT_INFO://클라이언트 넘버
		S2C_CLIENT_INFO_RECV(Data, type);
		break;
	case 3:
		break;

	}
}

void ClientNetworkManager::C2S_PLAYER_MOVE_SEND()
{
	if (ClientNumber == -1) { return; }

	int index = ClientNumber - 1;
	NetwrokPlayer* Temp = UserObjList[index];
	float P_x = Temp->PositionX;
	float P_y = Temp->PositionY;
	float P_z = Temp->PositionZ;

	float D_x = Temp->DirectionX;
	float D_y = Temp->DirectionY;
	float D_z = Temp->DirectionZ;

	int Number		= Temp->PlayerNumber;	//객체 번호
	int AnimeType	= Temp->AnimeType;		//애니메이션 번호
	float HP = 100;

	//PlayerData를 생성한다
	Eater::PlayerData::Vec3 _Position = Eater::PlayerData::Vec3(P_x, P_y, P_z);
	Eater::PlayerData::Vec3 _Mov_Vec = Eater::PlayerData::Vec3(D_x, D_y, D_z);
	
	auto Player_Data = Eater::PlayerData::CreatePlayer(*_Builder, ClientNumber, &_Position, &_Mov_Vec, HP, AnimeType);

	//생성종료
	_Builder->Finish(Player_Data);

	//데이터를 타입과 함께 엔진쪽으로 보내준다 
	Network_Send(_Builder, C2S_PLAYER_MOVE_REQ);
}

void ClientNetworkManager::S2C_PLAYER_MOVE_RECV(const uint8_t* Data, int type)
{
	//받은 데이터를 변환
	const Eater::PlayerData::PlayerList* Recv_Player_List_Data = flatbuffers::GetRoot<Eater::PlayerData::PlayerList>(Data);
	auto Player_List_Vector = Recv_Player_List_Data->players();
	
	//변환한 플레이어 리스트를 각각의 오브젝트로 넣어준다
	int Size = (int)UserObjList.size();
	for (int i = 0; i < Size; i++)
	{
		Eater::PlayerData::Player* Temp = const_cast<Eater::PlayerData::Player*>(Player_List_Vector->Get(i));
		UserObjList[i]->RECV(Temp, type);
	}
}

void ClientNetworkManager::S2C_CLIENT_INFO_RECV(const uint8_t* Data,int type)
{
	//클라이언트 넘버를 처음 한번만 받는다
	if (ClientNumber == -1)
	{
		uint8_t* num = const_cast<uint8_t*>(Data);
		ClientNumber = num[0];
		int Size = (int)UserObjList.size();

		//모든 객체에게 너는 몇번 클라이언트인지 보낸다
		for (int i = 0; i < Size; i++)
		{
			UserObjList[i]->RECV(&ClientNumber, type);
		}
	}
}
