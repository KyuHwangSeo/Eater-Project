#include "PhysX_API.h"
#include "PhysEngine.h"

PhysEngine* Engine = nullptr;
void PhysX_Initialize(int ThreadCount, PhysSceneData* SceneData, bool OnDebug)
{
	Engine = new PhysEngine();
	Engine->Initialize(ThreadCount,SceneData,OnDebug);
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

void PhysX_Create_Actor(PhysData* data)
{
	if (Engine != nullptr)
	{
		Engine->Create_Actor(data);
	}
}

void PhysX_Update_Actor(PhysData* data)
{
	if (Engine != nullptr)
	{
		Engine->Update_Actor(data);
	}
}

void PhysX_Delete_Actor(PhysData* data)
{
	if (Engine != nullptr)
	{
		Engine->Delete_Actor(data);
	}
}

 bool PhysX_RayCast(PhysRayCast* ray)
{
	 return Engine->RayCast(ray);
}

