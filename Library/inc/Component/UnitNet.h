#pragma once

#include "EaterEngineDLL.h"

#include "Component.h"
class NetworkManager;
class Rigidbody;
class Transform;
class UnitNet :public Component
{
public:
	EATER_ENGINEDLL UnitNet();
	~UnitNet();

	virtual void Awake();
	virtual void SetUp();

	//������ �� �ִ� ��ü���� üũ
	EATER_ENGINEDLL bool NET_PLATER_CHECK();
	//��ü�� ������
	EATER_ENGINEDLL void NET_MOVE_POS(Vector3 Pos, Vector3 Direction,float Speed);

private:
	bool MyPlayer;	//���� ������ �� �ִ� ��ü���� ����
private:
	friend NetworkManager;
	NetworkManager* mManger;

	Transform* mTransform;
	Rigidbody* mRigidbody;
};

