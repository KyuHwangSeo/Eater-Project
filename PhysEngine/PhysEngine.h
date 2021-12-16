#pragma once
#include "PhysEngineDLL.h"
#include <vector>

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
	class PxCooking;
	class PxPvd;
	class PxTolerancesScale;
	class PxTransform;
	class PxRigidActor;
}

class Factory;
struct PhysSceneData;

class PhysEngine
{
public:
	PhysEngine();
	~PhysEngine();

	//초기화
	bool Initialize(int ThreadCount, PhysSceneData* SceneData, bool Debug);
	//씬 생성
	bool CreateScene(PhysSceneData* SceneData);
	//전체 삭제
	void Release();
	//씬 업데이트
	void Update(float m_time);

	//엑터 생성
	void Create_Actor(PhysData* data);
	//엑터 업데이트
	void Update_Actor(PhysData* data);
	//엑터 삭제
	void Delete_Actor(PhysData* data);

private:
	bool Initialize_Release(int ThreadCount);
	bool Initialize_Debug(int ThreadCount);
	void CreateStart();

	bool Start = false;


	physx::PxDefaultAllocator*		m_Allocator;
	physx::PxDefaultErrorCallback*	m_ErrorCallback;
	physx::PxTolerancesScale*		m_TolerancesScale;

	physx::PxCooking*				m_Cooking;
	physx::PxFoundation*			m_Foundation;
	physx::PxPhysics*				m_Physics;
	physx::PxDefaultCpuDispatcher*	m_Dispatcher;
	physx::PxScene*					m_Scene;
	physx::PxMaterial*				m_Material;
	physx::PxPvd*					m_Pvd;				//디버깅을 사용하기위해

	bool OnDebug = false;	//디버깅 사용여부

	Factory* m_Factory;
};