#include "MainHeader.h"
#include "intro.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "Transform.h"
#include "AI.h"
#include "KeyInput.h"
#include "Camera.h"
#include "Light.h"

void intro::Awake()
{
	//불러오는 매쉬의 경로 파악
	LoadMeshPath("../Resources/Mesh/");
	LoadTesturePath("../Resources/Texture/");
	
	LoadMesh("Player_Idle");
	//LoadMesh("EnemyB");
	LoadMesh("Field");
	LoadMesh("Table");
	//LoadMesh("box");
	//LoadMesh("Dome_v03");
	//LoadMesh("1s_table_long_lower");
	//LoadMesh("Enemy");
	//LoadMesh("Player_Attack");
	//LoadMesh("Skinning");
	//LoadMesh("Enemy");
	//LoadMesh("box");
	//LoadMesh("AnimeBox");
	//LoadTesture("body_normal_tangent_Base_color.png");
	LoadTesture("Dump.png");

	///카메라
	testobj = Instance();
	//testobj->AddComponent<Keyinput>();
	testobj->AddComponent<Camera>();
	testobj->AddComponent<Keyinput>();
	testobj->GetTransform()->Position = { 0,0,-50 };

	///라이트
	testobj = Instance();
	testobj->AddComponent<DirectionLight>();

	testobj = Instance("obj1");
	testobj->AddComponent<MeshFilter>()->SetMeshName("Field");
	testobj->GetTransform()->Position = { 0 ,0, 0 };
	testobj->GetTransform()->Scale = { 1 , 1, 1 };

	testobj = Instance("obj2");
	testobj->AddComponent<MeshFilter>()->SetMeshName("Table");
	testobj->GetTransform()->Position = { 5 ,0, 0 };
	testobj->GetTransform()->Scale = { 1 , 1, 1 };

	//testobj = Instance("obj");
	//MeshFilter* Filter		= testobj->AddComponent<MeshFilter>();
	////AnimationController* AC = testobj->AddComponent<AnimationController>();
	//Filter->SetMeshName("Player_Idle");
	////Filter->SetTextureName("body_normal_tangent_Base_color");
	////testobj->SetActive(false);
	//
	//testobj->GetTransform()->Position	= { 0 ,0, 0 };
	//testobj->GetTransform()->Scale		= { 1 ,1, 1 };
	//testobj->GetTransform()->Rotation	= {0 ,0,0 };
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
