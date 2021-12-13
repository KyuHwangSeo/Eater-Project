#pragma once

namespace physx
{
	class PxPhysics;
	class PxShape;
	class PxSphereGeometry;
}

#include "PxPhysicsAPI.h"

class Factory
{
public:
	Factory();
	~Factory();

	void Initialize(physx::PxPhysics* Phys);

	physx::PxShape* CreateBoxCollider(physx::PxMaterial* m);
	physx::PxShape* CreateBoxCollider(physx::PxMaterial* m ,float x, float y, float z);
	physx::PxShape* CreateBoxCollider(physx::PxMaterial* m, float  length);

	physx::PxMaterial* CreateMaterial();
	physx::PxMaterial* CreateMaterial(float x,float y,float z);
	physx::PxMaterial* CreateMaterial(float length);


		

	physx::PxShape* CreateSphereCollider();


	void CreateRigidbody();
private:
	physx::PxPhysics* m_Phys;
};

