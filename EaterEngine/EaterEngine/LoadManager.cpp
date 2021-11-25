#include "LoadManager.h"
#include "DebugManager.h"
#include "EngineData.h"
#include "GraphicsEngine.h"
#include "ModelParser.h"
#include "GraphicEngineManager.h"

using namespace ParserData;

std::map<std::string, ModelData*>			LoadManager::ModelList;
std::map<std::string, TextureBuffer*>		LoadManager::TextureList;
std::map<std::string, ModelAnimationData*>	LoadManager::AnimationList;
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
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_GET,"Mesh",Name,true);
		return nullptr;
	}
	else
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_GET, "Mesh", Name, false);
		return temp->second;
	}

}

TextureBuffer* LoadManager::GetTexture(std::string Name)
{
	std::map<std::string, TextureBuffer*>::iterator temp = TextureList.find(Name);
	if (temp == TextureList.end())
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_GET, "Texture", Name, true);
		return nullptr;
	}
	else
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_GET, "Texture", Name, false);
		return temp->second;
	}
}

ModelAnimationData* LoadManager::GetAnimation(std::string Name)
{
	if (AnimationList.find(Name) != AnimationList.end())
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_GET, "Animation", Name, false);
		return AnimationList[Name];
	}
	else
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_GET, "Animation", Name,true);
		return nullptr;
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
	if (temp == nullptr)
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "Mesh", Name, true);
	}
	else
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "Mesh", Name, false);
	}


	//애니메이션 데이터 넣어줌
	LoadAnimation(SaveMesh,temp, Name);

	//애니메이션 정보만 읽을거면 여기서 종료
	if (LoadAnime == true) { return; }

	

	//본오프셋 TM과 본리스트를 먼저읽어오기위해 
	int MeshCount = (int)temp->m_MeshList.size();
	for (int i = 0; i < MeshCount; i++)
	{
		//리스트에 매쉬 조사
		ParserData::Mesh* mesh = temp->m_MeshList[i];

		//최상위 매쉬가 아니라면 아무것도 하지않음
		if (mesh->m_TopNode != true) { continue; }

		//매쉬이고 스키닝 오브젝트라면
		if (mesh->m_IsBone == false && mesh->m_IsSkinningObject == true)
		{
			//본과 오프셋 정보만 읽어옴 생성은 밑에서 
			SaveMesh->BoneOffsetList	= &mesh->m_BoneTMList;
			SaveMesh->BoneList			= &mesh->m_BoneMeshList;
		}

		//매쉬이고 탑 오브젝트라면
		if (mesh->m_IsBone == false && mesh->m_TopNode)
		{
			LoadMeshData* TopMesh = CreateMeshObjeect(mesh);
			SaveMesh->TopMeshList.push_back(TopMesh);
		}
	}
	

	////읽어온 BoneOffset 과 리스트를 기반으로 생성하고 계층구조 연결후 최상위만뽑아온다
	LoadMeshData* TopBone = CreateBoneObjeect(SaveMesh);
	if (TopBone != nullptr) { SaveMesh->TopBoneList.push_back(TopBone); }


	//최상위 오브젝트의 리스트를 넣어준다
	ModelList.insert({ Name,SaveMesh });
}

void LoadManager::LoadTexture(std::string Name)
{
	std::string TextureName = TexturePath + Name;


	TextureBuffer* Tbuffer = GEngine->CreateTextureBuffer(TextureName);
	
	if (Tbuffer == nullptr || Tbuffer->TextureBufferPointer == nullptr)
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "Texture", Name, true);
	}
	else
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "Texture", Name, false);
		//이름은 .dds , .png 를 빼고 저장한다
		std::string::size_type End = Name.rfind('.');
		std::string::size_type start = 0;
		std::string SaveName = Name.substr(start, End);

		TextureList.insert({ SaveName,Tbuffer });
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

	//자식객체가 있다면 정보읽어옴
	int ChildCount = (int)mesh->m_ChildList.size();
	for (int i = 0; i < ChildCount; i++)
	{
		LoadMeshData* temp = CreateMeshObjeect(mesh->m_ChildList[i]);
		box->Child.push_back(temp);
		temp->Parent = box;
	}

	return box;
}

LoadMeshData* LoadManager::CreateBoneObjeect(ModelData* SaveData)
{
	///읽어온 본 리스트로 본을 생성하고 BoneOffset과 연결해줌
	if (SaveData->BoneList == nullptr) { return nullptr; }

	//본의 개수만큼 본을 생성시킴
	int boneCount = (int)SaveData->BoneList->size();

	//크기지정
	std::vector<LoadMeshData*> TempBoneList;
	TempBoneList.resize(boneCount);

	for (int i = 0; i < boneCount; i++)
	{
		ParserData::Mesh* mesh = (*SaveData->BoneList)[i];
		if (mesh == nullptr) { continue; }

		LoadMeshData* data = new LoadMeshData();
		TempBoneList[i] = data;
		SetData(data, mesh);
		data->BoneOffset = &(*SaveData->BoneOffsetList)[i];
		data->BoneList	 = &(SaveData->BoneList)[i];
	}

	///생성된 본들을 부모자식관계로 링크시켜줌
	LoadMeshData* TOP_BONE = nullptr;
	for (int i = 0; i < boneCount; i++)
	{
		for (int j = 0; j < boneCount; j++)
		{
			//자기 자신과는 검사하지않는다
			if (TempBoneList[i] == TempBoneList[j]) { continue; }
			//없는거와 비교하지않음
			if (TempBoneList[i] == nullptr || TempBoneList[j] == nullptr) { continue; }

			LoadMeshData* Front = TempBoneList[i];
			LoadMeshData* Back = TempBoneList[j];
			//나의 부모이름이 자기자신의 이름일때 연결
			if (Front->ParentName == Back->Name)
			{
				Front->Parent = Back;
				Back->Child.push_back(Front);
			}

			if (Front->Top_Object == true)
			{
				TOP_BONE = Front;
			}
		}
	}

	///모두 연결된 상태에서 최상위 오브젝트만 내보내준다
	return TOP_BONE;
}

void LoadManager::LoadAnimation(ModelData* SaveMesh, ParserData::Model* MeshData, std::string Name)
{
	if (MeshData->m_isAnimation == false)
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "Animation", Name, true);
		return;
	}
	else
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "Animation", Name, false);
	}

	//저장할 이름 설정 첫번째 key
	//들어온 string을 맨앞부터"_"까지만읽는다
	std::string::size_type start = 0;
	std::string::size_type End = Name.rfind('_');
	std::string SaveName = Name.substr(start, End);
	//Enemy_Run 이라면 Enemy가 첫번째 키 

	if (AnimationList.find(SaveName) == AnimationList.end())
	{
		//찾는 이름이 없다면 만들어준다
		ModelAnimationData* data = new ModelAnimationData();
		AnimationList.insert({ SaveName,data});
	}

	//저장할 이름 설정 두번째 key
	//"_"부터 string 끝까지읽음
	//Enemy_Run 이라면 Run 두번째 키 
	start = End+1;
	End = Name.length();
	std::string key = Name.substr(start, End);


	//데이터 저장
	ModelAnimationData* temp = AnimationList[SaveName];
	temp->AnimList.insert({key, &(MeshData->m_AnimationList)});
}

void LoadManager::SetData(LoadMeshData* MeshData, ParserData::Mesh* LoadData)
{
	MeshData->Name				= LoadData->m_NodeName;
	MeshData->ParentName		= LoadData->m_ParentName;
	MeshData->Top_Object		= LoadData->m_TopNode;
	MeshData->Bone_Object		= LoadData->m_IsBone;
	MeshData->Skinning_Object	= LoadData->m_IsSkinningObject;
	MeshData->BoneIndex			= LoadData->m_BoneIndex;

	//기존 데이터 그냥 읽어옴
	MeshData->Animation = LoadData->m_Animation;
	MeshData->Material = LoadData->m_MaterialData;
	
	// Texture Buffer 삽입..
	CMaterial* mat = LoadData->m_MaterialData;
	if (mat != nullptr)
	{
		if (mat->m_IsDiffuseMap == true)
		{
			MeshData->Diffuse = GEngine->CreateTextureBuffer(mat->m_DiffuseMap->m_BitMap);
		}
		if (mat->m_IsBumpMap == true)
		{
			MeshData->Normal = GEngine->CreateTextureBuffer(mat->m_BumpMap->m_BitMap);
		}
	}

	//매트릭스 정보 받기
	MeshData->WorldTM = &LoadData->m_WorldTM;
	MeshData->LocalTM = &LoadData->m_LocalTM;
}











