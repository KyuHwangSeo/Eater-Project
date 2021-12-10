#include "PhysEngine.h"
#include "PxPhysicsAPI.h"
#include <ctype.h>

#pragma comment(lib,"PhysX_64.lib")
#pragma comment(lib,"PhysXFoundation_64.lib")
#pragma comment(lib,"PhysXExtensions_static_64.lib")
//#pragma comment(lib,"PhysXVehicle_static_64.lib") //차량
//#pragma comment(lib,"PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib,"PhysXCooking_64.lib")


//PhysXExtensions_static_64.lib
//e\PhysX_64.lib
//e\PhysXPvdSDK_static_64.lib
//e\PhysXVehicle_static_64.lib
//e\PhysXCharacterKinematic_static_64.lib
//e\PhysXCooking_64.lib
//e\PhysXCommon_64.lib
//e\SnippetUtils_static_64.lib
//e\SnippetRender_static_64.lib
//e\PhysXFoundation_64.lib

using namespace physx;
PhysEngine::PhysEngine()
{
	m_Foundation	= nullptr;
	m_Physics		= nullptr;
	m_Dispatcher	= nullptr;
	m_Scene			= nullptr;
	m_Material		= nullptr;
}

PhysEngine::~PhysEngine()
{

}

bool PhysEngine::Initialize()
{
	/// <summary>
	/// https://gameworksdocs.nvidia.com/PhysX/4.0/documentation/PhysXGuide/Manual/Startup.html
	/// </summary>
	//physx::PxDefaultAllocator		m_Allocator;
	//physx::PxDefaultErrorCallback	m_ErrorCallback;
	m_Allocator = new PxDefaultAllocator();
	m_ErrorCallback = new physx::PxDefaultErrorCallback();

	//PxFoundation 는 프로세스 당 1개 생성
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_Allocator, *m_ErrorCallback);
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), false);
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);

	if (m_Foundation == nullptr){return false;}
	if (m_Physics == nullptr){return false;}
	if (m_Dispatcher == nullptr){return false;}

	//스크린 생성
	CreateScene();

	//CreateRigidbody();
	return true;
}

void PhysEngine::Release()
{
	//생성의 역순으로 삭제
	m_Dispatcher->release();
	m_Physics->release();
	m_Foundation->release();
}

void PhysEngine::CreateRigidbody()
{
	PxTransform transform = PxTransform(PxVec3(0,0,0), PxQuat(0,0,0,0));
	

	PxMaterial* pMaterial = m_Physics->createMaterial(0.5f, 0.5f, 0.5f);


	//충돌체 모양을 설정해준다
	// 1. Geometry로 반지름인1인 구 모양을 만든다
	// 2. 충돌재질
	// 3. Actor가 오브젝트를 독점할것인가?
	PxShape* pShape = m_Physics->createShape(PxSphereGeometry(1.0f),*pMaterial);

	pShape->setFlag(PxShapeFlag::eVISUALIZATION, true);
	pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true); //ray , sweep 등의 충돌을 검출할때 사용

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); 
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);	//Trigger로 사용할것인지 선택 


	//생성한다
	PxRigidDynamic* pActor = m_Physics->createRigidDynamic(transform);
	pActor->attachShape(*pShape);
	pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	
	pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false); //중력을 받도록 설정한다
	////위에서 생성한 pShape를 붙여준다
	////Scene에 추가한다
	m_Scene->addActor(*pActor);
}

bool PhysEngine::CreateScene()
{
	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());

	//중력 설정
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = m_Dispatcher;

	//시뮬레이션 중에 충돌 이벤트 함수가 정의된 클래스가들어갈곳
	//sceneDesc.simulationEventCallback = NULL;

	//시뮬레이션 중에 사용할 필터 함수(현재는 디폴트)
	sceneDesc.filterShader		= PxDefaultSimulationFilterShader;
	sceneDesc.broadPhaseType	= PxBroadPhaseType::eMBP;

	//스크린 생성
	m_Scene = m_Physics->createScene(sceneDesc);

	//바닥생성
	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
	PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material);
	m_Scene->addActor(*groundPlane);

	return false;
}


