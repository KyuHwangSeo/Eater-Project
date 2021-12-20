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
/// �������
///  
/// ������ ���۳�Ʈ�� AddComponent
///
/// -�ʱ�ȭ-----------------------------------------------------
/// ���۳�Ʈ �ȿ� Awack		�Լ����� ����� ���۳�Ʈ�� ������
/// ���۳�Ʈ �ȿ� SetUp		���� �Լ����� ����� ���۳�Ʈ�� �ʱ�ȭ�� �־��ֱ�
/// ���۳�Ʈ �ȿ� Start		�ʱ�ȭ�� ������ ���۳�Ʈ�� �ʱ�ȭ
/// 
/// -Updata----------------------------------------------
/// StartUpdate			���۴ܰ� ������Ʈ
/// TransformUpdate		�̵����� ������Ʈ 
/// PhysicsUpdate		�������� ������Ʈ
/// Update				Ŭ���̾�Ʈ���� ����� �⺻ ������Ʈ
/// EndUpdate			������ ���� ������Ʈ
/// 
/// ������Ʈ ���� ���� üũ�� ����
/// �׷��ȷ���ť ����
/// </summary>

void intro::Awake()
{
	//�ҷ����� �Ž��� ��� �ľ�
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

	///����Ʈ
	testobj = Instance("DirectionLight");
	testobj->AddComponent<DirectionLight>();

	///ī�޶�
	testobj = Instance("Cam");
	testobj->AddComponent<Camera>();
	testobj->AddComponent<Keyinput>();
	testobj->GetTransform()->Position = { 0,0,-25 };


	///��Ʈ��ũ �Ŵ���
	//GameObject* Net = Instance("Network");
	//NetworkManager* NM = Net->AddComponent<NetworkManager>();
	//NM->Initialize();


	///ĳ����
	//GameObject* obj02 = CreatePlayer(10,1,0);
	GameObject* obj01 = CreatePlayer(0,1,0);
	//NM->AddPlayer(obj01, obj02);



	///�ͷ���
	CreateTerrain();

	///�ڽ�
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
	///�ͷ���
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
