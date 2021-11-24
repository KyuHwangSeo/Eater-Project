#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "GameObject.h"
#include "LoadManager.h"
#include "Transform.h"
#include "EngineData.h"
#include "Animator.h"
#include "ObjectManager.h"
#include "Material.h"
#include "MaterialManager.h"


ObjectManager* MeshFilter::OBJ_Manager = nullptr;
MaterialManager* MeshFilter::MAT_Manager = nullptr;

MeshFilter::MeshFilter()
{
	MeshName = "";
}

MeshFilter::~MeshFilter()
{

}

void MeshFilter::Awake()
{
	if (isLoadMesh == true)
	{
		////최상위 객체를 가져옴
		ModelData* data = LoadManager::GetMesh(MeshName);
		Transform* MyTr = gameobject->transform;
		gameobject->OneMeshData->ObjType = OBJECT_TYPE::GAMEOBJECT;

		
		if(data->TopMeshList[0]->Skinning_Object)
		{
			BoneList.resize(data->BoneList->size());
			BoneOffsetList.resize(data->BoneOffsetList->size());
		}
		
		//본 오브젝트 만들기
		for(int i = 0; i < data->TopBoneList.size(); i++)
		{
			CreateChild_Bone(data->TopBoneList[i], MyTr, &BoneList, &BoneOffsetList);
		}


		//매쉬 오브젝트 만들기
		for(int i = 0; i < data->TopMeshList.size(); i++)
		{
			CreateChild_Mesh(data->TopMeshList[i], MyTr, data);
		}

		//Texture Check & Setting..
		CheckTexture();

	}
}

void MeshFilter::SetManager(ObjectManager* obj, MaterialManager* mat)
{
	OBJ_Manager = obj;
	MAT_Manager = mat;
}

void MeshFilter::SetMeshName(std::string mMeshName)
{
	isLoadMesh = true;
	MeshName = mMeshName;
}

void MeshFilter::SetTextureName(std::string mTextureName)
{
	isLoadTexture = true;
	TextureName = mTextureName;
}

void MeshFilter::SetNormalTextureName(std::string mTextureName)
{


}

void MeshFilter::PushModelData(LoadMeshData* mModel)
{
	MeshData* data = gameobject->OneMeshData;

	data->IB = mModel->IB;
	data->VB = mModel->VB;

	data->Diffuse = mModel->Diffuse;
	data->Normal = mModel->Normal;

	data->mLocal = *(mModel->LocalTM);
	data->mWorld = *(mModel->WorldTM);

	// Diffuse Map이 없는경우 Dump Map으로 기본 출력..
	if (data->Diffuse == nullptr)
	{
		data->Diffuse = LoadManager::GetTexture("Dump");
	}
}

void MeshFilter::CheckTexture()
{
	// 설정한 Texture가 있을경우 로드한 Diffuse 대신 해당 Texture 설정..
	if (TextureName.empty() == false)
	{
		// 해당 Object Mesh Data..
		MeshData* data = gameobject->OneMeshData;

		// 설정 Texture Buffer..
		TextureBuffer* texBuffer = LoadManager::GetTexture(TextureName);

		// 해당 Texture가 Load되지 않은 경우 기존 Texture 사용..
		if (texBuffer == nullptr)
		{
			return;
		}

		// Texture 설정..
		data->Diffuse = texBuffer;
	}
}

void MeshFilter::CreateChild_Mesh(LoadMeshData* data, Transform* parent, ModelData* modeldata)
{
	int ChildCount = (int)data->Child.size();

	GameObject* OBJ = new GameObject();
	OBJ->Name = data->Name;


	//컨퍼넌트 생성
	Transform* Tr = OBJ->AddComponent<Transform>();
	MeshFilter* Filter = OBJ->AddComponent<MeshFilter>();

	//Transform 연결
	OBJ->transform = Tr;

	//스키닝 매쉬라면
	if (data->Skinning_Object == true)
	{
		SkinningFilter* SF = OBJ->AddComponent<SkinningFilter>();
		//본 오프셋 넘겨주기 ,본 리스트 넘겨주기
		SF->PushBoneList(&BoneList);
		SF->PushBone_OffsetList(&BoneOffsetList);

		OBJ->OneMeshData->ObjType = OBJECT_TYPE::SKINNING;
	}
	else
	{
		OBJ->OneMeshData->ObjType = OBJECT_TYPE::BASE;
	}

	// Material Data..
	if (data->Material)
	{
		Material* mat = OBJ->AddComponent<Material>();

		// 해당 Material Data..
		MaterialData matData;
		matData.Ambient = data->Material->m_Material_Ambient;
		matData.Diffuse = data->Material->m_Material_Diffuse;
		matData.Specular = data->Material->m_Material_Specular;

		// 해당 Material 삽입..
		mat->SetMaterialData(matData);

		// Material 등록..
		MAT_Manager->AddMaterial(mat);
	}

	//데이터를 넘겨준다 
	Filter->PushModelData(data);
	Tr->Load_Local = *data->LocalTM;
	Tr->Load_World = *data->WorldTM;

	//Transform 끼리 연결
	//LinkHierarchy(Tr, parent);
	//오브젝트 매니저에서 관리할수있도록 넣어준다
	OBJ_Manager->PushCreateObject(OBJ);

	//자식객체 개수만큼 실행
	for (int i = 0; i < ChildCount; i++)
	{
		//재귀 호출
		Filter->CreateChild_Mesh(data->Child[i], Tr, modeldata);
	}
}

void MeshFilter::CreateChild_Bone(LoadMeshData* data, Transform* parent, std::vector<Transform*>* mBoneList, std::vector<DirectX::SimpleMath::Matrix>* BoneOffsetList)
{
	GameObject* OBJ = new GameObject();
	OBJ->Name = data->Name;
	OBJ->OneMeshData->ObjType = OBJECT_TYPE::BONE;

	//컨퍼넌트 생성
	Transform* Tr		= OBJ->AddComponent<Transform>();
	MeshFilter* Filter	= OBJ->AddComponent<MeshFilter>();

	Animator* Anime = OBJ->AddComponent<Animator>();
	//애니메이션 데이터 넣어주기
	Anime->SetAnimation(data->Animation);

	//Transform 연결
	OBJ->transform = Tr;

	Tr->Load_Local = *data->LocalTM;
	Tr->Load_World = *data->WorldTM;


	//Transform 끼리 연결
	LinkHierarchy(Tr, parent);
	//오브젝트 매니저에서 관리할수있도록 넣어준다
	OBJ_Manager->PushCreateObject(OBJ);

	//본에 해당하는 Transform과 오프셋을
	(*mBoneList)[data->BoneIndex] = Tr;
	(*BoneOffsetList)[data->BoneIndex] = (*data->BoneOffset);

	//자식객체 개수만큼 실행
	int ChildCount = (int)data->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		//재귀 호출
		Filter->CreateChild_Bone(data->Child[i], Tr, mBoneList, BoneOffsetList);
	}
}

void MeshFilter::LinkHierarchy(Transform* my, Transform* parent)
{
	my->SetParnet(parent);
	parent->SetChild(my);
}




