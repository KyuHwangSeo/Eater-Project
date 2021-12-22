#pragma once
#include <vector>

namespace Eater
{
	namespace PlayerData 
	{
		struct Player;
	}
}

class S2C_Packet;
class DHNetWorkAPI;
class GameObject;
class UnitNet;

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
	void Initialize();
	void Update();
	
	///������ ������ �Ǿ�����
	void Add_USER_OBJ(int SocketNumber);
private:
	///������ �ޱ�
	void NETWORK_RECV();
	///������ ������
	void NETWORK_SAND();
	//���� üũ
	bool CHECK_USER(int SocketNumber);


	//�÷��̾� �����͸� �޴´�
	void GetPlayerData(S2C_Packet* data,int SocketNumber);

	void PushData(const Eater::PlayerData::Player* data);

	S2C_Packet*		Recv_Packet;
	DHNetWorkAPI*	my_NetWork;

	Eater::PlayerData::Player* TestData;
private:
	std::vector<int> UresList;

	static UnitNet* URES_01;
	static UnitNet* URES_02;
};