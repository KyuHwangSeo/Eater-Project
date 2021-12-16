#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"
#include <vector>
#include <string>

class Terrain : public Component
{
public:
	EATER_ENGINEDLL Terrain();
	~Terrain();

public:
	EATER_ENGINEDLL void AddLayer(std::string diffuseName, std::string normalName);
	EATER_ENGINEDLL void SetTextureTiling(float scale);
	EATER_ENGINEDLL void SetTextureTiling(float scale_x, float scale_y);

private:
	std::vector<Material*> m_MaterialLayer;
};