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
	
	///유저가 연결이 되었을떄
	void Add_USER_OBJ(int SocketNumber);
private:
	///데이터 받기
	void NETWORK_RECV();
	///데이터 보내기
	void NETWORK_SAND();
	//유저 체크
	bool CHECK_USER(int SocketNumber);


	//플레이어 데이터를 받는다
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