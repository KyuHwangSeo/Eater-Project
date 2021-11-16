#include "LoadManager.h"
#include "DebugManager.h"
#include "EngineData.h"
#include "GraphicsEngine.h"
#include "ModelParser.h"
#include "GraphicEngineManager.h"



std::map<std::string, ModelData*> LoadManager::ModelList;
std::map<std::string, TextureBuffer*> LoadManager::TextureList;
LoadManager::LoadManager()
{
	GEngine = nullptr;
	EaterParser = nullptr;
}

LoadManager::~LoadManager()
{

}

void LoadManager::Initialize(GraphicEngineManager* Graphic)
{
	//그래픽 엔진 받아오기
	GEngine = Graphic;
	EaterParser = ModelParser::Create(ModelParser::FBX);

	EaterParser->Initialize();
}

ModelData* LoadManager::GetMesh(std::string Name)
{
	std::map<std::string, ModelData*>::iterator temp = ModelList.find(Name);
	if (temp == ModelList.end())
	{
		DebugManager::Print(Name, 0, 0, DebugManager::MSG_TYPE::MSG_LOAD, true);
		return nullptr;
	}
	else
	{
		return temp->second;
	}

}

TextureBuffer* LoadManager::GetTexture(std::string Name)
{
	std::map<std::string, TextureBuffer*>::iterator temp = TextureList.find(Name);
	if (temp == TextureList.end())
	{
		DebugManager::Print(Name, 0, 0, DebugManager::MSG_TYPE::MSG_LOAD, true);
		return nullptr;
	}
	else
	{
		return temp->second;
	}
}

void LoadManager::LoadMesh(std::string Name, bool Scale, bool LoadAnime)
{
	// "../ 이거와 .fbx 이거를 붙여준다"
	std::string Strtemp = ".fbx";
	std::string FullName = MeshPath + Name + Strtemp;

	ModelData* SaveMesh = new ModelData();

	//파서를 통해서 매쉬를 로드
	ParserData::Model* temp = EaterParser->LoadModel(FullName, Scale, LoadAnime);

	//본오프셋 TM과 본리스트를 먼저읽어오기위해 
	int MeshCount = temp->m_MeshList.size();
	for (int i = 0; i < MeshCount; i++)
	{
		//리스트에 매쉬 조사
		ParserData::Mesh* mesh = temp->m_MeshList[i];
		//최상위 매쉬가 아니라면 아무것도 하지않음
		if (mesh->m_TopNode != true) { continue; }

		//매쉬이고 스키닝 오브젝트라면
		if (mesh->m_IsBone == false && mesh->m_IsSkinningObject == true)
		{
			SaveMesh->BoneOffsetList = &mesh->m_BoneTMList;
			SaveMesh->BoneList = &mesh->m_BoneMeshList;
		}
	}


	for (int i = 0; i < MeshCount; i++)
	{
		//리스트에 매쉬 조사
		ParserData::Mesh* mesh = temp->m_MeshList[i];
		//최상위 매쉬가 아니라면 아무것도 하지않음
		if (mesh->m_TopNode != true) { continue; }


		if (mesh->m_IsBone == true)
		{
			LoadMeshData* temp = CreateBoneObjeect(mesh, SaveMesh);
			SaveMesh->TopBoneList.push_back(temp);
		}
		else
		{
			LoadMeshData* temp = CreateMeshObjeect(mesh);
			SaveMesh->TopMeshList.push_back(temp);
		}
	}


	//최상위 오브젝트의 리스트를 넣어준다
	ModelList.insert({ Name,SaveMesh });
}

void LoadManager::LoadTexture(std::string Name)
{
	std::string TextureName = TexturePath + Name + ".dds";
	TextureBuffer* Tbuffer = GEngine->CreateTextureBuffer(TextureName);

	if (Tbuffer == nullptr || Tbuffer->TextureBufferPointer == nullptr)
	{
		DebugManager::Print(TextureName, 0, 0, DebugManager::MSG_TYPE::MSG_LOAD, true);
	}
	else
	{
		std::string strTemp = "텍스쳐를 로드합니다 :" + Name;
		DebugManager::Print(strTemp, 0, 0, DebugManager::MSG_TYPE::MSG_LOAD);
		TextureList.insert({ Name,Tbuffer });
	}
}

void LoadManager::LoadPrefap(std::string Name)
{
	///나중에 구현
}

void LoadManager::LoadMeshPath(std::string mPath)
{
	//모델 경로 입력
	MeshPath = mPath;
}

void LoadManager::LoadTexturePath(std::string mPath)
{
	//텍스쳐 경로 입력
	TexturePath = mPath;
	ModelParser::SetTextureRoute(TexturePath);
}

void LoadManager::DeleteMesh(std::string mMeshName)
{
	//메모리에 할당한 매쉬의 정보를 삭제시킴

	std::map<std::string, ModelData*>::iterator temp = ModelList.find(mMeshName);
	if (temp == ModelList.end())
	{
		return;
	}
	else
	{
		ModelList.erase(mMeshName);
	}
}

void LoadManager::DeleteMeshAll()
{
	ModelList.clear();
}

LoadMeshData* LoadManager::CreateMeshObjeect(ParserData::Mesh* mesh)
{
	//계층정보 받기
	LoadMeshData* box = new LoadMeshData();

	SetData(box, mesh);

	//버퍼값 읽어오기
	box->IB = GEngine->CreateIndexBuffer(mesh);
	box->IB->Count = (int)mesh->m_IndexList.size() * 3;

	box->VB = GEngine->CreateVertexBuffer(mesh);
	box->VB->Count = (int)mesh->m_VertexList.size();

	// Texture Buffer 삽입..
	CMaterial* mat = mesh->m_MaterialData;

	if (mat)
	{
		if (mat->m_IsDiffuseMap)
		{
			box->Diffuse = GEngine->CreateTextureBuffer(mat->m_DiffuseMap->m_BitMap);
		}
		if (mat->m_IsBumpMap)
		{
			box->Normal = GEngine->CreateTextureBuffer(mat->m_BumpMap->m_BitMap);
		}
	}

	//자식객체가 있다면 정보읽어옴
	int ChildCount = mesh->m_ChildList.size();
	for (int i = 0; i < ChildCount; i++)
	{
		LoadMeshData* temp = CreateMeshObjeect(mesh->m_ChildList[i]);
		box->Child.push_back(temp);
		temp->Parent = box;
	}

	return box;
}

LoadMeshData* LoadManager::CreateBoneObjeect(ParserData::Mesh* mesh, ModelData* SaveData)
{
	LoadMeshData* box = nullptr;
	int size = SaveData->BoneList->size();

	//본리스트에 데이터가들어있지않으면 생성하지않는다
	for (int i = 0; i < size; i++)
	{
		if (mesh == (*SaveData->BoneList)[i])
		{
			//계층정보 받기
			box = new LoadMeshData();
			box->BoneNumber = (*SaveData->BoneList)[i];
			box->BoneOffset = &(*SaveData->BoneOffsetList)[i];

			SetData(box, mesh);

			//자식객체가 있다면 정보읽어옴
			int ChildCount = mesh->m_ChildList.size();
			for (int i = 0; i < ChildCount; i++)
			{
				LoadMeshData* temp = CreateBoneObjeect(mesh->m_ChildList[i], SaveData);

				if (temp != nullptr)
				{
					box->Child.push_back(temp);
					temp->Parent = box;
				}
			}
		}
	}

	return box;
}

void LoadManager::SetData(LoadMeshData* MeshData, ParserData::Mesh* LoadData)
{
	MeshData->Name = LoadData->m_NodeName;
	MeshData->ParentName = LoadData->m_ParentName;
	MeshData->Top_Object = LoadData->m_TopNode;
	MeshData->Bone_Object = LoadData->m_IsBone;
	MeshData->Skinning_Object = LoadData->m_IsSkinningObject;

	//기존 데이터 그냥 읽어옴
	MeshData->Animation = LoadData->m_Animation;
	MeshData->Material = LoadData->m_MaterialData;

	//매트릭스 정보 받기
	MeshData->WorldTM = &LoadData->m_WorldTM;
	MeshData->LocalTM = &LoadData->m_LocalTM;

	MeshData->BoneOffset;
}











