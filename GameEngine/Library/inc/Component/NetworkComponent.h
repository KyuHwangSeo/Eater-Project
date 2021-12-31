#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"

class NetworkManager;
struct NetworkData;
struct S2C_Packet;
struct C2S_Packet;

///네트워크 관련된 컨퍼넌트 들은 이클래스를 상속받아 제작됨
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
	S2C_Packet* Recv_Packet;	//보낼 데이터
	C2S_Packet* Send_Packet;	//받을 데이터

	void* RecvData;	//받을 데이터
	void* SendData;	//보내줄 데이터

	NetworkManager* NetManager;
private:
	friend NetworkManager;
};


