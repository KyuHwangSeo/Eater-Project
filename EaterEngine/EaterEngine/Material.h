#pragma once
#include "Component.h"
#include "EaterEngineDLL.h"
#include "LightHelper.h"

class Material : public Component
{
public:
	Material();
	~Material();

public:
	void SetMaterialIndex(UINT index);
	UINT GetMaterialIndex();

	void SetMaterialData(MaterialData matData);
	MaterialData GetMaterialData();

private:
	UINT m_Material_Index;

	MaterialData m_MaterialData;
};

