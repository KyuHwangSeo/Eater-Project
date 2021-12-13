#include "Factory.h"
#include "PxPhysicsAPI.h"

using namespace physx;
Factory::Factory()
{
	m_Phys = nullptr;

}

Factory::~Factory()
{


}

void Factory::Initialize(physx::PxPhysics* Phys)
{
	m_Phys = Phys;
}

PxShape* Factory::CreateBoxCollider(PxMaterial* m)
{
	PxBoxGeometry temp = PxBoxGeometry();
	PxShape* shape = m_Phys->createShape(temp, *m);
	return shape;
}

PxShape* Factory::CreateBoxCollider(PxMaterial* m,float x,float y,float z)
{
	PxBoxGeometry temp = PxBoxGeometry(PxReal(x), PxReal(y), PxReal(z));
	PxShape* shape = m_Phys->createShape(temp, *m,true);
	return shape;
}

PxShape* Factory::CreateBoxCollider(PxMaterial* m,float length)
{
	PxBoxGeometry temp = PxBoxGeometry(PxReal(length), PxReal(length), PxReal(length) );
	PxShape* shape = m_Phys->createShape(temp, *m);
	return shape;
}


PxMaterial* Factory::CreateMaterial()
{
	return m_Phys->createMaterial(0, 0, 0);
}

PxMaterial* Factory::CreateMaterial(float x, float y, float z)
{
	return m_Phys->createMaterial(x, y, z);
}

PxMaterial* Factory::CreateMaterial(float length)
{
	return m_Phys->createMaterial(length, length, length);
}



void Factory::CreateRigidbody()
{


}

