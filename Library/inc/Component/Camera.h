#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Component.h"
#include "EaterEngineDLL.h"

/// <summary>
/// 카메라 컨퍼넌트 뷰매트릭스와 프로젝션 매트릭스를 생성해준다
/// </summary>
 
class Transform;
class Camera : public Component
{
public:
	EATER_ENGINEDLL Camera();
	virtual ~Camera();

	virtual void Awake();
	virtual void Update();

	//현재카메라를 메인카메라로 변경시킨다
	void ChoiceMainCam();
	// Camera Position
	DirectX::XMFLOAT3* GetPos();
	// Camera View TexSpace
	DirectX::XMMATRIX* GetViewTex();
	//현재 카메라의 뷰행렬
	DirectX::XMMATRIX* GetView();
	//프로젝션 행렬을 가져온다
	DirectX::XMMATRIX* GetProj();

	//카메라의 로컬 좌표를 가져온다
	//static DirectX::XMFLOAT3  GetLocalPos_Up();
	//static DirectX::XMFLOAT3  GetLocalPos_Right();
	//static DirectX::XMFLOAT3  GetLocalPos_Look();

	//OnResize 에서 실행될 함수
	void SetSize(int Change_Width, int Change_Height);
	void CreateProj(int winsizeX,int WinSizeY, bool ViewPoint = false);
private:
	Transform* tranform;

	//프로젝션 행렬 생성하기
	void CreateView();

	//프로젝션
	DirectX::XMFLOAT4X4 mProj;
	DirectX::XMMATRIX mProj_M;

	//뷰
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMMATRIX mView_M;

	//ViewTexSpace
	DirectX::XMMATRIX mViewTex_M;

	//카메라 리스트에서 나의 인덱스
	int MyIndex;
	//카메라리스트에 나자신을 밀어넣는다
	void PushCamList();
	//카메라 리스트
	static std::vector<Camera*> CamList;

public:
	//메인카메라
	static Camera* g_MainCam;
};