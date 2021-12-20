#include "Transform.h"
#include "MainHeader.h"
#include "AI.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "MeshFilter.h"
#include "AnimationController.h"
#include "PhysData.h"
#include "UnitNet.h"
#include "Camera.h"


AI::AI()
{
	mTransform				= nullptr;
	mRigidbody				= nullptr;
	mMeshFilter				= nullptr;
	mAnimationController	= nullptr;
	mNetwork					= nullptr;

	isJump = false;
}

AI::~AI()
{

}

void AI::Awake()
{
	mTransform	= gameobject->GetComponent<Transform>();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mRigidbody	= gameobject->GetComponent<Rigidbody>();
	//mNetwork		= gameobject->GetComponent<UnitNet>();
	mRay		= new PhysRayCast();
}

void AI::SetUp()
{
	mTransform->Rotation	= { 90,0,0 };
	mTransform->Scale		= { 1,1,1 };

	mMeshFilter->SetMeshName("Sphere");
	mRigidbody->CreateSphereCollider(1);

	//mRigidbody->SetFreezePosition(true, true, true);
	mRigidbody->SetRestitution(0);
	mRigidbody->SetFreezeRotation(true, true, true);
}

void AI::Update()
{
	Move();

	Vector3 Pos = mTransform->Position;
	Vector3 Direction = Vector3(Right, 0, Up);
	
	//mNetwork->NET_MOVE_POS(Pos,Direction,Speed);
	mRigidbody->SetVelocity(Right, 0, Up);
}

void AI::GetCamera(GameObject* Cam)
{
	//if (mNetwork->NET_PLATER_CHECK() == true)
	//{
	//	mCam = Cam->GetComponent<Camera>();
	//}
}

void AI::Move()
{
	Speed = 10;
	if (GetKey(VK_UP))
	{
		Up = Speed;
	}
	else if (GetKey(VK_DOWN))
	{
		Up = -Speed;
	}
	else
	{
		Up = 0;
	}


	if (GetKey(VK_RIGHT))
	{
		Right = Speed;
	}
	else if (GetKey(VK_LEFT))
	{
		Right = -Speed;
	}
	else
	{
		Right = 0;
	}
}


