#include "Material.h"
#include "MaterialManager.h"
#include "EngineData.h"

std::unordered_map<UINT, MaterialOption*> MaterialManager::g_MaterialList;

MaterialManager::MaterialManager()
{

}

MaterialManager::~MaterialManager()
{

}

void MaterialManager::Initialize()
{
	// Basic Material..
	MaterialOption* mat = new MaterialOption();

	Global->mMatData.push_back(mat);

	UINT material_Index = (UINT)g_MaterialList.size();

	// Material List �߰�..
	g_MaterialList.insert(std::make_pair(material_Index, mat));
}

void MaterialManager::Release()
{
	g_MaterialList.clear();
}

void MaterialManager::PushMaterial(Material* mat)
{
	// ������ Material üũ
	for (auto& matData : g_MaterialList)
	{
		MaterialOption* material = matData.second;

		// �ش� Material Data�� List�� �ö��ִ��� üũ..
		if (*material == *mat->MaterialDatas->Material_Option)
		{
			mat->SetMaterialIndex(matData.first);
			return;
		}
	}

	// Material �߰� �ɶ����� Global Data ����..
	MaterialData* matBuf = mat->MaterialDatas;

	UINT material_Index = (UINT)g_MaterialList.size();

	// ���� Material Index ����..
	matBuf->Material_Index = material_Index;
	
	// ���� Material Data ����..
	Global->mMatData.push_back(matBuf->Material_Option);

	// Material List �߰�..
	g_MaterialList.insert(std::make_pair(material_Index, matBuf->Material_Option));
}
