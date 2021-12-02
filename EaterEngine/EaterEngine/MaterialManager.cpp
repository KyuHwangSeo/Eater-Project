#include "GameObject.h"
#include "Transform.h"
#include "Material.h"
#include "MaterialManager.h"
#include "EngineData.h"



MaterialManager::MaterialManager()
	:m_MaterialCount(0)
{

}

MaterialManager::~MaterialManager()
{

}

void MaterialManager::Initialize()
{
	// Basic Material..
	MaterialData mat;
	mat.Ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	mat.Diffuse = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	mat.Specular = Vector4(0.4f, 0.4f, 0.4f, 1.0f);
	mat.Reflect = Vector4(0.4f, 0.4f, 0.4f, 1.0f);

	Global->mMatData[m_MaterialCount++] = mat;
}

void MaterialManager::Release()
{
	m_MaterialList.clear();
}

void MaterialManager::AddMaterial(Material* mat)
{
	// 동일한 Material 체크
	for (auto& matData : m_MaterialList)
	{
		Material* material = matData.second;
		if (material->GetMaterialData() == mat->GetMaterialData())
		{
			mat->SetMaterialIndex(material->GetMaterialIndex());
			return;
		}
	}

	// Material 추가 될때마다 Global Data 설정..
	Global->mMatData[m_MaterialCount] = mat->GetMaterialData();

	// Material List 추가..
	m_MaterialList.insert(std::make_pair(m_MaterialCount++, mat));
}
