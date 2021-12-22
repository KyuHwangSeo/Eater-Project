#include "LightManager.h"
#include "EngineData.h"

std::vector<Light*> LightManager::g_LightList;
LightData* LightManager::g_LightData;
LightManager::LightManager()
{
	g_LightData = new LightData();
}

LightManager::~LightManager()
{

}

void LightManager::Initialize()
{
	// Light 관련 Global Data 등록..
	Global->mLightData = g_LightData;
}

void LightManager::PushLight(Light* light)
{
	// Light Type에 따른 Global Data 삽입..
	switch (light->GetType())
	{
	case eLightType::DIRECTION:
	{
		DirectionLight* dirLight = reinterpret_cast<DirectionLight*>(light);
		g_LightData->DirLights[g_LightData->gDirLightCount++] = dirLight->GetLightData();
	}
	break;
	case eLightType::SPOT:
	{
		SpotLight* spotLight = reinterpret_cast<SpotLight*>(light);
		g_LightData->SpotLights[g_LightData->gSpotLightCount++] = spotLight->GetLightData();
	}
	break;
	case eLightType::POINT:
	{
		PointLight* pointLight = reinterpret_cast<PointLight*>(light);
		g_LightData->PointLights[g_LightData->gPointLightCount++] = pointLight->GetLightData();
	}
	break;
	default:
		break;
	}

	// 새로운 Light 추가..
	g_LightList.push_back(light);
}
