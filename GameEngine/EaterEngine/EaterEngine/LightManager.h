#pragma once
#include <vector>
#include "BaseManager.h"
#include "Light.h"

class LightManager : public BaseManager
{
public:
	LightManager();
	~LightManager();

public:
	void Initialize();
	static void PushLight(Light* light);

private:
	static LightData* g_LightData;

	static std::vector<Light*> g_LightList;
};

