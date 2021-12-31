#include "UnitNet.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "NetworkManager.h"
UnitNet::UnitNet()
{
	Manager = nullptr;
}

UnitNet::~UnitNet()
{

}

void UnitNet::Awake()
{
	mTransform = gameobject->GetTransform();
	mRigidbody = gameobject->GetComponent<Rigidbody>();
	
}

void UnitNet::Update()
{
	//매니저와 패킷넘버를 받은 오브젝트만 움직일 수 있도록한다
	if (Manager != nullptr)
	{
		//Manager->NETWORK_SEND(mTransform->Position.x, mTransform->Position.y, mTransform->Position.z);
	}
}

bool UnitNet::NET_PLATER_CHECK()
{
	return MyPlayer;
}

EATER_ENGINEDLL void UnitNet::NET_MOVE_POS(Vector3 Pos, Vector3 Direction, float Speed)
{
	//물리 충돌로 움직일것인지 그냥 움직일 것인지
	if (mRigidbody == nullptr)
	{
		mTransform->SetTranlate(Direction.x, Direction.y, Direction.z);
	}
	else
	{
		mRigidbody->SetVelocity(Direction.x, Direction.y, Direction.z);
	}

}

void UnitNet::PushData(Vector3 Pos)
{
	if (mRigidbody == nullptr)
	{
		mTransform->SetTranlate(Pos.x, Pos.y, Pos.z);
	}
	else
	{
		mRigidbody->SetVelocity(Pos.x, Pos.y, Pos.z);
	}
}
