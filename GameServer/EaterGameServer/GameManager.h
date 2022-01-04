#pragma once

#include "LobbyManager.h"
#include <atomic>
#include <map>

class DHNetWorkAPI;
class DHTimer;
struct S2C_Packet;

namespace flatbuffers
{
	class FlatBufferBuilder;
}

namespace Eater 
{
	namespace PlayerData 
	{
		struct PlayerBuilder;
		struct PlayerListBuilder;
	}
}

class GameManager
{
	// 게임서버에 연결된 플레이어 정보
private:
	// 추후에 SOCKET이 키값이 아니고 로그인서버에서 준 Key값으로 대체될 예정..
	std::map<SOCKET, int> Connect_Player_List;

	// 로비서버에서 받아야되는 정보들..
private:
	// 몇명이되면 시작할건가?
	int Game_Max_User_Count = 2;

private:
	DHNetWorkAPI* m_Game_Server = nullptr;
	DHNetWorkAPI* m_Lobby_Client = nullptr;

	DHTimer* m_Timer = nullptr;

	// Loop Thread 용
	S2C_Packet* _S2C_Msg_Loop = nullptr;
	flatbuffers::FlatBufferBuilder* Loop_Builder = nullptr;
	// Main Logic  용
	S2C_Packet* _S2C_Msg_Main = nullptr;
	flatbuffers::FlatBufferBuilder* Main_Builder = nullptr;

	// 게임의 종료여부 (종료시 현재 게임서버를 초기화하고 다시 사용가능한 상태로 로비서버에게 알려줘야 한다.)
	std::atomic<bool> Is_Game_End = false;
	// 현재 게임이 진행중인가? (2명이서 진행한 게임이 시작되고, 한명이 나가더라도 진행은 가능하도록 하기위해서 별도의 플래그로 지정하였다.)
	std::atomic<bool> Is_Game_Playing = false;
	// 현재 게임이 최대 인원수로 진행되고 있는가? (추후 유저가 재접속할 수도 있으니 구분한다.)
	std::atomic<bool> Is_Max_User = false;

	// 클라이언트에서 온 메세지를 받는 벡터.
	std::vector<Network_Message> Msg_Vec;

	// 일정 주기로 보내줘야 하는 패킷에 대한 쓰레드.
	std::thread* g_Packet_Send_Thread = nullptr;

public:
	GameManager();
	~GameManager();

	void S2C_Loop();
	void GameLogic();
	// S2C Function
public:
	void	S2C_Start_Game();
	void	S2C_Player_Move();

	int Player01_AnimeType;
	float Player01_Pos_x = 0;
	float Player01_Pos_y = 0;
	float Player01_Pos_z = 0;
	float Player01_Rot_x = 0;
	float Player01_Rot_y = 0;
	float Player01_Rot_z = 0;


	int Player02_AnimeType;
	float Player02_Pos_x = 0;
	float Player02_Pos_y = 0;
	float Player02_Pos_z = 0;
	float Player02_Rot_x = 0;
	float Player02_Rot_y = 0;
	float Player02_Rot_z = 0;



	// C2S Function
public:
	BOOL	C2S_Player_Move(C2S_Packet* Packet);
	BOOL	C2S_Keep_Alive();
	BOOL	C2S_Player_Loading_Complete(SOCKET _Socket_Num);
};

