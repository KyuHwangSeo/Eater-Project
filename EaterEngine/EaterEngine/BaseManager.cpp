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

	//ī�޶� ������
	Global->mCamView = Camera::g_MainCam->GetView();
	Global->mCamProj = Camera::g_MainCam->GetProj();
	Global->mCamPos = Camera::g_MainCam->GetPos();

	Global->mCamVP = Global->mCamView * Global->mCamProj;
	Global->mCamPT = Global->mCamProj * gTexSpace;
	Global->mCamVPT = Global->mCamView * Global->mCamProj * gTexSpace;

	//����Ʈ ������
	Global->mLightView = DirectionLight::g_DirLight->GetView();
	Global->mLightProj = DirectionLight::g_DirLight->GetProj();

	Global->mLightVP = Global->mLightView * Global->mLightProj;
	Global->mLightVPT = Global->mLightView * Global->mLightProj * gTexSpace;
}

GlobalData* BaseManager::GetGlobalData()
{
	return Global;
}
