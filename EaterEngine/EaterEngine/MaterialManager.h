#pragma once
#include <unordered_map>
#include "BaseManager.h"
#include "LightHelper.h"

class Material;
class MaterialManager : public BaseManager
{
public:
	MaterialManager();
	~MaterialManager();

public:
	void Initialize();
	void Release();

	void AddMaterial(Material* mat);

private:
	std::unordered_map<int, Material*> m_MaterialList;
	
	UINT m_MaterialCount;
};

