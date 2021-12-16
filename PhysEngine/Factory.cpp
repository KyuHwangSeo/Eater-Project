#include "Factory.h"
#include "PxPhysicsAPI.h"
#include "PhysData.h"


using namespace physx;
Factory::Factory()
{
	m_Phys = nullptr;
	m_Scene = nullptr;
}

Factory::~Factory()
{
	m_Phys = nullptr;
	m_Scene = nullptr;
}

void Factory::Initialize(physx::PxPhysics* Phys, physx::PxScene* Scene, physx::PxCooking* Cooking)
{
	m_Phys		= Phys;
	m_Scene		= Scene;
	m_Cooking	= Cooking;
}

PxShape* Factory::CreateBoxCollider(PxMaterial* m)
{
	PxBoxGeometry temp = PxBoxGeometry();
	PxShape* shape = m_Phys->createShape(temp, *m);
	return shape;
}

PxShape* Factory::CreateBoxCollider( PxMaterial* m,float x,float y,float z)
{
	PxBoxGeometry temp = PxBoxGeometry(PxReal(x), PxReal(y), PxReal(z));
	PxShape* shape = m_Phys->createShape(temp, *m);
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

physx::PxShape* Factory::CreateSphereCollider(physx::PxMaterial* m, float length)
{
	PxSphereGeometry temp = PxSphereGeometry(PxReal(length));
	PxShape* shape = m_Phys->createShape(temp, *m);
	return shape;
}

physx::PxShape* Factory::CreateCapsuleCollider(physx::PxMaterial* m, float Radius, float Height)
{
	physx::PxTransform T = physx::PxTransformFromSegment(physx::PxVec3(1, 1, 1), physx::PxVec3(0, 1, 0));

	PxCapsuleGeometry temp = PxCapsuleGeometry(PxReal(Radius), PxReal(Height));
	PxShape* shape = m_Phys->createShape(temp, *m);
	
	return shape;
}

void Factory::CreateDinamicActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr)
{
	//Tr->rotate(physx::PxVec3(45, 45, 45));
	//Tr->transform(PxVec3(10, 0, 0));

	PxRigidDynamic* body = m_Phys->createRigidDynamic(*Tr);

	shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	shape->setFlag(PxShapeFlag::eVISUALIZATION,true); //Ray, sweep등 할때 사용됨
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE,false);

	body->attachShape(*shape);

	physx::PxVec3 temp = physx::PxVec3(Data->CenterPoint.x, Data->CenterPoint.y, Data->CenterPoint.z);

	PxRigidBodyExt::updateMassAndInertia(*body, Data->MT_Mass,&temp);
	m_Scene->addActor(*body);

	//서로 연결
	body->userData = Data;
	Data->ActorObj = body;

	body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC,Data->isKinematic);
	body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY ,!Data->isGrvity);
	int num = 0;
}

void Factory::CreateStaticActor(PhysData* Data, physx::PxShape* shape, physx::PxTransform* Tr)
{
	PxRigidStatic* body = m_Phys->createRigidStatic(*Tr);
	body->attachShape(*shape);
	m_Scene->addActor(*body);

	//서로 연결
	body->userData = Data;
	Data->ActorObj = body;
}
void Factory::CreateTriangleBuffer(TriangleMeshData* TriangleData, PxVec3* mVertex, PxU32* mIndex)
{
	int VertexCount = TriangleData->VertexList->size();
	int IndexCount = TriangleData->IndexList->size();

	for (int i = 0; i < VertexCount; i++)
	{
		mVertex[i].x = (*TriangleData->VertexList)[i].x;
		mVertex[i].y = (*TriangleData->VertexList)[i].y;
		mVertex[i].z = (*TriangleData->VertexList)[i].z;
	}

	for (int j = 0; j < IndexCount; j++)
	{
		mIndex[j] = (*TriangleData->IndexList)[j];
	}
}

physx::PxShape* Factory::CreateTriangleCollider(physx::PxMaterial* m, TriangleMeshData* TriangleData)
{ 
	int num = sizeof(PxVec3);

	

	int IndexCount = TriangleData->IndexList->size();
	int VertexCount = TriangleData->VertexList->size();

	//std::vector<PxVec3> VectexList;
	//std::vector<PxU32> VectexList;

	PxVec3* VectexList	= new PxVec3[VertexCount];
	PxU32* IndexList	= new PxU32[IndexCount];

	CreateTriangleBuffer(TriangleData, VectexList, IndexList);


	PxTriangleMeshDesc meshDesc;
	///버텍스 관련 데이터
	meshDesc.points.count		= VertexCount;
	meshDesc.points.stride		= sizeof(PxVec3);
	meshDesc.points.data		= VectexList;

	///페이스 관련 데이터
	meshDesc.triangles.count	= IndexCount;
	meshDesc.triangles.stride	= 3*sizeof(PxU32);
	meshDesc.triangles.data		= IndexList;

	PxTriangleMesh* triMesh = m_Cooking->createTriangleMesh(meshDesc, m_Phys->getPhysicsInsertionCallback());
	PxTriangleMeshGeometry geom;
	geom.triangleMesh = triMesh;
	physx::PxShape* shape = m_Phys->createShape(geom, *m);

	//bool t = m_Cooking->validateTriangleMesh(meshDesc);
	//
	//PxDefaultMemoryOutputStream writeBuffer;
	//PxTriangleMeshCookingResult::Enum result;
	//bool status = m_Cooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
	//if (!status)
	//	return NULL;
	//
	//PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	//PxTriangleMesh* Triangle =  m_Phys->createTriangleMesh(readBuffer);
	//
	//PxTriangleMeshGeometry geom;
	//geom.triangleMesh = Triangle;
	//
	//physx::PxShape* shape = m_Phys->createShape(geom, *m);

	//Triangle->release();
	return shape;
}


//void setupCommonCookingParams(PxCookingParams* params, bool skipMeshCleanup, bool skipEdgeData)
//{
//	// we suppress the triangle mesh remap table computation to gain some speed, as we will not need it 
//// in this snippet
//	params->suppressTriangleMeshRemapTable = true;
//
//	// If DISABLE_CLEAN_MESH is set, the mesh is not cleaned during the cooking. The input mesh must be valid. 
//	// The following conditions are true for a valid triangle mesh :
//	//  1. There are no duplicate vertices(within specified vertexWeldTolerance.See PxCookingParams::meshWeldTolerance)
//	//  2. There are no large triangles(within specified PxTolerancesScale.)
//	// It is recommended to run a separate validation check in debug/checked builds, see below.
//
//	if (!skipMeshCleanup)
//		params->meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);
//	else
//		params->meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
//
//	// If DISABLE_ACTIVE_EDGES_PREDOCOMPUTE is set, the cooking does not compute the active (convex) edges, and instead 
//	// marks all edges as active. This makes cooking faster but can slow down contact generation. This flag may change 
//	// the collision behavior, as all edges of the triangle mesh will now be considered active.
//	if (!skipEdgeData)
//		params->meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE);
//	else
//		params->meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
//}


