#include "EngineData.h"
#include "BaseManager.h"

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

GlobalData* BaseManager::GetGlobalData()
{
	return Global;
}
