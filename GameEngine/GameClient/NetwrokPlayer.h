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
	
	int PlayerNumber;	//�������� �������ִ� ��ü ��ȣ
	int ClientNumber;	//���� ���° Ŭ���̾�Ʈ�� �÷����ϴ���

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
	void KeyInputMove();		//������ ���� Ű�Է����� ������
	void KeyInputMoveTest();	//�׽�Ʈ 2p ��
	void RemoteMove();			//��Ʈ��ũ ������� ������
	void DataSetting();			//������ ����
private:	
	float Keyinput_Right;		//������ ���ʰ� 
	float keyinput_Up;			//���Ʒ� ��
	float Keyinput_Angle;		//ȸ����
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

