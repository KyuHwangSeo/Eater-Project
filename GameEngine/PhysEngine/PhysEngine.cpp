#include "PhysEngine.h"
#include "PxPhysicsAPI.h"
#include <vector>
#include <ctype.h>
#include "PhysData.h"
#include "Factory.h"
#include "RaycastManager.h"


#pragma comment(lib,"PhysX_64.lib")
#pragma comment(lib,"PhysXFoundation_64.lib")
#pragma comment(lib,"PhysXExtensions_static_64.lib")
#pragma comment(lib,"PhysXCooking_64.lib")
#pragma comment(lib,"PhysXCommon_64.lib")
#pragma comment(lib,"PhysXPvdSDK_static_64.lib")


//#pragma comment(lib,"PhysXVehicle_static_64.lib")				//차량
//#pragma comment(lib,"PhysXCharacterKinematic_static_64.lib")	//캐릭터



using namespace physx;
PhysEngine::PhysEngine()
{
	m_Allocator			= nullptr;
	m_ErrorCallback		= nullptr;
	m_TolerancesScale	= nullptr;

	m_Foundation	= nullptr;
	m_Physics		= nullptr;
	m_Dispatcher	= nullptr;
	m_Scene			= nullptr;
	m_Pvd			= nullptr;
	m_Cooking		= nullptr;

	m_Factory		= nullptr;
	m_RayManager	= nullptr;
}

PhysEngine::~PhysEngine()
{

}

bool PhysEngine::Initialize(int ThreadCount, PhysSceneData* SceneData, bool Debug)
{
	/// <summary>
	/// https://gameworksdocs.nvidia.com/PhysX/4.0/documentation/PhysXGuide/Manual/Startup.html
	/// </summary>
	
	OnDebug = Debug;
	if (Debug == true)
	{
		Initialize_Debug(ThreadCount);
	}
	else
	{
		Initialize_Release(ThreadCount);
	}

	CreateScene(SceneData);
	
	//펙토리 생성
	m_Factory = new Factory();
	m_Factory->Initialize(m_Physics,m_Scene, m_Cooking);

	m_RayManager = new RaycastManager();
	m_RayManager->Initialize(m_Scene);

	return true;
}

void PhysEngine::Release()
{
	//생성의 역순으로 삭제
	m_Dispatcher->release();
	m_Physics->release();
	m_Foundation->release();

	delete m_Allocator;
	delete m_ErrorCallback;
	delete m_TolerancesScale;

	//디버깅일때 삭제
	if (m_Pvd != nullptr)
	{
		PxPvdTransport* transport = m_Pvd->getTransport();
		m_Pvd->release();	m_Pvd = NULL;
		transport->release();
	}
}

void PhysEngine::Update(float m_time)
{
	if (Start == false)
	{
		m_Scene->simulate(0);
		m_Scene->fetchResults(true);
		Start = true;
	}
	else
	{
		m_Scene->simulate(m_time);
		m_Scene->fetchResults(true);
	}
}

void PhysEngine::Create_Actor(PhysData* data)
{
	float Pi = 3.141592f;

	///위치,회전을 지정해준다
	PxTransform P = PxTransform(data->WorldPosition.x, data->WorldPosition.y, data->WorldPosition.z);
	
	PxTransform Rx = PxTransform(PxQuat((data->Rotation.x)* Pi / 180, PxVec3(1, 0, 0)));
	PxTransform Ry = PxTransform(PxQuat(data->Rotation.y * Pi / 180, PxVec3(0, 1, 0)));
	PxTransform Rz = PxTransform(PxQuat(data->Rotation.z * Pi / 180, PxVec3(0, 0, 1)));
	
	PxTransform  TR = PxTransform(P * (Rz * Ry* Rx));

	///재질을 지정해준다
	PxMaterial* pMaterial	= m_Factory->CreateMaterial(data->MT_StaticFriction, data->MT_DynamicFriction, data->MT_Restitution);
	
	///모양을 지정해준다
	PxShape* shape = nullptr;
	switch (data->Shape_type)
	{
	case SHAPE_TYPE::BOX:
		shape = m_Factory->CreateBoxCollider(pMaterial, data->Shape_Size.x, data->Shape_Size.y, data->Shape_Size.z);
		break;
	case SHAPE_TYPE::SPHERE:
		shape = m_Factory->CreateSphereCollider(pMaterial, data->Shape_Size.x);
		break;
	case SHAPE_TYPE::CAPSULE:
		shape = m_Factory->CreateCapsuleCollider(pMaterial, data->Shape_Size.x, data->Shape_Size.y);
		break;
	case SHAPE_TYPE::TRIANGLE:
		shape = m_Factory->CreateTriangleCollider(pMaterial,data);
		break;
	}

	shape->setLocalPose(PxTransform(data->CenterPoint.x, data->CenterPoint.y, data->CenterPoint.z));

	///물리 객체 생성
	if (data->isDinamic == true)
	{
		m_Factory->CreateDinamicActor(data, shape,&TR);
	}

	if(data->isDinamic == false)
	{
		m_Factory->CreateStaticActor(data, shape,&TR);
	}

	if (shape != nullptr)
	{
		shape->release();
	}
}

void  PhysEngine::Update_Actor(PhysData* data)
{
	//받아온 Data를 Actor와 동기화시켜준다
	PxRigidActor* rig = reinterpret_cast<PxRigidActor*>(data->ActorObj);
	PxRigidBody* body = reinterpret_cast<PxRigidBody*>(data->ActorObj);
	PxTransform Tr = rig->getGlobalPose();


	//관성이 들어간 힘을 준다
	if (data->isForce == true)
	{
		PxVec3 Force	= PxVec3(data->Force.x, data->Force.y, data->Force.z);
		PxVec3 Pos		= PxVec3(data->WorldPosition.x, data->WorldPosition.y, data->WorldPosition.z);
		PxRigidBodyExt::addForceAtPos(*body, Force, Pos);
		data->isForce = false;
	}

	
	if (data->isVelocity == true)
	{
		PxVec3 pos = body->getLinearVelocity();
		PxVec3 Pox = PxVec3(data->Velocity.x,pos.y, data->Velocity.z);
		body->setLinearVelocity(Pox);
		data->isVelocity = false;
	}


	data->WorldPosition.x = Tr.p.x;
	data->WorldPosition.y = Tr.p.y;
	data->WorldPosition.z = Tr.p.z;

	data->Rotation.x = Tr.q.x;
	data->Rotation.y = Tr.q.y;
	data->Rotation.z = Tr.q.z;
	data->Rotation.w = Tr.q.w;
}

void PhysEngine::Delete_Actor(PhysData* data)
{
	//한개의 엑터 삭제
	PxRigidActor* rig = reinterpret_cast<PxRigidActor*>(data->ActorObj);
	rig->release();
}

bool PhysEngine::RayCast(PhysRayCast* ray)
{
	return m_RayManager->RayCast(ray);
}

bool PhysEngine::MultiRayCast(PhysRayCast* ray)
{
	return m_RayManager->RayCastMultiple(ray);
}



bool PhysEngine::CreateScene(PhysSceneData* SceneData)
{
	PxSceneDesc sceneDesc = PxSceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.broadPhaseType	= PxBroadPhaseType::eMBP;
	m_Scene = m_Physics->createScene(sceneDesc);


	if (OnDebug == true)
	{
		PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	//충돌될 범위를 지정해준다
	PxBroadPhaseRegion regions[4] =
	{
		{	PxBounds3(PxVec3(-100, -100, -100),  PxVec3(0, 100,   0)), reinterpret_cast<void*>(1) },
		{	PxBounds3(PxVec3(-100, -100,    0),  PxVec3(0, 100, 100)), reinterpret_cast<void*>(2) },
		{	PxBounds3(PxVec3(0, -100, -100),  PxVec3(100, 100,   0)), reinterpret_cast<void*>(3) },
		{	PxBounds3(PxVec3(0, -100,    0),  PxVec3(100, 100, 100)), reinterpret_cast<void*>(4) }
	};

	for (PxU32 i = 0; i < 4; i++)
	{
		m_Scene->addBroadPhaseRegion(regions[i]);
	}

	//바닥생성
	//PxMaterial* m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
	//PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material);
	//m_Scene->addActor(*groundPlane);

	return false;
}

bool PhysEngine::Initialize_Release(int ThreadCount)
{
	CreateStart();

	m_Foundation	= PxCreateFoundation(PX_PHYSICS_VERSION, *m_Allocator, *m_ErrorCallback);
	m_Physics		= PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, *m_TolerancesScale);
	m_Dispatcher	= PxDefaultCpuDispatcherCreate(ThreadCount);
	m_Cooking		= PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale());

	if (m_Foundation == nullptr) { return false; }
	if (m_Physics == nullptr) { return false; }
	if (m_Dispatcher == nullptr) { return false; }

	return true;
}

bool PhysEngine::Initialize_Debug(int ThreadCount)
{
	CreateStart();

	m_Foundation= PxCreateFoundation(PX_PHYSICS_VERSION, *m_Allocator, *m_ErrorCallback);

	//PhysX visual Debugger 와 연결 준비를 한다
	m_Pvd		= PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	//physX 생성
	m_Physics	= PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_Pvd);
	PxInitExtensions(*m_Physics, m_Pvd);

	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale());



	//쓰레드 개수만큼 Phys 를 돌림
	m_Dispatcher = PxDefaultCpuDispatcherCreate(ThreadCount);

	//생성여부 체크
	if (m_Foundation == nullptr) { return false; }
	if (m_Physics == nullptr) { return false; }
	if (m_Dispatcher == nullptr) { return false; }

	return true;
}

void PhysEngine::CreateStart()
{
	m_Allocator			= new PxDefaultAllocator();
	m_ErrorCallback		= new PxDefaultErrorCallback();
	m_TolerancesScale	= new PxTolerancesScale();
}




