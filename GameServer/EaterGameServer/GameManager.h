#pragma once

#include "LobbyManager.h"

class DHNetWorkAPI;
class DHTimer;
struct S2C_Packet;

class GameManager
{
private:
	DHNetWorkAPI* m_Game_Server = nullptr;
	DHNetWorkAPI* m_Lobby_Client = nullptr;

	DHTimer* m_Timer = nullptr;

	S2C_Packet* _S2C_Msg = nullptr;

	bool Is_Game_End = false;

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
	void	S2C_Player_Move();


	// C2S Function
public:
	BOOL	C2S_Player_Move();
	BOOL	C2S_Keep_Alive();
};

