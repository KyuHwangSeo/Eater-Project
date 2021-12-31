#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"

class NetworkManager;
struct NetworkData;
struct S2C_Packet;
struct C2S_Packet;

///��Ʈ��ũ ���õ� ���۳�Ʈ ���� ��Ŭ������ ��ӹ޾� ���۵�
class NetworkComponent : public Component
{
public:
	EATER_ENGINEDLL NetworkComponent();
	EATER_ENGINEDLL ~NetworkComponent();
	EATER_ENGINEDLL NetworkManager* GetNetWorkManager();
	virtual void RECV(void* Data,int type) {};
	virtual void SEND(void* Data,int type) {};
protected:


	int PacketNumber;
	S2C_Packet* Recv_Packet;	//���� ������
	C2S_Packet* Send_Packet;	//���� ������

	void* RecvData;	//���� ������
	void* SendData;	//������ ������

	NetworkManager* NetManager;
private:
	friend NetworkManager;
};


