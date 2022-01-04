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
	// ���Ӽ����� ����� �÷��̾� ����
private:
	// ���Ŀ� SOCKET�� Ű���� �ƴϰ� �α��μ������� �� Key������ ��ü�� ����..
	std::map<SOCKET, int> Connect_Player_List;

	// �κ񼭹����� �޾ƾߵǴ� ������..
private:
	// ����̵Ǹ� �����Ұǰ�?
	int Game_Max_User_Count = 2;

private:
	DHNetWorkAPI* m_Game_Server = nullptr;
	DHNetWorkAPI* m_Lobby_Client = nullptr;

	DHTimer* m_Timer = nullptr;

	// Loop Thread ��
	S2C_Packet* _S2C_Msg_Loop = nullptr;
	flatbuffers::FlatBufferBuilder* Loop_Builder = nullptr;
	// Main Logic  ��
	S2C_Packet* _S2C_Msg_Main = nullptr;
	flatbuffers::FlatBufferBuilder* Main_Builder = nullptr;

	// ������ ���Ῡ�� (����� ���� ���Ӽ����� �ʱ�ȭ�ϰ� �ٽ� ��밡���� ���·� �κ񼭹����� �˷���� �Ѵ�.)
	std::atomic<bool> Is_Game_End = false;
	// ���� ������ �������ΰ�? (2���̼� ������ ������ ���۵ǰ�, �Ѹ��� �������� ������ �����ϵ��� �ϱ����ؼ� ������ �÷��׷� �����Ͽ���.)
	std::atomic<bool> Is_Game_Playing = false;
	// ���� ������ �ִ� �ο����� ����ǰ� �ִ°�? (���� ������ �������� ���� ������ �����Ѵ�.)
	std::atomic<bool> Is_Max_User = false;

	// Ŭ���̾�Ʈ���� �� �޼����� �޴� ����.
	std::vector<Network_Message> Msg_Vec;

	// ���� �ֱ�� ������� �ϴ� ��Ŷ�� ���� ������.
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

