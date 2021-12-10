#pragma once
#include "PhysEngineDLL.h"

namespace physx
{
	class PxDefaultAllocator;
	class PxDefaultErrorCallback;
	class PxFoundation;
	class PxPhysics;
	class PxDefaultCpuDispatcher;
	class PxScene;
	class PxMaterial;
	class PxRigidDynamic;
	class PxRigidStatic;
}


class PhysEngine
{
public:
	PHYS_ENGINEDLL PhysEngine();
	PHYS_ENGINEDLL ~PhysEngine();

	
	PHYS_ENGINEDLL bool Initialize();	//초기화
	PHYS_ENGINEDLL void Release();		//삭제

	

private:
	//실질적인 물리연산될 공간을 생성한다
	void CreateRigidbody();//물리 충돌하는 객체를 생성
	bool CreateScene();

	physx::PxDefaultAllocator*		m_Allocator;
	physx::PxDefaultErrorCallback*	m_ErrorCallback;

	physx::PxFoundation*			m_Foundation;
	physx::PxPhysics*				m_Physics;
	physx::PxDefaultCpuDispatcher*	m_Dispatcher;
	physx::PxScene*					m_Scene;
	physx::PxMaterial*				m_Material;

	//PxRigidStatic* groundPlane;

	//버전 디버깅용
	//physx::PxPvd*					gPvd = NULL;

	//PxRigidDynamic* CreateBox();
};