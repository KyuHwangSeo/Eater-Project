#include "Material.h"
#include "Terrain.h"
#include "LoadManager.h"
#include "EngineData.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "MeshFilter.h"

Terrain::Terrain()
{
	// Terrain ���� ������ ����..
	m_TerrainData = new TerrainData();

	//�����۳�Ʈ�� Start�Լ��� ���ϳ��߿� ����
	Component::Start_Order = Component::FUNCTION_ORDER_LAST;
}

Terrain::~Terrain()
{

}

void Terrain::Awake()
{
	//Awake ������ ������ GetComponent ��
	mRigidbody	= gameobject->GetComponent<Rigidbody>();
	mMeshFilter = gameobject->GetComponent<MeshFilter>();
}

void Terrain::SetUp()
{
	//�ʱ�ȭ ���� �־��ش�
	if (TerrainColliderName != "" && mRigidbody != nullptr)
	{
		mRigidbody->CreateTriangleCollider(TerrainColliderName);
	}

	if (TerrainMeshName != "" && mMeshFilter != nullptr)
	{
		mMeshFilter->SetMeshName(TerrainMeshName);
	}
}

void Terrain::Start()
{
	//�ʱ�ȭ �־��ذ����� ���۳�Ʈ�� ����
	int count = (int)DiffuseNameList.size();
	for (int i = 0; i < count; i++)
	{
		AddLayer(DiffuseNameList[i],NormalNameList[i]);
	}
}


void Terrain::AddLayer(std::string diffuseName, std::string normalName)
{
	// ���ο� Material ����..
	Material* newMaterial = new Material();

	// ���ο� Material Buffer ����..
	MaterialData* materialBuffer = newMaterial->GetMaterialData();

	// Material Layer Texture ����..
	materialBuffer->Albedo = LoadManager::GetTexture(diffuseName);
	materialBuffer->Normal = LoadManager::GetTexture(normalName);

	// ���ο� Material Layer ����..
	gameobject->OneMeshData->Terrain_Data = m_TerrainData;

	m_TerrainData->Material_List.push_back(materialBuffer);
	m_MaterialLayer.push_back(newMaterial);
}

EATER_ENGINEDLL void Terrain::SetTextureTiling(float scale_x, float scale_y)
{
	gameobject->OneMeshData->mTexTM = DirectX::SimpleMath::Matrix::CreateScale(scale_x, scale_y, 1.0f);
}

EATER_ENGINEDLL void Terrain::SetTextureTiling(float scale)
{
	gameobject->OneMeshData->mTexTM = DirectX::SimpleMath::Matrix::CreateScale(scale, scale, 1.0f);
}

void Terrain::SetLayerName(std::string diffuseName, std::string normalName)
{
	DiffuseNameList.push_back(diffuseName);
	NormalNameList.push_back(normalName);
}

 void Terrain::SetColliderName(std::string _TerrainColliderName)
{
	 TerrainColliderName = _TerrainColliderName;
}

  void Terrain::SetMeshName(std::string _TerrainMeshName)
 {
	  TerrainMeshName = _TerrainMeshName;
 }
