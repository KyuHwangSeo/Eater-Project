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

	//움직일 수 있는 객체인지 체크
	EATER_ENGINEDLL bool NET_PLATER_CHECK();
	//객체를 움직임
	EATER_ENGINEDLL void NET_MOVE_POS(Vector3 Pos, Vector3 Direction,float Speed);

private:
	bool MyPlayer;	//내가 움직일 수 있는 객체인지 여부
private:
	friend NetworkManager;
	NetworkManager* mManger;

	Transform* mTransform;
	Rigidbody* mRigidbody;
};

