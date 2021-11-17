#include "MainHeader.h"
#include "intro.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
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
	
	//LoadMesh("Player");
	//LoadMesh("Table");
	//LoadMesh("Dome_v03");
	//LoadMesh("1s_table_long_lower");
	//LoadMesh("EnemyB");
	LoadMesh("Player");
	//LoadMesh("Skinning");
	//LoadMesh("Enemy");
	//LoadMesh("box");
	LoadMesh("Field");
	LoadTesture("WoodCrate01");

	///카메라
	testobj = Instance();
	testobj->AddComponent<Keyinput>();
	testobj->AddComponent<Camera>();

	/// Light
	testobj = Instance();
	testobj->AddComponent<DirectionLight>();

	testobj = Instance("obj1");
	testobj->AddComponent<MeshFilter>()->SetMeshName("Field");
	//testobj->AddComponent<AI>();
	testobj->GetTransform()->Position = { 0 ,0, 0 };
	testobj->GetTransform()->Scale = { 1 , 1, 1 };

	testobj = Instance("obj");
	testobj->AddComponent<MeshFilter>()->SetMeshName("Player");
	//testobj->AddComponent<AI>();
	testobj->GetTransform()->Position = { 0 ,0, 0 };
	testobj->GetTransform()->Scale = { 1 , 1, 1 };
	testobj->GetTransform()->Rotation = { 45 ,0,0 };


	//testobj->GetTransform()->Rotation = { 90 ,0,0 };
	//매쉬 필터로 매쉬의 정보만 읽은 오브젝트
	
	//
	//
	////스키닝필터로 본과 매쉬정보 둘다 읽은 오브젝트
	//testobj = Instance("Table");
	//testobj->AddComponent<SkinningFilter>()->SetMeshName("EnemyB");
	//testobj->GetTransform()->Position = { 1 ,0,0 };
	//testobj->GetTransform()->Scale = { 100 ,100,100 };
	//testobj->GetTransform()->Rotation = { 90 ,0,0 };
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
