#pragma once
#include <vector>

namespace Eater
{
	namespace PlayerData 
	{
		struct Player;
	}
}

namespace flatbuffers
{
	class FlatBufferBuilder;
}


struct S2C_Packet;
struct C2S_Packet;
class DHNetWorkAPI;
class GameObject;
class NetworkManagerComponent;
class NetworkComponent;

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
	void Initialize();
	void Update();
public:
	///Ŭ���̾�Ʈ�� ������ �Ŵ��� �ޱ�
	void SetClientNetworkManager(NetworkManagerComponent* mManager);
public:
	///Sand
	void NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type);
	void C2S_LOADING_COMPLETE_SEND();
private:
	///Recv
	void NETWORK_RECV();
private:
	S2C_Packet*		Recv_Packet;	//���� ������
	C2S_Packet*		Send_Packet;	//������ ������
	DHNetWorkAPI*	DHNetWork;		//����� ���� ��Ʈ��ũ Engine
private:
	NetworkManagerComponent* mClientNetworkManager;	//Ŭ�󿡼� ���� ��Ʈ��ũ �Ŵ���
	flatbuffers::FlatBufferBuilder* mBuilder;		//Ŭ�������� ������ ������
	bool StartNework;
};