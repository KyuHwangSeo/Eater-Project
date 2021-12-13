#pragma once

namespace physx 
{
	class PxShape;
}

class PhysEngine;

class PhysX_Collider
{
public:
	PhysX_Collider();
	~PhysX_Collider();

	PhysX_Collider* CreateCollider();
protected:
	float x;
	float y;
	float z;

};

