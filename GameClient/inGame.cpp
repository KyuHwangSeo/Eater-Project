#include "MainHeader.h"
#include "inGame.h"
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

void InGame::Awake()
{
	//불러오는 매쉬의 경로 파악
	LoadMeshPath("../Resources/Mesh/");
	LoadTexturePath("../Resources/Texture/");

	LoadMesh("Table");
	LoadMesh("Field");
	LoadMesh("CHARACTER_idle", SCALING);
	LoadMesh("Player_Run");
	LoadMesh("Player_Roll", ANIMATION_ONLY);
	LoadMesh("Player_Idle", ANIMATION_ONLY);
	LoadMesh("Pistol", SCALING);
	LoadMesh("Weapon", SCALING);
	LoadMesh("TerrainDecimate", SCALING);

	LoadTerrainMesh("Terrain", "Terrain_RGB.png", SCALING);

	LoadTexture("Player.dds");
	LoadTexture("Dump.png");
	LoadTexture("ground01_Albedo.png");
	LoadTexture("ground02_Albedo.png");
	LoadTexture("ground01_Normal.png");
	LoadTexture("ground02_Normal.png");

	///캐릭터
	testobj = Instance("Player");
	testobj->AddComponent<AnimationController>();
	testobj->AddComponent<MeshFilter>();
	testobj->AddComponent<Player>();

	///카메라
	testobj = Instance("Cam");
	testobj->AddComponent<Camera>();
	testobj->AddComponent<Keyinput>();
	testobj->GetTransform()->Position = { 0,5,-25 };
	
	///라이트
	testobj = Instance("DirectionLight");
	testobj->AddComponent<DirectionLight>();


	///터레인
	testobj = InstanceTerrain("Terrain");
	testobj->GetComponent<MeshFilter>()->SetMeshName("Terrain");
	testobj->GetComponent<Terrain>()->SetLayerName("ground01_Albedo", "ground01_Normal");
	testobj->GetComponent<Terrain>()->SetLayerName("ground02_Albedo", "ground02_Normal");
	testobj->GetComponent<Terrain>()->SetColliderName("TerrainDecimate");
	testobj->GetComponent<Terrain>()->SetTextureTiling(1.0f / 31.0f);
	testobj->GetTransform()->Rotation = { 0, 0, 0 };

	/// 바닥
	//testobj = Instance("Field");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Field");
	//testobj->GetComponent<Transform>()->Rotation = { -90.f, 0.f, 0.f };
	//testobj->GetComponent<Transform>()->Scale = { 0.05f, 0.05f, 0.05f };


	///캐릭터
	testobj = Instance("CHARACTER");
	testobj->AddComponent<AnimationController>();
	testobj->AddComponent<MeshFilter>();
	testobj->GetComponent<MeshFilter>()->SetMeshName("CHARACTER");
	testobj->GetComponent<MeshFilter>()->SetAnimationName("CHARACTER");
	//testobj->GetComponent<AnimationController>()->Choice("idle");

	testobj->GetTransform()->Scale = { 0.5f, 0.5f, 0.5f };
	testobj->GetTransform()->Rotation = { -90.0f, 0.0f, 0.0f };
	testobj->GetTransform()->Position = { 100.0f, 1.0f, 0.0f };


	//testobj = Instance("Field1");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Field");
	//testobj->GetComponent<Transform>()->Rotation = { 90.0f, 0.0f, 0.0f };
	//testobj->GetComponent<Transform>()->Scale = { 0.5f, 0.5f, 0.5f };
	//testobj->GetComponent<Transform>()->Position = { 0.0f, 2.5f, 2.5f };

	//testobj = Instance("Table");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Table");
	//testobj->GetComponent<Transform>()->Scale = { 0.1f, 0.1f, 0.1f };
	//testobj->GetTransform()->Position = { -5.0f, 0.0f, -5.0f };
}

void InGame::Start()
{
}

void InGame::Update()
{
	if (GetKey(VK_LEFT) == true)
	{
		testobj->GetTransform()->Position.x -= 250.0f * GetDeltaTime();
	}
	if (GetKey(VK_RIGHT) == true)
	{
		testobj->GetTransform()->Position.x += 250.0f * GetDeltaTime();
	}
	
	if (GetKey(VK_UP) == true)
	{
		testobj->GetTransform()->Position.z += 250.0f * GetDeltaTime();
	}
	if (GetKey(VK_DOWN) == true)
	{
		testobj->GetTransform()->Position.z -= 250.0f * GetDeltaTime();
	}
}

void InGame::End()
{
}
