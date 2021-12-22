#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"
#include <vector>
#include <string>

class Rigidbody;
class MeshFilter;
class TerrainData;
class Terrain : public Component
{
public:
	EATER_ENGINEDLL Terrain();
	virtual ~Terrain();

public:
	void Awake() override;
	void SetUp() override;
	void Start() override;

public:
	EATER_ENGINEDLL void SetLayerName(std::string diffuseName, std::string normalName);
	EATER_ENGINEDLL void SetColliderName(std::string TerrainColliderName);
	EATER_ENGINEDLL void SetMeshName(std::string TerrainMeshName);

	EATER_ENGINEDLL void SetTextureTiling(float scale);
	EATER_ENGINEDLL void SetTextureTiling(float scale_x, float scale_y);

private:
	void AddLayer(std::string diffuseName, std::string normalName);

private:
	TerrainData* m_TerrainData;
	std::vector<Material*> m_MaterialLayer;
	
	std::vector<std::string> DiffuseNameList;
	std::vector<std::string> NormalNameList;


	std::string TerrainColliderName;
	std::string TerrainMeshName;

	Rigidbody* mRigidbody;
	MeshFilter* mMeshFilter;
};