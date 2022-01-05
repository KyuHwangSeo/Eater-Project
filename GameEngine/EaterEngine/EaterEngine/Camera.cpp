#include "Camera.h"
#include "GameObject.h"
#include "EngineData.h"
#include "Transform.h"
#include "DebugManager.h"

using namespace DirectX;
std::vector<Camera*> Camera::CamList;
Camera* Camera::g_MainCam = nullptr;

Camera::Camera()
{
	PushCamList();
}

Camera::~Camera()
{
	if (g_MainCam == this) { g_MainCam = nullptr; }

	CamList[MyIndex] = nullptr;
}

void Camera::Start()
{
	//������Ʈ�� ���۳�Ʈ ��������
	tranform = gameobject->transform;
	CreateProj(1920,1080);
	gameobject->OneMeshData->ObjType = OBJECT_TYPE::CAMERA;
}

void Camera::Update()
{
	CreateView();
}

DirectX::SimpleMath::Matrix Camera::GetProj()
{
	return mProj;
}

//DirectX::XMFLOAT3 Camera::GetLocalPos_Up()
//{
//	if (g_MainCam == nullptr) { return; }
//	return g_MainCam->gameobject->GetTransform()->GetLocalPosition_UP();
//}
//
//DirectX::XMFLOAT3 Camera::GetLocalPos_Right()
//{
//	if (g_MainCam == nullptr) { return; }
//	return g_MainCam->gameobject->GetTransform()->GetLocalPosition_Right();
//}
//
//DirectX::XMFLOAT3 Camera::GetLocalPos_Look()
//{
//	if (g_MainCam == nullptr) { return; }
//	return g_MainCam->gameobject->GetTransform()->GetLocalPosition_Look();
//}

DirectX::SimpleMath::Vector3 Camera::GetPos()
{
	return tranform->Position;
}

void Camera::SetSize(int Change_Width, int Change_Height)
{
	float Ratio = (float)(Change_Width / Change_Height);
}

void Camera::ChoiceMainCam()
{
	g_MainCam = this;
}

DirectX::SimpleMath::Matrix Camera::GetView()
{
	return mView;
}

void Camera::CreateProj(int winsizeX, int WinSizeY, bool ViewPoint)
{
	///�𸣸� MSDN ���� 
	///XMMatrixPerspectiveFovLH �Լ� �˻��ϸ��
	
	static XMMATRIX gTexSpace = XMMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	//�þ߰�
	float mFovY	= 0.25f * 3.1415926535f;
	//�� ���� X:Y ���μ��κ���
	float mAspect = (float)winsizeX / (float)WinSizeY;
	//������� ������ �Ÿ� 0���� Ŀ�ߵ�
	float mNearZ	= 0.1f;
	//�� ��� ������ �Ÿ� 0���� Ŀ�ߵ�
	float mFarZ	= 4000.0f;

	float mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	float mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);
	
	if (ViewPoint == false)
	{
		//���� ����
		mProj = DirectX::XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	}
	else
	{
		//���� ����
		mProj = DirectX::XMMatrixOrthographicLH(mFovY, mAspect, mNearZ, mFarZ);
	}
}

void Camera::CreateView()
{
	DirectX::XMFLOAT3 r_ = tranform->GetLocalPosition_Right();
	DirectX::XMFLOAT3 u_ = tranform->GetLocalPosition_UP();
	DirectX::XMFLOAT3 l_ = tranform->GetLocalPosition_Look();


	DirectX::XMVECTOR R = XMLoadFloat3(&r_);
	DirectX::XMVECTOR U = XMLoadFloat3(&u_);
	DirectX::XMVECTOR L = XMLoadFloat3(&l_);
	DirectX::XMVECTOR P = XMLoadFloat3(&tranform->Position);

	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));
	R = XMVector3Cross(U, L);

	float x = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	mView._11 = r_.x;	mView._12 = u_.x;	mView._13 = l_.x; mView._14 = 0;
	mView._21 = r_.y;	mView._22 = u_.y;	mView._23 = l_.y; mView._24 = 0;
	mView._31 = r_.z;	mView._32 = u_.z;	mView._33 = l_.z; mView._34 = 0;
	mView._41 = -x;		mView._42 = -y;		mView._43 = -z;	  mView._44 = 1;
}

void Camera::PushCamList()
{
	//���� ����ī�޶�� �����ѳ��̾��ٸ� ó�������ѳ��� ����ī�޶��
	if (g_MainCam == nullptr)
	{
		g_MainCam = this;
	}

	//ī�޶� ����Ʈ�� �ִ���
	int count = (int)CamList.size();
	for (int i = 0; i < count; i++)
	{
		if (CamList[i] == nullptr)
		{
			CamList[i] = this;
			CamList[i]->MyIndex = i;
			return;
		}
	}

	//����� �Դٴ°��� ����Ʈ�� ��á�ٴ� ��
	CamList.push_back(this);
	//�ڱ��� �ε��� ����
	MyIndex = count;
}

