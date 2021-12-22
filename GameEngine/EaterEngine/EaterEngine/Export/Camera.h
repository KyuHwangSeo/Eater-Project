#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Component.h"
#include "EaterEngineDLL.h"

/// <summary>
/// ī�޶� ���۳�Ʈ ���Ʈ������ �������� ��Ʈ������ �������ش�
/// </summary>
 
class Transform;
class Camera : public Component
{
public:
	EATER_ENGINEDLL Camera();
	virtual ~Camera();

	virtual void Start();
	virtual void Update();

	//����ī�޶� ����ī�޶�� �����Ų��
	void ChoiceMainCam();
	// Camera Position
	DirectX::SimpleMath::Vector3 GetPos();
	//���� ī�޶��� �����
	DirectX::SimpleMath::Matrix GetView();
	//�������� ����� �����´�
	DirectX::SimpleMath::Matrix GetProj();

	//ī�޶��� ���� ��ǥ�� �����´�
	//static DirectX::XMFLOAT3  GetLocalPos_Up();
	//static DirectX::XMFLOAT3  GetLocalPos_Right();
	//static DirectX::XMFLOAT3  GetLocalPos_Look();

	//OnResize ���� ����� �Լ�
	void SetSize(int Change_Width, int Change_Height);
	void CreateProj(int winsizeX,int WinSizeY, bool ViewPoint = false);
private:
	Transform* tranform;

	//�������� ��� �����ϱ�
	void CreateView();

	//��������
	DirectX::SimpleMath::Matrix mProj;

	//��
	DirectX::SimpleMath::Matrix mView;

	//ī�޶� ����Ʈ���� ���� �ε���
	int MyIndex;
	//ī�޶󸮽�Ʈ�� ���ڽ��� �о�ִ´�
	void PushCamList();
	//ī�޶� ����Ʈ
	static std::vector<Camera*> CamList;

public:
	//����ī�޶�
	static Camera* g_MainCam;
};