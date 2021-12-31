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
#include "NetwrokPlayer.h"
#include "ClientNetworkManager.h"

void intro::Awake()
{
	//불러오는 매쉬의 경로 파악
	LoadMeshPath("../Resources/Mesh/");
	LoadTexturePath("../Resources/Texture/");


	LoadTerrainMesh("Terrain","Terrain_RGB.png", SCALING);
	//LoadMesh("box", SCALING);
	//LoadMesh("Player_Run", ANIMATION_ONLY);
	LoadMesh("TerrainDecimate",SCALING);
	//LoadMesh("Sphere",SCALING);
	//LoadMesh("MOdNA_Idle", SCALING);
	//LoadMesh("MOdNA_Run", SCALING | ANIMATION_ONLY);
	//LoadMesh("Dome", SCALING);
	//LoadMesh("Inside", SCALING);
	//LoadMesh("outside", SCALING);
	//LoadMesh("outsiderock", SCALING);
	//LoadMesh("BossA", SCALING);
	//LoadMesh("BossB", SCALING);

	
	//LoadTexture("Player.dds");
	LoadTexture("Dump.png");
	//LoadTexture("ColorMap_1.png");
	LoadTexture("ground01_Albedo.png");
	LoadTexture("ground02_Albedo.png");
	LoadTexture("ground01_Normal.png");
	LoadTexture("ground02_Normal.png");

	//Network_Loading_Send();

	///라이트
	testobj = Instance("DirectionLight");
	testobj->AddComponent<DirectionLight>();

	///카메라
	testobj = Instance("Cam");
	testobj->AddComponent<Camera>();
	testobj->AddComponent<Keyinput>();
	testobj->GetTransform()->Position = { 0,0,-25 };



	//네트워크 매니저 생성후 Engine쪽으로 넘겨준다

	GameObject*		obj = nullptr;
	MeshFilter*		MF	= nullptr;
	NetwrokPlayer*	NP	= nullptr;

	//네트워크 테스트용
	//CreatePlayerNetwork();

	//그냥 일반 테스트용
	//CreatePlayer();
	//CreateObject(0, 2, 0, 0);
	//CreateMap();

	///터레인
	CreateTerrain();
}

void intro::Start()
{

}

void intro::Update()
{
	if (GetKey(VK_RIGHT))
	{
		BossA->transform->SetRotate(0,0,1);
		BossB->transform->SetRotate(0,0,1);
	}
	if (GetKey(VK_LEFT))
	{
		BossA->transform->SetRotate(0, 0, -1);
		BossB->transform->SetRotate(0, 0, -1);
	}

	if (GetKey(VK_UP))
	{
		BossA->transform->SetRotate(-1, 0, 0);
		BossB->transform->SetRotate(-1, 0, 0);
	}

	if (GetKey(VK_DOWN))
	{
		BossA->transform->SetRotate(1, 0, 0);
		BossB->transform->SetRotate(1, 0, 0);
	}

}

void intro::End()
{

}

void intro::CreatePlayer()
{
	//GameObject* obj = Instance("Player");
	//MeshFilter* mf = obj->AddComponent<MeshFilter>();
	//Transform* Tr = obj->GetComponent<Transform>();
	//mf->SetMeshName("Dome");
	//Tr->Position = { 0,0,0 };


	GameObject* obj = Instance("Player");
	obj->AddComponent<MeshFilter>();
	obj->AddComponent<Rigidbody>();
	obj->AddComponent<AnimationController>();
	obj->AddComponent<AI>();
}

void intro::CreatePlayerNetwork()
{
	Network_Loading_Send();


	ClientNetworkManager* Manager = testobj->AddComponent<ClientNetworkManager>();
	Network_SetManager(Manager);

	///캐릭터
	GameObject* obj = Instance("Player01");
	MeshFilter* MF = obj->AddComponent<MeshFilter>();
	NetwrokPlayer* NP = obj->AddComponent<NetwrokPlayer>();
	AnimationController* AC	= obj->AddComponent<AnimationController>();

	obj->GetTransform()->Position = { 0,0,0 };
	MF->SetMeshName("MOdNA");
	MF->SetAnimationName("MOdNA");
	NP->PlayerNumber = 20000;


	obj = Instance("Player02");
	MF = obj->AddComponent<MeshFilter>();
	NP	= obj->AddComponent<NetwrokPlayer>();
	obj->AddComponent<AnimationController>();
	obj->GetTransform()->Position = { 10,0,0 };
	MF->SetMeshName("MOdNA");
	MF->SetAnimationName("MOdNA");
	MF->SetTextureName("Player");
	NP->PlayerNumber = 20001;
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

void intro::CreateObject(float x, float y, float z, int type)
{
	if(type == 0)
	{
		GameObject* obj = Instance("box");
		MeshFilter* mMeshFilter = obj->AddComponent<MeshFilter>();
		Rigidbody* mRigdbody = obj->AddComponent<Rigidbody>();
		Transform* mTransform = obj->GetComponent<Transform>();

		mMeshFilter->SetMeshName("box");
		mRigdbody->CreateBoxCollider(0.5f);
		mTransform->Position = { x,y,z };
		mTransform->Scale = { 0.5f,0.5f,0.5f };
	}
	else
	{
		GameObject* obj = Instance("box");
		MeshFilter* mMeshFilter = obj->AddComponent<MeshFilter>();
		Rigidbody* mRigdbody = obj->AddComponent<Rigidbody>();
		Transform* mTransform = obj->GetComponent<Transform>();

		mMeshFilter->SetMeshName("Sphere");
		mRigdbody->CreateSphereCollider(1);
		mTransform->Position = { x,y,z };
	}
}

void intro::CreateMap()
{
	Transform*	Tr;
	MeshFilter* filter;
	//GameObject* Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetMeshName("Dome");

	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetMeshName("Inside");
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetMeshName("outside");
	//Transform* Tr = Object->GetTransform();
	//Tr->Rotation = { -90,0, 180 };
	//
	//
	//Object = Instance();
	//filter = Object->AddComponent<MeshFilter>();
	//filter->SetMeshName("outsiderock");
	//Tr = Object->GetTransform();
	//Tr->Rotation = { -90,0, 0 };


	BossA = Instance();
	filter = BossA->AddComponent<MeshFilter>();
	filter->SetMeshName("BossA");
	Tr = BossA->GetTransform();
	Tr->Rotation = { -90,0, 0 };
	Tr->Scale = { 1,1,1 };
	Tr->Position = { 0,0,0 };


	BossB = Instance();
	filter = BossB->AddComponent<MeshFilter>();
	filter->SetMeshName("BossB");
	Tr = BossB->GetTransform();
	Tr->Rotation = { -90,0, 0 };
	Tr->Position = { 5,2.5f,0 };
	Tr->Scale = { 0.25f,0.25f ,0.25f };
}
