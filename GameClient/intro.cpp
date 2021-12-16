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

void intro::Awake()
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

	LoadTerrainMesh("Terrain", "Terrain_RGB.png", SCALING);

	LoadTexture("Player.dds");
	LoadTexture("Dump.png");


	///카메라
	testobj = Instance("Cam");
	testobj->AddComponent<Camera>();
	testobj->AddComponent<Keyinput>();
	testobj->GetTransform()->Position = { 0,0,-25 };

	///라이트
	testobj = Instance("DirectionLight");
	testobj->AddComponent<DirectionLight>();

	///캐릭터
	testobj = Instance("CHARACTER");
	testobj->AddComponent<AnimationController>();
	testobj->AddComponent<MeshFilter>();
	testobj->GetComponent<MeshFilter>()->SetMeshName("CHARACTER");
	testobj->GetComponent<MeshFilter>()->SetAnimationName("CHARACTER");
	testobj->GetComponent<AnimationController>()->Choice("idle");
	testobj->GetTransform()->Scale = { 0.2f, 0.2f, 0.2f };
	testobj->GetTransform()->Position = { 100.0f, 0.0f, 0.0f };

	///캐릭터
	testobj = Instance("Player");
	testobj->AddComponent<AnimationController>();
	testobj->AddComponent<MeshFilter>();
	testobj->AddComponent<Player>();

	/// 바닥
	testobj = Instance("Field");
	testobj->AddComponent<MeshFilter>()->SetMeshName("Field");
	testobj->GetComponent<Transform>()->Scale = { 0.5f, 0.5f, 0.5f };

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

void intro::Start()
{

}

void intro::Update()
{

}

void intro::End()
{


}
