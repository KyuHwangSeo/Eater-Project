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
	//�Ŵ����� ��Ŷ�ѹ��� ���� ������Ʈ�� ������ �� �ֵ����Ѵ�
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
	//���� �浹�� �����ϰ����� �׳� ������ ������
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
