#include "NetworkComponent.h"
#include "NetworkManager.h"


NetworkComponent::NetworkComponent()
{
	//���� �ʱ�ȭ
	PacketNumber = -1;

	Recv_Packet = nullptr;
	Send_Packet = nullptr;


	//���۳�Ʈ�� ���� �Ǿ����� ��Ʈ��ũ �Ŵ����� ������Ʈ�� �ø���
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

