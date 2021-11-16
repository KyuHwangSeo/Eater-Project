#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "GameObject.h"
#include "LoadManager.h"
#include "Transform.h"
#include "EngineData.h"
#include "Animation.h"
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
	if (isLoad == true)
	{
		////최상위 객체를 가져옴
		ModelData* data = LoadManager::GetMesh(MeshName);
		Transform* MyTr = gameobject->transform;



		//본 오브젝트 만들기
		for (int i = 0; i < data->TopBoneList.size(); i++)
		{
			CreateChild_Bone(data->TopBoneList[i], MyTr, &BoneList, &BoneOffsetList);
		}


		//매쉬 오브젝트 만들기
		for (int i = 0; i < data->TopMeshList.size(); i++)
		{
			CreateChild_Mesh(data->TopMeshList[i], MyTr, data);
		}
	}
}

void MeshFilter::SetManager(ObjectManager* obj, MaterialManager* mat)
{
	OBJ_Manager = obj;
	MAT_Manager = mat;
}

void MeshFilter::SetMeshName(std::string mMeshName)
{
	isLoad = true;
	MeshName = mMeshName;
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
}

void MeshFilter::CreateChild_Mesh(LoadMeshData* data, Transform* parent, ModelData* modeldata)
{
	int ChildCount = data->Child.size();

	GameObject* OBJ		= new GameObject();
	OBJ->Name			= data->Name;
	
	
	//컨퍼넌트 생성
	Transform*	Tr		= OBJ->AddComponent<Transform>();
	MeshFilter* Filter	= OBJ->AddComponent<MeshFilter>();
	
	//Transform 연결
	OBJ->transform = Tr;

	//스키닝 매쉬라면
	if (data->Skinning_Object == true)
	{
		SkinningFilter* SF = OBJ->AddComponent<SkinningFilter>();
		//본 리스트 넘겨주기
		SF->PushBoneList(&BoneList);
		//본 오프셋 넘겨주기
		SF->PushBone_OffsetList(&BoneOffsetList);

		OBJ->OneMeshData->ObjType = OBJECT_TYPE::Skinning;
	}
	else
	{
		OBJ->OneMeshData->ObjType = OBJECT_TYPE::Base;
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
	LinkHierarchy(Tr, parent);
	//오브젝트 매니저에서 관리할수있도록 넣어준다
	OBJ_Manager->PushCreateObject(OBJ);

	//자식객체 개수만큼 실행
	for (int i = 0; i < ChildCount; i++)
	{
		//재귀 호출
		Filter->CreateChild_Mesh(data->Child[i],Tr,modeldata);
	}
}

void MeshFilter::CreateChild_Bone(LoadMeshData* data, Transform* parent, std::vector<Transform*>* mBoneList, std::vector<DirectX::SimpleMath::Matrix>* BoneOffsetList)
{
	GameObject* OBJ = new GameObject();
	OBJ->Name = data->Name;
	OBJ->OneMeshData->ObjType = OBJECT_TYPE::Bone;

	//컨퍼넌트 생성
	Transform*	Tr		= OBJ->AddComponent<Transform>();
	MeshFilter* Filter	= OBJ->AddComponent<MeshFilter>();


	//Animation*	Anime	= OBJ->AddComponent<Animation>();
	//애니메이션 데이터 넣어주기
	//Anime->SetAnimation(data->Animation);

	//Transform 연결
	OBJ->transform = Tr;

	Tr->Load_Local = *data->LocalTM;
	Tr->Load_World = *data->WorldTM;


	//Transform 끼리 연결
	LinkHierarchy(Tr, parent);
	//오브젝트 매니저에서 관리할수있도록 넣어준다
	OBJ_Manager->PushCreateObject(OBJ);

	mBoneList->push_back(Tr);
	BoneOffsetList->push_back(*data->BoneOffset);


	//자식객체 개수만큼 실행
	int ChildCount = data->Child.size();
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




