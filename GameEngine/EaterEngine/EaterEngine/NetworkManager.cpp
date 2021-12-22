#include "NetworkManager.h"

#define CONNECT_IP "127.0.0.1"
#define CONNET_PORT 729

#include <iostream>
#include "DHNetWorkAPI.h"
#include "SharedDataStruct.h"
#include "GameClientGameServerPacketDefine.h"
#include "PlayerData_generated.h"
#include "SimpleMath.h"



#include "GameObject.h"
#include "UnitNet.h"
#include "Transform.h"
#include "MeshFilter.h"


std::vector<Network_Message> Msg_Vec;

UnitNet* NetworkManager::URES_01 = nullptr;
UnitNet* NetworkManager::URES_02 = nullptr;

NetworkManager::NetworkManager()
{
	Recv_Packet = nullptr;
	my_NetWork = nullptr;

	TestData = nullptr;
}

NetworkManager::~NetworkManager()
{


}

void NetworkManager::Initialize()
{
	// Recv 데이터 받아오기
	std::vector<Network_Message> Msg_Vec;

	S2C_Packet* Recv_Packet = nullptr;

	my_NetWork = new DHNetWorkAPI();
	my_NetWork->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);

	while (!my_NetWork->Connect(CONNET_PORT, CONNECT_IP));


}

void NetworkManager::Update()
{
	NETWORK_RECV();
	NETWORK_SAND();
}

void NetworkManager::Add_USER_OBJ(int SocketNumber)
{
	GameObject* obj = new GameObject();
	obj->AddComponent<Transform>();
	obj->AddComponent<MeshFilter>();
	obj->AddComponent<UnitNet>();
}

void NetworkManager::NETWORK_RECV()
{
	///서버-->-->-->-->-->클라
	if (my_NetWork->Recv(Msg_Vec))
	{
		for (auto& S2C_Msg : Msg_Vec)
		{
			Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);
			switch (Recv_Packet->Packet_Type)
			{
				case S2C_PLAYER_MOVE_RES:
				{
					///플레이어가 움직였을때
					GetPlayerData(Recv_Packet, S2C_Msg.Socket);
					break;
				}

				case C2S_KEEP_ALIVE_CHECK_REQ:
				{
					///연결이 끊켰을때
					break;
				}
			}

			delete S2C_Msg.Packet;
			S2C_Msg.Packet = nullptr;
		}

		Msg_Vec.clear();
	}
}

void NetworkManager::NETWORK_SAND()
{
	///클라-->-->-->-->-->서버





}

bool NetworkManager::CHECK_USER(int SocketNumber)
{
	int Size = (int)UresList.size();
	for (int i = 0; i < Size; i++)
	{
		if (UresList[i] == SocketNumber)
		{
			return true;
		}
	}

	//없는 유저가 들어왔다면 유저 리스트에 넣는다
	Add_USER_OBJ(SocketNumber);
	UresList.push_back(SocketNumber);

	return false;
}

void NetworkManager::GetPlayerData(S2C_Packet* Packet, int SocketNumber)
{
	const uint8_t* Recv_Data_Ptr = (unsigned char*)Packet->Packet_Buffer;
	const Eater::PlayerData::Player* Recv_Player_Data = Eater::PlayerData::GetPlayer(Recv_Data_Ptr);

	if (CHECK_USER(SocketNumber) == true)
	{
		//기존 유저의 대한 데이터
	}
	else
	{
		//새로 들어온 유저의 데이터
	}

}

void NetworkManager::PushData(const Eater::PlayerData::Player* data)
{
	//체력
	//float hp = data->hp();
	////마나
	//float Mana = data->mana();
	//
	////위치
	//float X = data->pos()->x();
	//float y = data->pos()->y();
	//float z = data->pos()->z();
	//
	////이름
	//std::string name = data->name()->c_str();
}
