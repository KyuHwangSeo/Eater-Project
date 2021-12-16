#include "PhysData.h"

PhysData::PhysData()
{
	//디폴트 값 초기화
	WorldPosition	= { 0.0f,0.0f,0.0f,0.0f };
	LocalPosition	= { 0.0f,0.0f,0.0f,0.0f };
	Rotation		= { 1.0f,1.0f,1.0f,1.0f };
	FreezePositon	= { 0.0f,0.0f,0.0f,0.0f };
	FreezeRotaticon = { 0.0f,0.0f,0.0f,0.0f };
	Velocity		= { 0.0f,0.0f,0.0f,0.0f };
	CenterPoint		= { 0.0f,0.0f,0.0f,0.0f };
	MovePoint		= { 0.0f,0.0f,0.0f,0.0f };
	Shape_Size		= { 1.0f,1.0f,1.0f,0.0f };

	MT_StaticFriction	= 0.5f;
	MT_DynamicFriction	= 0.5f;
	MT_Restitution		= 0.6f;
	MT_Mass = 1.0f;

	isDinamic	= true;
	isGrvity	= true;
	isKinematic = false;

	ActorObj = nullptr;

	Shape_type = SHAPE_TYPE::BOX;
	CreateBoxCollider(0.5f);
}

PhysData::~PhysData()
{
	ActorObj = nullptr;
}

void PhysData::SetWorldPosition(float x, float y, float z)
{
	WorldPosition.x = x;
	WorldPosition.y = y;
	WorldPosition.z = z;
	WorldPosition.w = 0.0f;
}

void PhysData::SetLocalPosition(float x, float y, float z)
{
	LocalPosition.x = x;
	LocalPosition.y = y;
	LocalPosition.z = z;
	LocalPosition.w = 0.0f;
}

void PhysData::SetTranlate(float x, float y, float z)
{
	MovePoint.x = x;
	MovePoint.y = y;
	MovePoint.z = z;
	WorldPosition.w = 0.0f;
	isMove = true;
}

void PhysData::SetRotation(float x, float y, float z)
{
	Rotation.x	= x;
	Rotation.y	= y;
	Rotation.z	= z;
	Rotation.w	= 0.0f;
}

void PhysData::SetRotate(float x, float y, float z)
{
	Rotation.x += x;
	Rotation.y += y;
	Rotation.z += z;
	Rotation.w = 0.0f;
}

void PhysData::AddForce(float x, float y, float z)
{
	Force.x = x;
	Force.y = y;
	Force.z = z;
	Force.w = 0.0f;
	isForce = true;
}

void PhysData::CreateBoxCollider(float x, float y, float z)
{
	Shape_Size = { x,y,z };
	Shape_type = SHAPE_TYPE::BOX;
}

//반지름
void PhysData::CreateBoxCollider(float Radius)
{
	Shape_Size = { Radius,Radius,Radius,Radius };
	Shape_type = SHAPE_TYPE::BOX;
}

//반지름
void PhysData::CreateSphereCollider(float Radius)
{
	Shape_Size = { Radius,0.0f,0.0f,0.0f };
	Shape_type = SHAPE_TYPE::SPHERE;
}

//반지름,높이
void PhysData::CreateCapsuleCollider(float Radius, float Height)
{
	Shape_Size = { Radius,Height,0.0f,0.0f };
	Shape_type = SHAPE_TYPE::CAPSULE;
}

void PhysData::CreateTriangleCollider(TriangleMeshData* Data)
{
	Triangle	= Data;
	Shape_type	= SHAPE_TYPE::TRIANGLE;

	//페이스로 만드는 오브젝트는 Dinamic 객체로 생성할수없다
	isDinamic = false;
}




