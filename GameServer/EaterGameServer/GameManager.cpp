#include "GameManager.h"
#include "DHNetWorkAPI.h"
#include "GameClientGameServerPacketDefine.h"
#include "PlayerData_generated.h"
#include "DHTimer.h"

GameManager::GameManager()
{
	// Ÿ�̸� ����
	m_Timer = new DHTimer;
	m_Timer->SetFrame(60);

	// �κ� ������ ���(Ŭ���̾�Ʈ ����)
	//m_Lobby_Client = new DHNetWorkAPI();
	//m_Lobby_Client->Initialize(DHNetWork_Name::DHNet);
	//m_Lobby_Client->Connect(729, "127.0.0.1");

	// ����Ŭ���̾�Ʈ�� �ޱ� ���� ���Ӽ��� ����
	m_Game_Server = new DHNetWorkAPI();
	m_Game_Server->Initialize(DHNetWork_Name::DHNet, DHDEBUG_SIMPLE);
	m_Game_Server->Accept(729, 4, 4);

	g_Packet_Send_Thread = new std::thread(std::bind(&GameManager::S2C_Loop, this));
}

GameManager::~GameManager()
{

}

void GameManager::S2C_Loop()
{
	// ���Ӽ����� ��� ����ǹǷ� �� ������ ��� ����Ǿ�� �Ѵ�.
	while (true)
	{
		while (!Is_Game_End)
		{
			// 60 ������
			if (m_Timer->Ready_Frame())
			{
				// ����������.
				S2C_Player_Move();
			}

			Sleep(0);
		}

		// ���� ���Ӽ����� ����Ǿ� �÷������� Ŭ���̾�Ʈ�� �������� �ʴ´ٸ�..
		Sleep(0);
	}
}

void GameManager::GameLogic()
{
	while (!Is_Game_End)
	{
		// �ֱ������� ���� ��Ŷ�� ������ Ȯ���Ѵ�.
		if (m_Game_Server->Recv(Msg_Vec))
		{
			// Ŭ���̾�Ʈ�κ��� ���� �����͵�� ������ ó��..
			for (auto Msg_Packet : Msg_Vec)
			{
				// Ŭ���̾�Ʈ�� ���� ��ȣ
				SOCKET _Recv_Socket_Num = Msg_Packet.Socket;
				// Ŭ���̾�Ʈ�� ���� �޼���
				C2S_Packet* C2S_Msg = static_cast<C2S_Packet*>(Msg_Packet.Packet);

				// �÷��̾� �̵��� ���� ó��..
				if (C2S_Msg->Packet_Type == C2S_PLAYER_MOVE_REQ)
				{
					if (C2S_Player_Move() == false)
					{
						// ��������..
					}
				}

				// ������ ����ִ��� �˻��ϴ� ��Ŷ.
				if (C2S_Msg->Packet_Type == C2S_KEEP_ALIVE_CHECK_REQ)
				{
					if (C2S_Keep_Alive() == false)
					{
						// ��������..
					}
				}

				// ����ѵ����� ����.
				delete Msg_Packet.Packet;
				C2S_Msg = nullptr;
			}
		}

		Sleep(0);
	}
}

void GameManager::S2C_Player_Move()
{
	// FlatBuffer�� ����ϱ� ���� ����
	flatbuffers::FlatBufferBuilder _Builder;
	// Ŭ���̾�Ʈ�� ������ �޼���.
	_S2C_Msg = new S2C_Packet;

	// �÷��̾� ������ ���� ����..
	auto Name = _Builder.CreateString("Player01");
	auto Position = Eater::PlayerData::Vec3(0.1f, 0.f, 0.f);
	auto Mov_Vec = Eater::PlayerData::Vec3(0.5f, 0.f, 0.f);
	UINT32 HP = 100;
	UINT32 Mana = 50;

	// ������ ����ȭ �Ϸ�.
	_Builder.Finish(CreatePlayer(_Builder, Name, &Position, &Mov_Vec, HP, Mana));

	_S2C_Msg->Packet_Type = S2C_PLAYER_MOVE_RES;
	_S2C_Msg->Packet_Size = _Builder.GetSize();
	memcpy_s(_S2C_Msg->Packet_Buffer, _Builder.GetSize(), _Builder.GetBufferPointer(), _Builder.GetSize());

	// BroadCast
	m_Game_Server->Send(_S2C_Msg);

	_Builder.Clear();
}

BOOL GameManager::C2S_Player_Move()
{

	return true;
}

BOOL GameManager::C2S_Keep_Alive()
{

	return true;
}
