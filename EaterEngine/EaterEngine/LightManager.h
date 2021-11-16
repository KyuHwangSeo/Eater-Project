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
	void AddLight(Light* light);

private:
	LightData* m_LightData;

	UINT m_DirectionCount;
	UINT m_PointCount;
	UINT m_SpotCount;

	std::vector<Light*> m_LightList;
};

