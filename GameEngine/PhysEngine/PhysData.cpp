#include "PhysData.h"

PhysData::PhysData()
{
	//디폴트 값 초기화
	WorldPosition	= { 0.0f,0.0f,0.0f};
	LocalPosition	= { 0.0f,0.0f,0.0f};
	Rotation		= { 1.0f,1.0f,1.0f,1.0f };
	FreezePositon	= { 0.0f,0.0f,0.0f};
	FreezeRotaticon = { 0.0f,0.0f,0.0f};
	Velocity		= { 0.0f,0.0f,0.0f};
	CenterPoint		= { 0.0f,0.0f,0.0f};
	Shape_Size		= { 1.0f,1.0f,1.0f};
	PhysX_Velocity  = { 0.0f,0.0f,0.0f};

	MT_StaticFriction	= 0.5f;
	MT_DynamicFriction	= 0.5f;
	MT_Restitution		= 0.6f;
	MT_Mass = 1.0f;

	isDinamic	= true;
	isGrvity	= true;
	isKinematic = false;
	isVelocity	= false;
	isForce		= false;

	ActorObj	= nullptr;

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
}

void PhysData::SetLocalPosition(float x, float y, float z)
{
	LocalPosition.x = x;
	LocalPosition.y = y;
	LocalPosition.z = z;
}

void PhysData::SetRotation(float x, float y, float z)
{
	Rotation.x	= x;
	Rotation.y	= y;
	Rotation.z	= z;
}

void PhysData::SetRotate(float x, float y, float z)
{
	Rotation.x += x;
	Rotation.y += y;
	Rotation.z += z;
}

void PhysData::AddForce(float x, float y, float z)
{
	Force.x = x;
	Force.y = y;
	Force.z = z;
	isForce = true;
}

void PhysData::SetVelocity(float x, float y, float z)
{
	Velocity.x = x;
	Velocity.y = y;
	Velocity.z = z;
	isVelocity = true;
}

Vector3 PhysData::GetVelocity()
{
	return PhysX_Velocity;
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

void PhysData::CreateTriangleCollider(TriangleMeshData* mTriangle, int VertexSize, int IndexSize)
{
	Triangle = mTriangle;
	Shape_type	= SHAPE_TYPE::TRIANGLE;

	//페이스로 만드는 오브젝트는 Dinamic 객체로 생성할수없다
	isDinamic = false;
}

void PhysData::SetLockAxis_Position(bool x, bool y, bool z)
{
	FreezePositon.x = x;
	FreezePositon.y = y;
	FreezePositon.z = z;
}

void PhysData::SetLockAxis_Rotation(bool x, bool y, bool z)
{
	FreezeRotaticon.x = x;
	FreezeRotaticon.y = y;
	FreezeRotaticon.z = z;
}

PhysRayCast::PhysRayCast()
{


}

PhysRayCast::~PhysRayCast()
{


}
