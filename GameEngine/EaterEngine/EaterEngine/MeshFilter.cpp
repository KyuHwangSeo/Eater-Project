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
#include "Terrain.h"


MeshFilter::MeshFilter()
{
	MeshName = "";
	//Component Terrain보다 먼저 실행되어야함
	SetUp_Order = FUNCTION_ORDER_FIRST;
	Start_Order = FUNCTION_ORDER_FIRST;
}

MeshFilter::~MeshFilter()
{

}

void MeshFilter::Start()
{
	//클라이언트쪽에서 텍스쳐의 이름을 넣고 애니메이션을 넣고 모두 끝난상태
	if (isLoad_Mesh == true)
	{
		CreateMesh();
	}

	if (isLoad_Texture == true) 
	{
		CheckTexture();
	}

	if (isLoad_Animation == true)
	{
		CheckAnimation();
	}
}

void MeshFilter::SetMeshName(std::string mMeshName)
{
	isLoad_Mesh = true;
	MeshName = mMeshName;
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
	//CheckAnimation();
}

void MeshFilter::SetObjectData()
{
	// 기본으로 MeshFilter 생성시 Material을 보유..
	Materials = new Material();

	// 오브젝트 설정 후 추가 작업
	Materials->SetMeshData(gameobject->OneMeshData);
}

void MeshFilter::PushModelData(LoadMeshData* mModel)
{
	MeshData* data = gameobject->OneMeshData;
	MaterialData* mat = data->Material_Data;

	// Mesh Model Data 삽입..
	data->IB = mModel->IB;
	data->VB = mModel->VB;

	data->mLocal = mModel->LocalTM;
	data->mWorld = mModel->WorldTM;

	mat->Albedo = mModel->Albedo;
	mat->Normal = mModel->Normal;

	// 로드된 Diffuse Map이 없을경우 Dump Map으로 대체..
	if (mat->Albedo == nullptr)
	{
		mat->Albedo = LoadManager::GetTexture("Dump");
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
			// 현재 Mesh Data..
			MeshData* data = MeshList[i]->OneMeshData;

			// 현재 Mesh의 Material..
			MaterialData* material = data->Material_Data;

			// 설정 Texture Buffer..
			TextureBuffer* texBuffer = LoadManager::GetTexture(TextureName);

			// 해당 Texture가 Load되지 않은 경우 기존 Texture 사용..
			if (texBuffer == nullptr)
			{
				texBuffer = LoadManager::GetTexture("Dump");
			}

			// Texture 설정..
			material->Albedo = texBuffer;
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
	GameObject* OBJ		= gameobject;
	Transform*	Tr		= nullptr;
	MeshFilter* Filter	= nullptr;

	/// Model이 한개 이상일경우 빈 오브젝트로 그룹화
	if (data->Child.size() > 0 || data->MeshType == SKIN_MESH)
	{
		OBJ = new GameObject();
		MeshList.push_back(OBJ);

		///오브젝트를 새로 만들었으니 새롭게 컨퍼넌트를 생성
		Tr		= OBJ->AddComponent<Transform>();
		Filter	= OBJ->AddComponent<MeshFilter>();
		gameobject->PushChildList(OBJ);
	}
	else
	{
		///컨퍼넌트 가져오기
		Tr		= OBJ->GetComponent<Transform>();
		Filter	= OBJ->GetComponent<MeshFilter>();
		MeshList.push_back(OBJ);
	}
	Material* Mat		= Filter->Materials;

	///스키닝 오브젝트 여부
	switch (data->MeshType)
	{
		case MESH_TYPE::SKIN_MESH:
		{
			SkinningFilter* SF = OBJ->AddComponent<SkinningFilter>();
			SF->PushBoneList(&BoneList);
			SF->PushBone_OffsetList(&BoneOffsetList);
			OBJ->OneMeshData->ObjType = OBJECT_TYPE::SKINNING;
			Tr->Rotation = { -90,0,90 };
			break;
		}
		case MESH_TYPE::TERRAIN_MESH:
		{
			OBJ->OneMeshData->ObjType = OBJECT_TYPE::TERRAIN;
			break;
		}
		default:
		{
			if (Tr != parent)
			{
				LinkHierarchy(Tr, parent);
			}
			OBJ->OneMeshData->ObjType = OBJECT_TYPE::BASE;
		break;
		}
	}

	///기본 데이터 설정
	OBJ->Name = data->Name;
	OBJ->transform = Tr;

	Tr->Load_Local = data->LocalTM;
	Tr->Load_World = data->WorldTM;

	///모델 데이터 설정
	Mat->PushMaterialData(data);
	Filter->PushModelData(data);


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
	if (data->Top_Object == true){	Tr->Rotation = { -90,0,0 };	}
	OBJ->OneMeshData->ObjType = OBJECT_TYPE::BONE;
	OBJ->transform	= Tr;
	OBJ->Name		=  data->Name;
	Tr->Load_Local	= data->LocalTM;
	Tr->Load_World	= data->WorldTM;
	gameobject->PushChildBoneObject(OBJ);
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
	my->SetParent(parent);
	parent->SetChild(my);
}

void MeshFilter::CreateMesh()
{
	///이름으로 로드할 데이터를 찾아서 가져옴
	ModelData* data = LoadManager::GetMesh(MeshName);
	Transform* Tr = gameobject->GetTransform();

	if (data == nullptr) { return; }
	
	///본 오브젝트 생성
	int index = (int)data->TopBoneList.size();
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
