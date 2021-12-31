#pragma once

#include "EaterEngineDLL.h"
#include "Component.h"
#include <string>
class Transform;
class PhysData;
class Rigidbody : public Component
{
public:
	EATER_ENGINEDLL Rigidbody();
	EATER_ENGINEDLL ~Rigidbody();

	
	virtual void Start();
	virtual void SetUp();
	virtual void PhysicsUpdate();

	//타입을 정의 한다(static , Dinamic)
	EATER_ENGINEDLL void SetType(bool Dinamic);
	//중력 적용 여부
	EATER_ENGINEDLL void SetGrvity(bool grvity);
	//
	EATER_ENGINEDLL void SetKinematic(bool kinematic);
	//무게를 지정한다
	EATER_ENGINEDLL void SetMass(float mass);
	//복원력
	EATER_ENGINEDLL void SetRestitution(float mRestitution);


	//원하는 축의 방향이동을 고정 시킨다
	EATER_ENGINEDLL void SetFreezePosition(bool x,bool y,bool z);
	//원하는 축의 방향회전을 고정 시킨다 
	EATER_ENGINEDLL void SetFreezeRotation(bool x,bool y,bool z);
	//rigdbody를 이용해서 움직임
	EATER_ENGINEDLL void SetVelocity(float x, float y, float z);
	
	EATER_ENGINEDLL Vector3 GetVelocity();

	EATER_ENGINEDLL void SetAddForce(float x, float y, float z);

	EATER_ENGINEDLL void SetCenterPoint(float x, float y, float z);
		
	//콜라이더 설정
	//가로 세로 높이
	EATER_ENGINEDLL void CreateBoxCollider(float x, float y, float z);
	//반지름
	EATER_ENGINEDLL void CreateBoxCollider(float Radius);
	//반지름
	EATER_ENGINEDLL void CreateSphereCollider(float Radius);
	//반지름,높이
	EATER_ENGINEDLL void CreateCapsuleCollider(float Radius, float Height);

	EATER_ENGINEDLL void CreateTriangleCollider(std::string MeshName);


	static const int BOX		= 0;
	static const int SPHERE		= 1;
	static const int CAPSULE	= 2;
private:
	Transform* Tr;
	PhysData* RigidbodyData;
	std::string ColliderName;
};

