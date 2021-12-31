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
	//�����Ҷ� ����� ��Ŷ�� ��������
	delete Send_Packet;
	delete mBuilder;
	
	Send_Packet = nullptr;
	mBuilder	= nullptr;
}

void NetworkManager::Initialize()
{
	// Recv ������ �޾ƿ���
	std::vector<Network_Message> Msg_Vec;
	
	//��Ʈ��ũ ���� ����
	DHNetWork = new DHNetWorkAPI();
	DHNetWork->Initialize(DHNetWork_Name::DHNet, DHDEBUG_DETAIL);

	//������Ŷ ������ ����
	Send_Packet = new C2S_Packet();

	mBuilder = new flatbuffers::FlatBufferBuilder();
}

void NetworkManager::Update()
{
	//���� ���� �κ� �����͸� ��� �޴´�
	NETWORK_RECV();
}

void NetworkManager::SetClientNetworkManager(NetworkManagerComponent* mManager)
{
	//Ŭ���̾�Ʈ�� �Ŵ��� ���
	mClientNetworkManager = mManager;
	mClientNetworkManager->_Builder = mBuilder;
}

void NetworkManager::NETWORK_RECV()
{
	if (StartNework == false) { return; }

	///����-->-->-->-->-->Ŭ��
	if (DHNetWork->Recv(Msg_Vec))
	{
		for (auto& S2C_Msg : Msg_Vec)
		{
			//���� �����͸� ��Ŷ���� ����
			Recv_Packet = static_cast<S2C_Packet*>(S2C_Msg.Packet);
			
			//�����͸� Ŭ���̾�Ʈ ��Ʈ��ũ �Ŵ����� �����ش�
			uint8_t* Recv_Data_Ptr = (unsigned char*)Recv_Packet->Packet_Buffer;
			mClientNetworkManager->RECV(Recv_Data_Ptr, Recv_Packet->Packet_Type);
		}
		Msg_Vec.clear();
	}
}

void NetworkManager::NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type)
{
	if (StartNework == false) { return; }

	///Ŭ��-->-->-->-->-->����	
	//ä�� ������ ��Ŷ�� �����
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

	//���� ������ ������
	DHNetWork->Send(Send_Packet);

	//�ʱ�ȭ
	Builder->Clear();
}

void NetworkManager::C2S_LOADING_COMPLETE_SEND()
{
	///���� üũ
	while (!DHNetWork->Connect(CONNET_PORT, CONNECT_IP))
	{
		DebugManager::Print("���� �غ���");
	}

	DebugManager::Print("���� �Ϸ�");

	//��Ʈ��ũ�� ����Ѵ�
	StartNework = true;

	DebugManager::Print("�ε� �Ϸ�");
	C2S_Packet* Send_Packet = nullptr;
	Send_Packet = new C2S_Packet();
	Send_Packet->Packet_Type = C2S_LOADING_COMPLETE;
	Send_Packet->Packet_Size = 0;

	// �ε��� �������� �˸�.
	DHNetWork->Send(Send_Packet);
}
