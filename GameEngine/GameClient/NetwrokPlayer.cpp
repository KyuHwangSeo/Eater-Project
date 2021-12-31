#include "NetwrokPlayer.h"
#include "GameObject.h"
#include "MainHeader.h"
#include "ClientNetworkManager.h"
#include "PlayerData_generated.h"

#include "Rigidbody.h"
#include "MeshFilter.h"
#include "Transform.h"
#include "AnimationController.h"

#include "GameClientGameServerPacketDefine.h"


NetwrokPlayer::NetwrokPlayer()
{
	mTransform	= nullptr;
	mRigidbody	= nullptr;
	mMeshFilter = nullptr;
	mAnimation	= nullptr;

	NET_MOVE_PASS = true;
	ENT_MY_PLAYER = false;

	PositionX	= 0.0f;
	PositionY	= 0.0f;
	PositionZ	= 0.0f;
	DirectionX	= 0.0f;
	DirectionY	= 0.0f;
	DirectionZ	= 0.0f;
	Speed		= 2;

	AnimeType		= -1;
	PlayerNumber	= -1;
	ClientNumber	= -1;

	Keyinput_Right	= 0.0f;
	keyinput_Up		= 0.0f;
	Keyinput_Angle	= 0.0f;

	ClientNetworkManager::AddPlayerObject(this);
}

NetwrokPlayer::~NetwrokPlayer()
{

}

void NetwrokPlayer::Awake()
{
	//���۳�Ʈ�� �������°�
	mTransform	= gameobject->GetTransform();
	//mRigidbody	= gameobject->GetComponent<Rigidbody>();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
	mAnimation = gameobject->GetComponent<AnimationController>();
}

void NetwrokPlayer::SetUp()
{
	//���۳�Ʈ �ʱ�ȭ �ϴ°�
	//mRigidbody->CreateSphereCollider(1);
	//mMeshFilter->SetMeshName("Sphere");
	mTransform->Scale = { 0.1f,0.1f,0.1f };
	mTransform->Position = { 0,-9.25f,0 };
	mObj = gameobject->GetChildBone(0);
	mAnimation->Choice("Idle");
}


void NetwrokPlayer::Update()
{
	//���� ������ �÷��̾ �����ϼ��ֵ���
	//���� ClientNumber�� 1,2 �� ������ �÷��̾� �ѹ��� 20000,20001 �� ���´� �װ��� �����ֱ�����

	switch (ClientNumber)
	{
		case 1:
		{
			if (PlayerNumber == EATER_PLAYER_01)
			{
				KeyInputMove();
			}
			else
			{
				RemoteMove();
			}

			break;
		}
		case 2:
		{

			if (PlayerNumber == EATER_PLAYER_01)
			{
				RemoteMove();
			}
			else
			{
				KeyInputMove();
			}

			break;
		}
	}
	
	DataSetting();
	mAnimation->Play(1, true);
}

void NetwrokPlayer::RECV(void* mData, int type)
{
	switch (type)
	{
	case S2C_PLAYER_MOVE_RES:
		S2C_PLAYER_MOVE_RECV(mData);
		break;
	case S2C_CLIENT_INFO:
		S2C_PLAYER_INFO_RECV(mData);
		break;
	}
}

void NetwrokPlayer::S2C_PLAYER_MOVE_RECV(void* mData)
{
	//��Ʈ��ũ �Ŵ������� PlayerData�� �޾ƿ´�
	Eater::PlayerData::Player* Data = reinterpret_cast<Eater::PlayerData::Player*>(mData);
	
	//�ִϸ��̼� Ÿ��
	AnimeType = Data->animation_type();
	//if (PlayerNumber == -1)
	//{
	//	//PlayerNumber = Data->identifier();
	//}
	//��ġ
	PositionX = Data->pos()->x();
	PositionY = Data->pos()->y();
	PositionZ = Data->pos()->z();

	//����
	DirectionX = Data->mov_vector()->x();
	DirectionY = Data->mov_vector()->y();
	DirectionZ = Data->mov_vector()->z();

	//���ǵ�
	Speed = 2;
	ENT_MY_PLAYER = false;

	if (NET_MOVE_PASS == false)
	{
		//������ ����������
		//Speed = Data->Speed;


		//�����ϰ� �����ְ� true;
		NET_MOVE_PASS = true;
	}
}

void NetwrokPlayer::S2C_PLAYER_INFO_RECV(void* mData)
{
	if (ClientNumber == -1)
	{
		int* number = reinterpret_cast<int*>(mData);
		ClientNumber = *number;
	}
}

void NetwrokPlayer::KeyInputMove()
{
	if (GetKey(VK_UP))
	{
		Keyinput_Right = Speed * GetDeltaTime();
		Keyinput_Angle = 180;
	}
	else if (GetKey(VK_DOWN))
	{
		Keyinput_Right = -Speed * GetDeltaTime();
		Keyinput_Angle = 0;
	}
	else
	{
		Keyinput_Right = 0;
	}


	if (GetKey(VK_RIGHT))
	{
		keyinput_Up = Speed * GetDeltaTime();
		Keyinput_Angle = -90;
	}
	else if (GetKey(VK_LEFT))
	{
		keyinput_Up = -Speed * GetDeltaTime();
		Keyinput_Angle = 90;
	}
	else
	{
		keyinput_Up = 0;
	}

	//�ƹ��͵� �������ʾ��� ���
	if (keyinput_Up == 0 && Keyinput_Right == 0)
	{
		mAnimation->Choice("Idle");
		AnimeType = ANIMATION_IDLE;
	}
	else
	{
		mAnimation->Choice("Run");
		AnimeType = ANIMATION_RUN;
	}

	mTransform->SetTranlate(keyinput_Up, 0, Keyinput_Right);
	mTransform->Rotation = {0,Keyinput_Angle,0};
}

void NetwrokPlayer::KeyInputMoveTest()
{
	if (GetKey(VK_NUMPAD5))
	{
		Keyinput_Right = Speed * GetDeltaTime();
		Keyinput_Angle = 180;
	}
	else if (GetKey(VK_NUMPAD2))
	{
		Keyinput_Right = -Speed * GetDeltaTime();
		Keyinput_Angle = 0;
	}
	else
	{
		Keyinput_Right = 0;
	}


	if (GetKey(VK_NUMPAD3))
	{
		keyinput_Up = Speed * GetDeltaTime();
		Keyinput_Angle = -90;
	}
	else if (GetKey(VK_NUMPAD1))
	{
		keyinput_Up = -Speed * GetDeltaTime();
		Keyinput_Angle = 90;
	}
	else
	{
		keyinput_Up = 0;
	}

	if (keyinput_Up == 0 && Keyinput_Right == 0)
	{
		mAnimation->Choice("Idle");
		AnimeType = ANIMATION_IDLE;
	}
	else
	{
		mAnimation->Choice("Run");
		AnimeType = ANIMATION_RUN;
	}


	mTransform->SetTranlate(keyinput_Up, 0, Keyinput_Right);
	mTransform->Rotation = { 0,Keyinput_Angle,0 };
}

void NetwrokPlayer::RemoteMove()
{
	mTransform->Position = { PositionX,PositionY,PositionZ};
	mTransform->Rotation = { DirectionX,DirectionY,DirectionZ };
	switch (AnimeType)
	{
	case ANIMATION_IDLE:
		mAnimation->Choice("Idle");
		break;
	case ANIMATION_RUN:
		mAnimation->Choice("Run");
		break;
	}
}

void NetwrokPlayer::DataSetting()
{
	//��ġ�� ����
	PositionX = mTransform->Position.x;
	PositionY = mTransform->Position.y;
	PositionZ = mTransform->Position.z;

	//ȸ���� ����
	DirectionX = mTransform->Rotation.x;
	DirectionY = mTransform->Rotation.y;
	DirectionZ = mTransform->Rotation.z;

	
}
