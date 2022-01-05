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
	class PxCooking;
	class PxPvd;
	class PxTolerancesScale;
}

class PhysData;
class PhysRayCast;
class RaycastManager;
class Factory;
struct PhysSceneData;

class PhysEngine
{
public:
	PhysEngine();
	~PhysEngine();

	//�ʱ�ȭ
	bool Initialize(int ThreadCount, PhysSceneData* SceneData, bool Debug);
	//�� ����
	bool CreateScene(PhysSceneData* SceneData);
	//��ü ����
	void Release();
	//�� ������Ʈ
	void Update(float m_time);

	//���� ����
	void Create_Actor(PhysData* data);
	//���� ������Ʈ
	void Update_Actor(PhysData* data);
	//���� ����
	void Delete_Actor(PhysData* data);
	
	//�Ѱ��� Raycast�浹 ���θ� ��ȯ
	bool RayCast(PhysRayCast* ray);
	
	bool MultiRayCast(PhysRayCast* ray);
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
	physx::PxPvd*					m_Pvd;				//������� ����ϱ�����

	bool OnDebug = false;	//����� ��뿩��

	Factory*		m_Factory;
	RaycastManager* m_RayManager;
};