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
	//Component Terrain���� ���� ����Ǿ����
	SetUp_Order = FUNCTION_ORDER_FIRST;
	Start_Order = FUNCTION_ORDER_FIRST;
}

MeshFilter::~MeshFilter()
{

}

void MeshFilter::Start()
{
	//Ŭ���̾�Ʈ�ʿ��� �ؽ����� �̸��� �ְ� �ִϸ��̼��� �ְ� ��� ��������
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
	// �⺻���� MeshFilter ������ Material�� ����..
	Materials = new Material();

	// ������Ʈ ���� �� �߰� �۾�
	Materials->SetMeshData(gameobject->OneMeshData);
}

void MeshFilter::PushModelData(LoadMeshData* mModel)
{
	MeshData* data = gameobject->OneMeshData;
	MaterialData* mat = data->Material_Data;

	// Mesh Model Data ����..
	data->IB = mModel->IB;
	data->VB = mModel->VB;

	data->mLocal = mModel->LocalTM;
	data->mWorld = mModel->WorldTM;

	mat->Albedo = mModel->Albedo;
	mat->Normal = mModel->Normal;

	// �ε�� Diffuse Map�� ������� Dump Map���� ��ü..
	if (mat->Albedo == nullptr)
	{
		mat->Albedo = LoadManager::GetTexture("Dump");
	}

}

void MeshFilter::CheckTexture()
{
	// ������ Texture�� ������� �ε��� Diffuse ��� �ش� Texture ����..
	if (TextureName.empty() == false)
	{
		// �ش� Object Mesh Data..

		for (int i = 0; i <(int)MeshList.size(); i++)
		{
			// ���� Mesh Data..
			MeshData* data = MeshList[i]->OneMeshData;

			// ���� Mesh�� Material..
			MaterialData* material = data->Material_Data;

			// ���� Texture Buffer..
			TextureBuffer* texBuffer = LoadManager::GetTexture(TextureName);

			// �ش� Texture�� Load���� ���� ��� ���� Texture ���..
			if (texBuffer == nullptr)
			{
				texBuffer = LoadManager::GetTexture("Dump");
			}

			// Texture ����..
			material->Albedo = texBuffer;
		}
	}
}

void MeshFilter::CheckAnimation()
{
	if (isLoad_Animation == false) { return; }
	
	ModelAnimationData* data		= LoadManager::GetAnimation(AnimationName);
	AnimationController* Controller = gameobject->GetComponent<AnimationController>();
	
	//������ ���۳�Ʈ�� �� ������ �Ѱ��ش�
	if (Controller != nullptr)
	{	
		Controller->SetBoneList(&BoneList);
		Controller->SetAnimeList(data);
	}
}

void MeshFilter::CreateChild_Mesh(LoadMeshData* data, Transform* parent, ModelData* modeldata)
{
	DebugManager::Line("(Mesh)");

	///���� ������Ʈ ����
	GameObject* OBJ		= gameobject;
	Transform*	Tr		= nullptr;
	MeshFilter* Filter	= nullptr;

	/// Model�� �Ѱ� �̻��ϰ�� �� ������Ʈ�� �׷�ȭ
	if (data->Child.size() > 0 || data->MeshType == SKIN_MESH)
	{
		OBJ = new GameObject();
		MeshList.push_back(OBJ);

		///������Ʈ�� ���� ��������� ���Ӱ� ���۳�Ʈ�� ����
		Tr		= OBJ->AddComponent<Transform>();
		Filter	= OBJ->AddComponent<MeshFilter>();
		gameobject->PushChildList(OBJ);
	}
	else
	{
		///���۳�Ʈ ��������
		Tr		= OBJ->GetComponent<Transform>();
		Filter	= OBJ->GetComponent<MeshFilter>();
		MeshList.push_back(OBJ);
	}
	Material* Mat		= Filter->Materials;

	///��Ű�� ������Ʈ ����
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

	///�⺻ ������ ����
	OBJ->Name = data->Name;
	OBJ->transform = Tr;

	Tr->Load_Local = data->LocalTM;
	Tr->Load_World = data->WorldTM;

	///�� ������ ����
	Mat->PushMaterialData(data);
	Filter->PushModelData(data);


	///��� �Լ�
	int ChildCount = (int)data->Child.size();
	for (int i = 0; i < ChildCount; i++)
	{
		Filter->CreateChild_Mesh(data->Child[i], Tr, modeldata);
	}
}

void MeshFilter::CreateChild_Bone(LoadMeshData* data, Transform* parent, std::vector<GameObject*>* mBoneList, std::vector<DirectX::SimpleMath::Matrix>* BoneOffsetList)
{
	DebugManager::Line("(Bone)");

	///������Ʈ�� ����
	GameObject* OBJ = new GameObject();

	///���۳�Ʈ ����
	Transform*	Tr	= OBJ->AddComponent<Transform>();
	MeshFilter* MF	= OBJ->AddComponent<MeshFilter>();
	Animator*	AN	= OBJ->AddComponent<Animator>();

	///�ʱ�ȭ ����
	if (data->Top_Object == true){	Tr->Rotation = { -90,0,0 };	}
	OBJ->OneMeshData->ObjType = OBJECT_TYPE::BONE;
	OBJ->transform	= Tr;
	OBJ->Name		=  data->Name;
	Tr->Load_Local	= data->LocalTM;
	Tr->Load_World	= data->WorldTM;
	gameobject->PushChildBoneObject(OBJ);
	LinkHierarchy(Tr, parent);

	//���� �ش��ϴ� Transform�� ���������־��ش�
	(*mBoneList)[data->BoneIndex] = OBJ;
	(*BoneOffsetList)[data->BoneIndex] = (*data->BoneOffset);


	///��� �Լ�
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
	///�̸����� �ε��� �����͸� ã�Ƽ� ������
	ModelData* data = LoadManager::GetMesh(MeshName);
	Transform* Tr = gameobject->GetTransform();

	if (data == nullptr) { return; }
	
	///�� ������Ʈ ����
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

	///�޽� ������Ʈ ����
	index = (int)data->TopMeshList.size();
	for (int i = 0; i < index; i++)
	{
		CreateChild_Mesh(data->TopMeshList[i], Tr, data);
	}

	///������Ʈ �����Ϸ�
	isLoad_Mesh = true;
}
