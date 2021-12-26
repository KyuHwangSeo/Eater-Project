#include "EngineData.h"
#include "BaseManager.h"
#include "Camera.h"
#include "Light.h"

GlobalData* BaseManager::Global = nullptr;

void BaseManager::Initialize()
{
	Global = new GlobalData();
}

void BaseManager::Reset()
{
	if (Global != nullptr)
	{
		delete Global;
		Global = nullptr;
	}
}

void BaseManager::UpdateGlobalData()
{
	static Matrix gTexSpace = Matrix(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	//카메라 데이터
	Matrix camView = Camera::g_MainCam->GetView();
	Matrix camProj = Camera::g_MainCam->GetProj();
	Matrix camInvView;
	camInvView._11 = camView._11; 	camInvView._13 = camView._13;
	camInvView._31 = camView._31;	camInvView._33 = camView._33;

	Global->mCamView = camView;
	Global->mCamProj = camProj;
	Global->mCamInvView = camInvView.Invert();
	Global->mCamPos = Camera::g_MainCam->GetPos();

	Global->mCamVP = camView * camProj;
	Global->mCamPT = camProj * gTexSpace;
	Global->mCamVPT = camView * camProj * gTexSpace;

	Matrix lightView = DirectionLight::g_DirLight->GetView();
	Matrix lightProj = DirectionLight::g_DirLight->GetProj();

	//라이트 데이터
	Global->mLightView = lightView;
	Global->mLightProj = lightProj;

	Global->mLightVP = lightView * lightProj;
	Global->mLightVPT = lightView * lightProj * gTexSpace;
}

GlobalData* BaseManager::GetGlobalData()
{
	return Global;
}
