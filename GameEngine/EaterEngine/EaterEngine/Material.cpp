#include "Material.h"
#include "EngineData.h"
#include "MaterialManager.h"
#include "LoadManager.h"


Material::Material()
{
	// Material Buffer 생성..
	MaterialDatas = new MaterialData();

	// Material Data 생성..
	MaterialDatas->Material_Option = new MaterialOption();

	// Material 등록..
	MaterialManager::PushMaterial(this);
}

Material::~Material()
{
	Release();
}

void Material::SetMeshData(MeshData* meshData)
{
	// 해당 Object Material Data 삽입..
	meshData->Material_Data = MaterialDatas;

	m_MeshData = meshData;
}

void Material::SetMaterialIndex(UINT index)
{
	// 해당 Material Index 삽입..
	MaterialDatas->Material_Index = index;
}

void Material::PushMaterialData(LoadMeshData* mesh)
{
	// Load한 Material Data가 없을경우..
	if (mesh->Material == nullptr) return;

	// Material Data 추출..
	MaterialOption* matData = MaterialDatas->Material_Option;

	// Material Data 삽입..
	matData->Ambient = mesh->Material->m_Material_Ambient;
	matData->Diffuse = mesh->Material->m_Material_Diffuse;
	matData->Specular = mesh->Material->m_Material_Specular;
	matData->Ambient = mesh->Material->m_Material_Ambient;

	// Texture Map 삽입..
	MaterialDatas->Albedo = mesh->Albedo;
	MaterialDatas->Normal = mesh->Normal;
}

void Material::SetTexTransform(DirectX::SimpleMath::Vector3 scale)
{

}

void Material::SetTexTransform(float x, float y, float z)
{

}

void Material::SetDiffuseMap(std::string diffuseName)
{
	MaterialDatas->Albedo = LoadManager::GetTexture(diffuseName);
}

void Material::SetNormalMap(std::string noramlName)
{
	MaterialDatas->Albedo = LoadManager::GetTexture(noramlName);
}

void Material::SetBaseColor(DirectX::SimpleMath::Vector4 color)
{
	MaterialDatas->Color_Base = color;
}

void Material::SetStartColor(DirectX::SimpleMath::Vector4 color)
{
	MaterialDatas->Color_Add = color;
}

void Material::Release()
{
	// Manager 내부에 있는 해당 Material Data 삭제..
	MaterialManager::DeleteMaterial(MaterialDatas->Material_Index);
	
	// 해당 Material Data 해제..
	delete MaterialDatas->Material_Option;
	delete MaterialDatas;
}

MaterialData* Material::GetMaterialData()
{
	return MaterialDatas;
}

UINT Material::GetMaterialIndex()
{
	return MaterialDatas->Material_Index;
}