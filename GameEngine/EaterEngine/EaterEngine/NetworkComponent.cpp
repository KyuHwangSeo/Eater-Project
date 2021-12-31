#include "NetworkComponent.h"
#include "NetworkManager.h"


NetworkComponent::NetworkComponent()
{
	//변수 초기화
	PacketNumber = -1;

	Recv_Packet = nullptr;
	Send_Packet = nullptr;


	//컨퍼넌트가 생성 되었을때 네트워크 매니저에 오브젝트를 올린다
	//NetworkManager::SET_USER_OBJ(this);
}

NetworkComponent::~NetworkComponent()
{

}

NetworkManager* NetworkComponent::GetNetWorkManager()
{
	if (NetManager != nullptr)
	{
		return NetManager;
	}
	else
	{
		return nullptr;
	}
}

