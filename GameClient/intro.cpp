#include "MainHeader.h"
#include "intro.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "Transform.h"
#include "AI.h"
#include "Player.h"
#include "KeyInput.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "Rigidbody.h"
#include "NetworkManager.h"
#include "UnitNet.h"

/// <summary>
/// 실행순서
///  
/// 씬에서 컨퍼넌트를 AddComponent
///
/// -초기화-----------------------------------------------------
/// 컨퍼넌트 안에 Awack		함수에서 사용할 컨퍼넌트를 가져옴
/// 컨퍼넌트 안에 SetUp		실행 함수에서 사용할 컨퍼넌트를 초기화값 넣어주기
/// 컨퍼넌트 안에 Start		초기화된 값으로 컨퍼넌트를 초기화
/// 
/// -Updata----------------------------------------------
/// StartUpdate			시작단계 업데이트
/// TransformUpdate		이동관련 업데이트 
/// PhysicsUpdate		물리관련 업데이트
/// Update				클라이언트에서 사용할 기본 업데이트
/// EndUpdate			마지막 실행 업데이트
/// 
/// 오브젝트 삭제 여부 체크후 삭제
/// 그래픽랜더큐 생성
/// </summary>

void intro::Awake()
{
	//불러오는 매쉬의 경로 파악
	LoadMeshPath("../Resources/Mesh/");
	LoadTexturePath("../Resources/Texture/");

	

	LoadTerrainMesh("Terrain", "Terrain_RGB.png", SCALING);
	LoadMesh("TerrainDecimate",SCALING);
	LoadMesh("Sphere",SCALING);
	LoadMesh("box",SCALING);

	
	LoadTexture("Player.dds");
	LoadTexture("Dump.png");
	LoadTexture("ground01_Albedo.png");
	LoadTexture("ground02_Albedo.png");
	LoadTexture("ground01_Normal.png");
	LoadTexture("ground02_Normal.png");

	///라이트
	testobj = Instance("DirectionLight");
	testobj->AddComponent<DirectionLight>();

	///카메라
	testobj = Instance("Cam");
	testobj->AddComponent<Camera>();
	testobj->AddComponent<Keyinput>();
	testobj->GetTransform()->Position = { 0,0,-25 };


	///네트워크 매니저
	//GameObject* Net = Instance("Network");
	//NetworkManager* NM = Net->AddComponent<NetworkManager>();
	//NM->Initialize();


	///캐릭터
	//GameObject* obj02 = CreatePlayer(10,1,0);
	GameObject* obj01 = CreatePlayer(0,1,0);
	//NM->AddPlayer(obj01, obj02);



	///터레인
	CreateTerrain();

	///박스
	//for (int i = 0; i < 10; i++) 
	//{
	//	testobj = Instance("box");
	//	MeshFilter* Mf = testobj->AddComponent<MeshFilter>();
	//	Rigidbody* Rig = testobj->AddComponent<Rigidbody>();
	//	Transform* Tr = testobj->GetComponent<Transform>();
	//
	//	Tr->Position = { (float)i*2,10, (float)i*2 };
	//	Tr->Rotation = { 0,0,0 };
	//	Mf->SetMeshName("box");
	//	Rig->CreateBoxCollider(1.0f);
	//}
}

void intro::Start()
{

}

void intro::Update()
{
	
}

void intro::End()
{

}

GameObject* intro::CreatePlayer(float x,float y, float z)
{
	GameObject* obj = Instance("Player");
	obj->AddComponent<MeshFilter>();
	obj->AddComponent<Rigidbody>();
	obj->AddComponent<AI>();
	//obj->AddComponent<UnitNet>();
	obj->GetComponent<Transform>()->Position = { x,y,z };

	return obj;
}

void intro::CreateTerrain()
{
	///터레인
	testobj = InstanceTerrain("Terrain");
	Terrain* mTerrain = testobj->GetComponent<Terrain>();
	mTerrain->SetLayerName("ground01_Albedo", "ground01_Normal");
	mTerrain->SetLayerName("ground02_Albedo", "ground02_Normal");
	mTerrain->SetMeshName("Terrain");
	mTerrain->SetColliderName("TerrainDecimate");
	mTerrain->SetTextureTiling(1.0f / 31.0f);
}

void intro::CreateBox(float x, float y, float z)
{
	




}
