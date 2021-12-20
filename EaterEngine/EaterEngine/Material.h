#pragma once
#include <string>
#include "Component.h"
#include "EaterEngineDLL.h"

class MeshData;
class MaterialData;
class LoadMeshData;
class TextureBuffer;
class MaterialManager;

class Material
{
public:
	Material();
	~Material();

public:
	friend class MaterialManager;

public:
	static void SetManager(MaterialManager* mat);

public:
	void SetMeshData(MeshData* meshData);
	void SetMaterialIndex(UINT index);

	void PushMaterialData(LoadMeshData* mesh);

	void SetTexTransform(DirectX::SimpleMath::Vector3 scale);
	void SetTexTransform(float x, float y, float z);

	void SetDiffuseMap(std::string diffuseName);
	void SetNormalMap(std::string noramlName);

	void SetBaseColor(DirectX::SimpleMath::Vector4 color);
	void SetAddColor(DirectX::SimpleMath::Vector4 color);

public:
	UINT GetMaterialIndex();
	MaterialData* GetMaterialData();

private:
	static MaterialManager* MAT_Manager;

	MeshData* m_MeshData;

	MaterialData* MaterialDatas;
};

