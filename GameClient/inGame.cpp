#include "MainHeader.h"
#include "inGame.h"
#include "intro.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "KeyInput.h"
#include "Camera.h"

void InGame::Awake()
{
	//불러오는 매쉬의 경로 파악
	///카메라
	GameObject* testobj = Instance("카메라 오브젝트");
	testobj->AddComponent<Keyinput>();
	testobj->AddComponent<Camera>();


	testobj = Instance();
	testobj->AddComponent<MeshFilter>()->SetMeshName("Table");
	testobj->GetTransform()->Position = { 0,1,1 };
	
}

void InGame::Start()
{
}

void InGame::Update()
{
	
}

void InGame::End()
{
}
