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

	///�ڽ� �ݶ��̴��� �������ش�
	physx::PxShape* CreateBoxCollider(physx::PxMaterial* m);
	physx::PxShape* CreateBoxCollider(physx::PxMaterial* m ,float x, float y, float z);
	physx::PxShape* CreateBoxCollider(physx::PxMaterial* m, float  Radius);

	///�� �ݶ��̴��� �������ش�
	physx::PxShape* CreateSphereCollider(physx::PxMaterial* m, float  Radius);

	///ĸ�� �ݶ��̴��� �����Ѵ�
	physx::PxShape* CreateCapsuleCollider(physx::PxMaterial* m, float Radius, float Height);

	///���ؽ��� ���̽��������� �ݶ��̴��� �����Ѵ�
	physx::PxShape* CreateTriangleCollider(physx::PxMaterial* m , PhysData* data);

	///�ݶ��̴��� ������ �������ش�
	physx::PxMaterial* CreateMaterial();
	physx::PxMaterial* CreateMaterial(float x,float y,float z);
	physx::PxMaterial* CreateMaterial(float length);

	///�������� �ൿ�ϴ� ��ü�� ����
	void CreateDinamicActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr);
	///�������� �ൿ�ϴ� ��ü�� ����
	void CreateStaticActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr);

private:
	void CreateTriangleBuffer(PhysData* data, physx::PxVec3* mVertex, physx::PxU32* mIndex);

	//ȸ���� ���̳� �̵������� ������Ų��
	void SetLock(physx::PxRigidDynamic* Actor, PhysData* Data);

	physx::PxPhysics* m_Phys;
	physx::PxScene* m_Scene;
	physx::PxCooking* m_Cooking;
};

