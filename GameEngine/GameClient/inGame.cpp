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
#include "ParticleSystem.h"

void InGame::Awake()
{
	//불러오는 매쉬의 경로 파악
	LoadMeshPath("../Resources/Mesh/");
	LoadTexturePath("../Resources/Texture/");

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
	LoadTexture("tex_vfx-inf_particle_spritesheet_hotCloud-colour.png");
	LoadTexture("tex_vfx-inf_particle_spritesheet_blast.png");

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
	Terrain* terrain = testobj->GetComponent<Terrain>();
	terrain->SetMeshName("Terrain");
	terrain->SetLayerName("ground01_Albedo", "ground01_Normal");
	terrain->SetLayerName("ground02_Albedo", "ground02_Normal");
	terrain->SetColliderName("TerrainDecimate");
	terrain->SetTextureTiling(1.0f / 31.0f);
	testobj->GetTransform()->Rotation = { 0, 0, 0 };

	/// 바닥
	//testobj = Instance("Field");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Field");
	//testobj->GetComponent<Transform>()->Rotation = { -90.f, 0.f, 0.f };
	//testobj->GetComponent<Transform>()->Scale = { 0.05f, 0.05f, 0.05f };


	///캐릭터
	//testobj = Instance("Player");
	//testobj->AddComponent<AnimationController>();
	//testobj->AddComponent<MeshFilter>();
	//testobj->AddComponent<Player>();


	///파티클
	testobj = InstanceParticle("Particle");

	particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetDiffuseName("tex_vfx-inf_particle_spritesheet_blast");
	particles->SetMaxParticles(96);
	particles->SetForceAxis(0, 10, -1);
	particles->SetStartColor(Vector4(0, 0, 0, 1), Vector4(1, 1, 1, 1));
	particles->SetStartRotation(-360, 360);
	particles->SetStartPosition(0.25f);
	particles->SetStartLifeTime(3.0f, 3.0f);
	particles->SetStartSize(2.0f, 3.0f);
	particles->SetLifeTimeRotation(-180, 180);
	particles->SetTextureTiling(8, 8);
	particles->SetRateOverTime(15.0f);
	particles->SetPlay(10.0f, true);


	///캐릭터
	//testobj = Instance("CHARACTER");
	//testobj->AddComponent<AnimationController>();
	//testobj->AddComponent<MeshFilter>();
	//testobj->GetComponent<MeshFilter>()->SetMeshName("CHARACTER");
	//testobj->GetComponent<MeshFilter>()->SetAnimationName("CHARACTER");
	//testobj->GetComponent<AnimationController>()->Choice("idle");
	//
	//testobj->GetTransform()->Scale = { 0.5f, 0.5f, 0.5f };
	//testobj->GetTransform()->Rotation = { -90.0f, 0.0f, 0.0f };
	//testobj->GetTransform()->Position = { 100.0f, 1.0f, 0.0f };


	//testobj = Instance("Field1");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Field");
	//testobj->GetComponent<Transform>()->Rotation = { -90.0f, 90.0f, 0.0f };
	//testobj->GetComponent<Transform>()->Scale = { 0.05f, 0.05f, 0.05f };
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
		testobj->GetTransform()->Position.x -= 10.0f * GetDeltaTime();
	}
	if (GetKey(VK_RIGHT) == true)
	{
		testobj->GetTransform()->Position.x += 10.0f * GetDeltaTime();
	}

	if (GetKey(VK_UP) == true)
	{
		testobj->GetTransform()->Position.z += 10.0f * GetDeltaTime();
	}
	if (GetKey(VK_DOWN) == true)
	{
		testobj->GetTransform()->Position.z -= 10.0f * GetDeltaTime();
	}

	//if (GetKeyUp(VK_F5))
	//{
	//	particles->SetPlay(10.0f, false);
	//}
}

void InGame::End()
{
}
