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
	//LoadMesh("Field");
	//LoadMesh("Table");
	//LoadMesh("box");
	//LoadMesh("Dome_v03");
	//LoadMesh("1s_table_long_lower");
	//LoadMesh("MOdNA09_highpoly_1123");
	//LoadMesh("Enemy_Run",false, true);
	//LoadMesh("Player_Attack");
	//LoadMesh("Skinning");
	//LoadMesh("Enemy_Run",false,false);
	//LoadMesh("Enemy_Roll",false,true);
	//LoadMesh("Anim_Run",false,false);
	//LoadMesh("Anim_Idle",false,true);
	//LoadMesh("Anim_Idle",false,true);
	//LoadMesh("box");
	//LoadMesh("AnimeBox");
	//LoadTesture("body_normal_tangent_Base_color.png");
	//LoadTesture("Dump.png");
	//LoadTesture("Player.dds");
	//LoadTesture("body_normal_tangent_Base_color.png");

	///카메라
	testobj = Instance("Cam");
	testobj->AddComponent<Camera>();
	testobj->AddComponent<Keyinput>();
	testobj->GetTransform()->Position = { 0,0,-25 };
	
	///라이트
	testobj = Instance("DirectionLight");
	testobj->AddComponent<DirectionLight>();

	//testobj = Instance("obj");
	//MeshFilter* Filter		= testobj->AddComponent<MeshFilter>();
	//AnimationController* AC = testobj->AddComponent<AnimationController>();
	//Filter->SetMeshName("MOdNA09_highpoly_1123");

	testobj = Instance("obj1");
	MeshFilter* Filter = testobj->AddComponent<MeshFilter>();
	AnimationController* AC = testobj->AddComponent<AnimationController>();
	Filter->SetMeshName("Player_Idle");
	//Filter->SetTextureName("Player");
	//Filter->SetAnimationName("Anim");
	//testobj->SetActive(false);
	
	testobj->GetTransform()->Position	= { 0 ,0, 0 };
	testobj->GetTransform()->Scale		= { 0.01f ,0.01f, 0.01f };
	testobj->GetTransform()->Rotation	= {90 ,0,0 };
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
