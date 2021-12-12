#include "Player.h"
#include "AnimationController.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "GameObject.h"
#include "MainHeader.h"

Player::Player()
{
	AC = nullptr;
	Tr = nullptr;
	Mf = nullptr;
}

Player::~Player()
{
	
}

void Player::Awake()
{
	//컨퍼넌트 가져오기
	Tr = gameobject->GetTransform();
	Mf = gameobject->GetComponent<MeshFilter>();
	AC = gameobject->GetComponent<AnimationController>();


	//총 오브젝트 생성
	Gun = Instance("Gun");
	Gun->AddComponent<MeshFilter>()->SetMeshName("Pistol");

	//몽둥이 오브젝트 생성
	Weapon = Instance("Weapon");
	Weapon->AddComponent<MeshFilter>()->SetMeshName("Weapon");
}

void Player::Start()
{
	//컨퍼넌트 초기화
	Mf->SetTextureName("Player");
	Mf->SetAnimationName("Player");
	Mf->SetMeshName("Player");

	Tr->Position = { 0,0,0 };
	Tr->Scale = { 1 ,1, 1 };
	Tr->Rotation = { 0 ,0,0 };

	//무기를 넣을 본을 찾아옴
	RightHand	= gameobject->GetChildBone("mixamorig:RightHand");
	LeftHand	= gameobject->GetChildBone("mixamorig:LeftHand");


	//무기를 해당 본의 자식으로 넣는다
	RightHand->ChoiceChild(Gun);
	LeftHand->ChoiceChild(Weapon);

	//각각의 무기들의 위치값을 조정
	Gun->transform->Scale = { 1,1 ,1 };
	Gun->transform->Rotation = { -90,-90,0 };
	
	Weapon->transform->Scale = { 1,1 ,1 };
	Weapon->transform->Rotation = { -90,90,0 };
}

void Player::Update()
{
	float Speed = 2.5f * GetDeltaTime();

	if (GetKey(VK_RIGHT))
	{
		Move = true;
		gameobject->transform->SetLocalPosition(Speed,0,0);
	}
	else if (GetKey(VK_LEFT))
	{
		gameobject->transform->SetLocalPosition(-Speed, 0, 0);
		Move = true;
	}
	else if (GetKey(VK_UP))
	{
		gameobject->transform->SetLocalPosition(0, 0, Speed);
		Move = true;
	}
	else if (GetKey(VK_DOWN))
	{
		gameobject->transform->SetLocalPosition(0, 0, -Speed);
		Move = true;
	}
	else
	{
		Move = false;
	}

	if (GetKey('Q'))
	{
		gameobject->transform->SetRotate(0,1,0);
	}
	else if (GetKey('E'))
	{
		gameobject->transform->SetRotate(0, 1, 0);
	}




	if (Move == true)
	{
		AC->Choice("Run");
	}
	else
	{
		AC->Choice("Idle");
	}

	AC->Play(1, true);
}


