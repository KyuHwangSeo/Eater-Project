#pragma once

#include "PxPhysicsAPI.h"
namespace physx
{
	class PxPhysics;
	class PxShape;
	class PxSphereGeometry;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxTransform;
	struct PxCookingParams;
}

class PhysData;
struct TriangleMeshData;

class Factory
{
public:
	Factory();
	~Factory();

	void Initialize(physx::PxPhysics* Phys, physx::PxScene* Scene, physx::PxCooking* Cooking);

	///박스 콜라이더를 생성해준다
	physx::PxShape* CreateBoxCollider(physx::PxMaterial* m);
	physx::PxShape* CreateBoxCollider(physx::PxMaterial* m ,float x, float y, float z);
	physx::PxShape* CreateBoxCollider(physx::PxMaterial* m, float  Radius);

	///원 콜라이더를 생성해준다
	physx::PxShape* CreateSphereCollider(physx::PxMaterial* m, float  Radius);

	///캡슐 콜라이더를 생성한다
	physx::PxShape* CreateCapsuleCollider(physx::PxMaterial* m, float Radius, float Height);

	///버텍스의 페이스기준으로 콜라이더를 생성한다
	physx::PxShape* CreateTriangleCollider(physx::PxMaterial* m , PhysData* data);

	///콜라이더의 재질을 선택해준다
	physx::PxMaterial* CreateMaterial();
	physx::PxMaterial* CreateMaterial(float x,float y,float z);
	physx::PxMaterial* CreateMaterial(float length);

	///동적으로 행동하는 객체를 생성
	void CreateDinamicActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr);
	///정적으로 행동하는 객체를 생성
	void CreateStaticActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr);

private:
	void CreateTriangleBuffer(PhysData* data, physx::PxVec3* mVertex, physx::PxU32* mIndex);

	//회전의 축이나 이동의축을 고정시킨다
	void SetLock(physx::PxRigidDynamic* Actor, PhysData* Data);

	physx::PxPhysics* m_Phys;
	physx::PxScene* m_Scene;
	physx::PxCooking* m_Cooking;
};

