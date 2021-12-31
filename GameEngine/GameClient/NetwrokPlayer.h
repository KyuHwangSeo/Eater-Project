#pragma once

#include "NetworkComponent.h"

namespace Eater
{
	namespace PlayerData
	{
		struct Player;
	}
}

class Transform;
class Rigidbody;
class MeshFilter;
class AnimationController;

struct NetworkData;
struct PlayerData;

class NetwrokPlayer : public NetworkComponent
{
public:
	NetwrokPlayer();
	~NetwrokPlayer();

	virtual void Awake();
	virtual void SetUp();
	virtual void Update();

	virtual void RECV(void* Data,int type);
	
	int PlayerNumber;	//서버에서 지정해주는 객체 번호
	int ClientNumber;	//현재 몇번째 클라이언트가 플레이하는지

	int	 AnimeType;
	float PositionX;
	float PositionY;
	float PositionZ;

	float DirectionX;
	float DirectionY;
	float DirectionZ;
	float Speed;
private:
	void S2C_PLAYER_MOVE_RECV(void* mData);
	void S2C_PLAYER_INFO_RECV(void* mData);
private:
	void KeyInputMove();		//유저가 직접 키입력으로 움직임
	void KeyInputMoveTest();	//테스트 2p 용
	void RemoteMove();			//네트워크 통신으로 움직임
	void DataSetting();			//데이터 셋팅
private:	
	float Keyinput_Right;		//오른쪽 왼쪽값 
	float keyinput_Up;			//위아래 값
	float Keyinput_Angle;		//회전값
private:
	Transform*				mTransform;
	Rigidbody*				mRigidbody;
	MeshFilter*				mMeshFilter;
	AnimationController*	mAnimation;
	GameObject*				mObj;
private:
	bool NET_MOVE_PASS;
	bool ENT_MY_PLAYER;


};

