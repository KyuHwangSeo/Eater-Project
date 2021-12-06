#include "MeshFilter.h"
#include "DebugManager.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
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

	//모든 컨퍼넌트들 Start함수보다 나중에 실행될것이다
	Start_Order = FUNCTION_ORDER_LAST;
}

MeshFilter::~MeshFilter()
{

}

void MeshFilter::Start()
{
	//클라이언트쪽에서 텍스쳐의 이름을 넣고 애니메이션을 넣고 모두 끝난상태
	if (isLoad_Texture == true) 
	{
		CheckTexture();
	}

	if (isLoad_Animation == true)
	{
		CheckAnimation();
	}
}

void MeshFilter::SetManager(ObjectManager* obj, MaterialManager* mat)
{
	OBJ_Manager = obj;
	MAT_Manager = mat;
}

void MeshFilter::SetMeshName(std::string mMeshName)
{
	isLoad_Mesh = true;
	MeshName = mMeshName;
	CreateMesh();
}

void MeshFilter::SetTextureName(std::string mTextureName)
{
	isLoad_Texture = true;
	TextureName = mTextureName;
}

void MeshFilter::SetNormalTextureName(std::string mTextureName)
{


}

void MeshFilter::SetAnimationName(std::string mAnimeName)
{
	isLoad_Animation = true;
	AnimationName = mAnimeName;
	CheckAnimation();
}

void MeshFilter::PushModelData(LoadMeshData* mModel)
{
	MeshData* data = gameobject->OneMeshData;

	data->IB = mModel->IB;
	data->VB = mModel->VB;

	data->Diffuse	= mModel->Diffuse;
	data->Normal	= mModel->Normal;

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

		for (int i = 0; i <(int)MeshList.size(); i++)
		{
			MeshData* data = MeshList[i]->OneMeshData;

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
}

void MeshFilter::CheckAnimation()
{
	if (isLoad_Animation == false) { return; }
	
	ModelAnimationData* data		= LoadManager::GetAnimation(AnimationName);
	AnimationController* Controller = gameobject->GetComponent<AnimationController>();
	
	//가져온 컨퍼넌트에 본 정보를 넘겨준다
	if (Controller != nullptr)
	{	
		Controller->SetBoneList(&BoneList);
		Controller->SetAnimeList(data);
	}
}

void MeshFilter::CreateChild_Mesh(LoadMeshData* data, Transform* parent, ModelData* modeldata)
{
	DebugManager::Line("(Mesh)");

	///게임 오브젝트 생성
	GameObject* OBJ = new GameObject();

	///컨퍼넌트 생성후 초기화
	Transform* Tr		= OBJ->AddComponent<Transform>();
	MeshFilter* Filter	= OBJ->AddComponent<MeshFilter>();

	///스키닝 오브젝트 여부
	if (data->Skinning_Object == true)
	{
		SkinningFilter* SF = OBJ->AddComponent<SkinningFilter>();
		SF->PushBoneList(&BoneList);
		SF->PushBone_OffsetList(&BoneOffsetList);
		OBJ->OneMeshData->ObjType = OBJECT_TYPE::SKINNING;
		Tr->Rotation = { 180,0,0 };
	}
	else
	{
		LinkHierarchy(Tr, parent);
		OBJ->OneMeshData->ObjType = OBJECT_TYPE::BASE;
	}

	///메테리얼 정보 여부
	if (data->Material)
	{
		Material* mat = OBJ->AddComponent<Material>();
		MaterialData matData;
		matData.Ambient		= data->Material->m_Material_Ambient;
		matData.Diffuse		= data->Material->m_Material_Diffuse;
		matData.Specular	= data->Material->m_Material_Specular;
		mat->SetMaterialData(matData);	// 해당 Material 삽입..
		MAT_Manager->AddMaterial(mat);	// Material 등록..
	}

	///기본 데이터 초기화
	OBJ->Name = data->Name;
	OBJ->transform = Tr;
	Filter->PushModelData(data);
	Tr->Load_Local = *data->LocalTM;
	Tr->Load_World = *data->WorldTM;
	OBJ_Manager->PushCreateObject(OBJ);
	MeshList.push_back(OBJ);

	///재귀 함수
	int ChildCount = (int)data->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		Filter->CreateChild_Mesh(data->Child[i], Tr, modeldata);
	}
}

void MeshFilter::CreateChild_Bone(LoadMeshData* data, Transform* parent, std::vector<GameObject*>* mBoneList, std::vector<DirectX::SimpleMath::Matrix>* BoneOffsetList)
{
	DebugManager::Line("(Bone)");

	///오브젝트를 생성
	GameObject* OBJ = new GameObject();

	///컨퍼넌트 생성
	Transform*	Tr	= OBJ->AddComponent<Transform>();
	MeshFilter* MF	= OBJ->AddComponent<MeshFilter>();
	Animator*	AN	= OBJ->AddComponent<Animator>();

	///초기화 설정
	if (data->Top_Object == true){Tr->Rotation = { -90,0,0 };}
	OBJ->OneMeshData->ObjType = OBJECT_TYPE::BONE;
	OBJ->transform	= Tr;
	OBJ->Name		=  data->Name;
	Tr->Load_Local	= *data->LocalTM;
	Tr->Load_World	= *data->WorldTM;
	gameobject->PushChildBoneObject(OBJ);
	OBJ_Manager->PushCreateObject(OBJ);
	LinkHierarchy(Tr, parent);
	//본에 해당하는 Transform과 오프셋을넣어준다
	(*mBoneList)[data->BoneIndex] = OBJ;
	(*BoneOffsetList)[data->BoneIndex] = (*data->BoneOffset);


	///재귀 함수
	int ChildCount = (int)data->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		MF->CreateChild_Bone(data->Child[i], Tr, mBoneList, BoneOffsetList);
	}
}

void MeshFilter::LinkHierarchy(Transform* my, Transform* parent)
{
	my->SetParnet(parent);
	parent->SetChild(my);
}

void MeshFilter::CreateMesh()
{
	///이름으로 로드할 데이터를 찾아서 가져옴
	ModelData* data = LoadManager::GetMesh(MeshName);
	Transform* Tr = gameobject->GetTransform();
	if (data == nullptr) { return; }

	///본 오브젝트 생성
	int index = 0;
	index = (int)data->TopBoneList.size();
	if (data->BoneList != nullptr)
	{
		BoneList.resize((int)data->BoneList->size());
		BoneOffsetList.resize((int)data->BoneOffsetList->size());

		for (int i = 0; i < index; i++)
		{
			CreateChild_Bone(data->TopBoneList[i], Tr, &BoneList, &BoneOffsetList);
		}
	}

	///메쉬 오브젝트 생성
	index = (int)data->TopMeshList.size();
	for (int i = 0; i < index; i++)
	{
		CreateChild_Mesh(data->TopMeshList[i], Tr, data);
	}

	///오브젝트 생성완료
	isLoad_Mesh = true;
}




