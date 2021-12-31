#pragma once

/// <summary>
/// ���ӿ������� ���� �����͸� ������ Ÿ�Կ� ���� �����͸������ϰ� ������Ʈ�� �������ִ� �Ŵ���
/// </summary>

#include "NetworkManagerComponent.h"
#include <vector>
namespace flatbuffers
{
	class FlatBufferBuilder;
	class Vec3;
}
namespace Eater
{
	namespace PlayerData
	{
		struct Player;
	}
}

class NetwrokPlayer;
class NetworkComponent;
class ClientNetworkManager : public NetworkManagerComponent
{
public:
	ClientNetworkManager();
	~ClientNetworkManager();

	void EndUpdate();
	static void AddPlayerObject(NetwrokPlayer* obj);
private:
	///SEND
	void C2S_PLAYER_MOVE_SEND();
private:
	///RECV
	void S2C_PLAYER_MOVE_RECV(const uint8_t* Data, int type);
	void S2C_CLIENT_INFO_RECV(const uint8_t* Data, int type);
private:
	virtual void RECV(const uint8_t* Data, int type);
private:
	///��Ʈ��ũ �÷��̾� ������Ʈ ����Ʈ
	static std::vector<NetwrokPlayer*> UserObjList;

	///���糪�� Ŭ���̾�Ʈ �ѹ�
	int ClientNumber;
};

