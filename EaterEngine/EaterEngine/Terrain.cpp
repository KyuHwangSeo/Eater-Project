#include "Material.h"
#include "Terrain.h"
#include "LoadManager.h"
#include "EngineData.h"
#include "GameObject.h"

Terrain::Terrain()
{

}

Terrain::~Terrain()
{

}

void Terrain::AddLayer(std::string diffuseName, std::string normalName)
{
	// 새로운 Material 생성..
	Material* newMaterial = new Material();

	// 해당 Material Buffer 추출..
	MaterialBuffer* materialBuffer = newMaterial->GetMaterialData();

	// Material Layer Texture 삽입..
	materialBuffer->Albedo = LoadManager::GetTexture(diffuseName);
	materialBuffer->Normal = LoadManager::GetTexture(normalName);

	// 해당 Material Layer 삽입..
	gameobject->OneMeshData->Material_List.push_back(materialBuffer);
	m_MaterialLayer.push_back(newMaterial);
}

void Terrain::SetTextureTiling(float scale)
{
	gameobject->OneMeshData->mTexTM = DirectX::SimpleMath::Matrix::CreateScale(scale, scale, 1.0f);
}

void Terrain::SetTextureTiling(float scale_x, float scale_y)
{
	gameobject->OneMeshData->mTexTM = DirectX::SimpleMath::Matrix::CreateScale(scale_x, scale_y, 1.0f);
}
