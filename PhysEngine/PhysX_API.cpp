#include "PhysX_API.h"
#include "PhysEngine.h"

PhysEngine* Engine = nullptr;
void PhysX_Initialize(int ThreadCount, bool OnDebug)
{
	Engine = new PhysEngine();
	Engine->Initialize(ThreadCount, OnDebug);
}

void PhysX_Release()
{
	if (Engine != nullptr)
	{
		Engine->Release();
	}
}

void PhysX_Update(float m_time)
{
	if (Engine != nullptr)
	{
		Engine->Update(m_time);
	}
}

int PhysX_Create_DinamicActor(PhysData data)
{
	if (Engine != nullptr)
	{
		return Engine->Create_DinamicActor(data);
	}
}

PHYS_ENGINEDLL int PhysX_Create_StaticActor(PhysData data)
{
	if (Engine != nullptr)
	{
		return Engine->Create_StaticActor(data);
	}
}

PhysData Get_DinamicActors(int index)
{
	if (Engine != nullptr)
	{
		return Engine->GetActors(index,ACTOR_TYPE::DINAMIC);
	}
}

 PhysData Get_StaticActors(int index)
{
	if (Engine != nullptr)
	{
		return Engine->GetActors(index, ACTOR_TYPE::STATIC);
	}
}

 void PhysX_CreateScene()
{
	 
}
