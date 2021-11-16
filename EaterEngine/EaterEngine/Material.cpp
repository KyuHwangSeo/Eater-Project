#include "Material.h"
#include "GameObject.h"
#include "Transform.h"
#include "EngineData.h"

Material::Material()
{

}

Material::~Material()
{

}

void Material::SetMaterialIndex(UINT index)
{
	// 해당 Material Index 삽입..
	m_Material_Index = index;

	// Object 연동..
	gameobject->OneMeshData->Material_Index = index;
}

UINT Material::GetMaterialIndex()
{
	return m_Material_Index;
}

void Material::SetMaterialData(MaterialData matData)
{
	m_MaterialData = matData;
}

MaterialData Material::GetMaterialData()
{
	return m_MaterialData;
}
