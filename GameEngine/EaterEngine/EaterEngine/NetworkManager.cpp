#include "NetworkManager.h"

//#define CONNECT_IP "127.0.0.1"
#define CONNECT_IP "221.163.91.100"
#define CONNET_PORT 729

#include <iostream>
#include "DHNetWorkAPI.h"
#include "SharedDataStruct.h"
#include "GameClientGameServerPacketDefine.h"
#include "PlayerData_generated.h"


#include "GameObject.h"
#include "NetworkComponent.h"
#include "DebugManager.h"
#include "NetWorkData.h"
#include "NetworkManagerComponent.h"

std::vector<Network_Message> Msg_Vec;
NetworkManager::NetworkManager()
{
	Recv_Packet = nullptr;
	Send_Packet = nullptr;
	DHNetWork	= nullptr;
	mBuilder	= nullptr;
	StartNework = false;
	mClientNetworkManager = nullptr;
}

NetworkManager::~NetworkManager()
{
	//종료할때 사용한 페킷은 삭제주자
	delete Send_Packet;
	delete mBuilder;
	
	Send_Packet = nullptr;
	mBuilder	= nullptr;
}

void NetworkManager::Initialize()
{
	// Recv 데이터 받아오기
	std::vector<Network_Message> Msg_Vec;
	
	//네트워크 엔진 생성
	DHNetWork = new DHNetWorkAPI();
	DHNetWork->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);

	//보낼페킷 데이터 생성
	Send_Packet = new C2S_Packet();

	mBuilder = new flatbuffers::FlatBufferBuilder();
}

void NetworkManager::Update()
{
	//메인 루프 부분 데이터를 계속 받는다
	NETWORK_RECV();
}

void NetworkManager::SetClientNetworkManager(NetworkManagerComponent* mManager)
{
	//클라이언트쪽 매니저 등록
	mClientNetworkManager = mManager;
	mClientNetworkManager->_Builder = mBuilder;
}

void NetworkManager::NETWORK_RECV()
{
	if (StartNework == false) { return; }

	///서버-->-->-->-->-->클라
	if (DHNetWork->Recv(Msg_Vec))
	{
		for (auto& S2C_Msg : Msg_Vec)
		{
			//받은 데이터를 패킷으로 변경
			Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);
			
			//데이터를 클라이언트 네트워크 매니저로 보내준다
			uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;
			mClientNetworkManager->RECV(Recv_Data_Ptr, Recv_Packet->Packet_Type);
		}
		Msg_Vec.clear();
	}
}

void NetworkManager::NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type)
{
	if (StartNework == false) { return; }

	///클라-->-->-->-->-->서버	
	//채운 값으로 패킷을 만들고
	Send_Packet->Packet_Type = Type;
	Send_Packet->Packet_Size = Builder->GetSize();

	int Size = (int)Builder->GetSize();
	if (Size != 0)
	{
		memcpy_s
		(
			Send_Packet->Packet_Buffer,
			Builder->GetSize(),
			Builder->GetBufferPointer(),
			Builder->GetSize()
		);
	}

	//서버 쪽으로 보낸다
	DHNetWork->Send(Send_Packet);

	//초기화
	Builder->Clear();
}

void NetworkManager::C2S_LOADING_COMPLETE_SEND()
{
	///연결 체크
	while (!DHNetWork->Connect(CONNET_PORT, CONNECT_IP))
	{
		DebugManager::Print("연결 준비중");
	}

	DebugManager::Print("연결 완료");

	//네트워크를 사용한다
	StartNework = true;

	DebugManager::Print("로딩 완료");
	C2S_Packet* Send_Packet = nullptr;
	Send_Packet = new C2S_Packet();
	Send_Packet->Packet_Type = C2S_LOADING_COMPLETE;
	Send_Packet->Packet_Size = 0;

	// 로딩이 끝났음을 알림.
	DHNetWork->Send(Send_Packet);
}
