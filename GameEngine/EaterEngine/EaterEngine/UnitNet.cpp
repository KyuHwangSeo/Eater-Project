#include "UnitNet.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
UnitNet::UnitNet()
{
	mManger = nullptr;
}

UnitNet::~UnitNet()
{

}

void UnitNet::Awake()
{
	mTransform = gameobject->GetTransform();
	mRigidbody = gameobject->GetComponent<Rigidbody>();
}

void UnitNet::SetUp()
{

}

bool UnitNet::NET_PLATER_CHECK()
{
	return MyPlayer;
}

EATER_ENGINEDLL void UnitNet::NET_MOVE_POS(Vector3 Pos, Vector3 Direction, float Speed)
{
	//�����̴� �����͸� ������ ������
	//mManger->C2S_PLAYER_MOVE_REQ(Pos,Direction,Speed);


	//�����ϼ� �ִ� ��ü���� üũ
	if (NET_PLATER_CHECK() == true)
	{
		Direction *= Speed;
		//Rigdbody�� �����ϰ����� üũ
		if (mRigidbody == nullptr)
		{
			mTransform->SetTranlate(Direction.x, Direction.y, Direction.z);
		}
		else
		{
			mRigidbody->SetVelocity(Direction.x, Direction.y, Direction.z);
		}
	}
}
