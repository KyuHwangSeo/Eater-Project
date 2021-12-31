#pragma once

/// <summary>
/// 게임엔진에서 받은 데이터를 데이터 타입에 따라 데이터를변경하고 오브젝트에 전달해주는 매니저
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
	///네트워크 플레이어 오브젝트 리스트
	static std::vector<NetwrokPlayer*> UserObjList;

	///현재나의 클라이언트 넘버
	int ClientNumber;
};

