#include "LoadManager.h"
#include "DebugManager.h"
#include "EngineData.h"
#include "GraphicsEngine.h"
#include "ModelParser.h"
#include "MathHelper.h"
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

	// Parser 생성 및 초기화..
	EaterParser = ModelParser::Create(MODEL_TYPE::FBX_MODEL);
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

void LoadManager::LoadTerrain(std::string Name, std::string MaskName, UINT parsingMode)
{
	// "../ 이거와 .fbx 이거를 붙여준다"
	std::string modelExtention = ".fbx";
	std::string modelPath = MeshPath + Name + modelExtention;
	std::string texPath = TexturePath + MaskName;

	ModelData* SaveMesh = new ModelData();

	//파서를 통해서 매쉬를 로드
	ParserData::Model* temp = EaterParser->LoadModel(modelPath, parsingMode);

	if (temp == nullptr)
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "Mesh", Name, true);
	}
	else
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "Mesh", Name, false);
	}

	//본오프셋 TM과 본리스트를 먼저읽어오기위해 
	int MeshCount = (int)temp->m_MeshList.size();
	for (int i = 0; i < MeshCount; i++)
	{
		//리스트에 매쉬 조사
		ParserData::Mesh* mesh = temp->m_MeshList[i];

		//최상위 매쉬가 아니라면 아무것도 하지않음
		if (mesh->m_TopNode != true) { continue; }

		//매쉬이고 탑 오브젝트라면
		if (mesh->m_MeshType != BONE_MESH && mesh->m_TopNode)
		{
			//터레인 매쉬 타입 설정
			mesh->m_MeshType = TERRAIN_MESH;

			LoadMeshData* TopMesh = CreateTerrainObject(mesh, texPath);
			SaveMesh->TopMeshList.push_back(TopMesh);
		}
	}

	//최상위 오브젝트의 리스트를 넣어준다
	std::string::size_type End = Name.rfind('_');
	std::string::size_type start = 0;
	std::string SaveName = Name.substr(start, End);
	ModelList.insert({ SaveName,SaveMesh });
}

void LoadManager::LoadMesh(std::string Name, UINT parsingMode)
{
	// "../ 이거와 .fbx 이거를 붙여준다"
	std::string modelExtention = ".fbx";
	std::string modelPath = MeshPath + Name + modelExtention;

	ModelData* SaveMesh = new ModelData();

	//파서를 통해서 매쉬를 로드
	ParserData::Model* temp = EaterParser->LoadModel(modelPath, parsingMode);
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
	if (parsingMode & ANIMATION_ONLY) { return; }
	

	//본오프셋 TM과 본리스트를 먼저읽어오기위해 
	int MeshCount = (int)temp->m_MeshList.size();
	for (int i = 0; i < MeshCount; i++)
	{
		//리스트에 매쉬 조사
		ParserData::Mesh* mesh = temp->m_MeshList[i];

		//최상위 매쉬가 아니라면 아무것도 하지않음
		if (mesh->m_TopNode != true) { continue; }

		//매쉬이고 스키닝 오브젝트라면
		if (mesh->m_MeshType == SKIN_MESH)
		{
			//본과 오프셋 정보만 읽어옴 생성은 밑에서 
			SaveMesh->BoneOffsetList	= &mesh->m_BoneTMList;
			SaveMesh->BoneList			= &mesh->m_BoneMeshList;
		}

		//매쉬이고 탑 오브젝트라면
		if (mesh->m_MeshType != BONE_MESH && mesh->m_TopNode)
		{
			LoadMeshData* TopMesh = CreateMeshObject(mesh);
			SaveMesh->TopMeshList.push_back(TopMesh);
		}
	}
	

	////읽어온 BoneOffset 과 리스트를 기반으로 생성하고 계층구조 연결후 최상위만뽑아온다
	LoadMeshData* TopBone = CreateBoneObject(SaveMesh);
	if (TopBone != nullptr) { SaveMesh->TopBoneList.push_back(TopBone); }


	//최상위 오브젝트의 리스트를 넣어준다
	std::string::size_type End = Name.rfind('_');
	std::string::size_type start = 0;
	std::string SaveName = Name.substr(start, End);
	ModelList.insert({ SaveName,SaveMesh });
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

LoadMeshData* LoadManager::CreateTerrainObject(ParserData::Mesh* mesh, std::string maskName)
{
	//계층정보 받기
	LoadMeshData* terrain = new LoadMeshData();

	SetData(terrain, mesh);

	//버퍼값 읽어오기
	terrain->IB = GEngine->CreateIndexBuffer(mesh);
	terrain->VB = GEngine->CreateTerrainVertexBuffer(mesh, maskName);

	return terrain;
}

LoadMeshData* LoadManager::CreateMeshObject(ParserData::Mesh* mesh)
{
	//계층정보 받기
	LoadMeshData* model = new LoadMeshData();

	SetData(model, mesh);

	//버퍼값 읽어오기
	model->IB = GEngine->CreateIndexBuffer(mesh);
	model->VB = GEngine->CreateVertexBuffer(mesh);

	//자식객체가 있다면 정보읽어옴
	int ChildCount = (int)mesh->m_ChildList.size();
	for (int i = 0; i < ChildCount; i++)
	{
		LoadMeshData* temp = CreateMeshObject(mesh->m_ChildList[i]);
		model->Child.push_back(temp);
		temp->Parent = model;
	}

	return model;
}

LoadMeshData* LoadManager::CreateBoneObject(ModelData* SaveData)
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
		return;
	}
	else
	{
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_LOAD, "Animation", Name, false);
		CreateAnimationKeyFrame( &(MeshData->m_AnimationList) ,10);
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

void LoadManager::CreateAnimationKeyFrame(std::vector<ParserData::OneAnimation*>* Anime,int InputKeyCount)
{
	//기존 애니메이션
	std::vector<ParserData::OneAnimation*>::iterator it = Anime->begin();

	for (it; it != Anime->end(); it++)
	{
		std::vector<ParserData::OneFrame*> data = (*it)->m_AniData;
		//새롭게 넣을 데이터 리스트
		std::vector<ParserData::OneFrame*> CreateData;

		int Size = (int)data.size();
		for (int i = 0; i < Size-1; i++)
		{
			//보간할 처음값
			DirectX::SimpleMath::Vector3 Start_Pos		= data[i]->m_Pos;
			DirectX::SimpleMath::Quaternion Start_Rot	= data[i]->m_RotQt;
			DirectX::SimpleMath::Vector3 Start_Scl		= data[i]->m_Scale;
			float Start_Time = data[i]->m_Time;

			//보간할 다음값
			int NextIndex = i + 1;
			DirectX::SimpleMath::Vector3 End_Pos		= data[NextIndex]->m_Pos;
			DirectX::SimpleMath::Quaternion End_Rot		= data[NextIndex]->m_RotQt;
			DirectX::SimpleMath::Vector3 End_Scl		= data[NextIndex]->m_Scale;
			float End_Time = data[NextIndex]->m_Time;

			///처음값 넣어주기
			CreateData.push_back(data[i]);
			
			int KeyCount = InputKeyCount;		//생성할 키프레임
			float CountLerp = (float)(1/InputKeyCount);	//생성할 키프레임 보간할 위치값을구함
			for (int j = 0; j < KeyCount; j++)
			{
				//새로운 키 프레임 생성
				ParserData::OneFrame* temp = new OneFrame();
				temp->m_Pos		= Vector3::Lerp(Start_Pos, End_Pos, CountLerp);
				temp->m_RotQt	= Quaternion::Lerp(Start_Rot, End_Rot, CountLerp);
				temp->m_Scale	= Vector3::Lerp(Start_Scl, End_Scl, CountLerp);
				temp->m_Time	= MathHelper::Lerp(Start_Time, End_Time, CountLerp);

				CreateData.push_back(temp);
				CountLerp += 0.1f;
			}

			///마지막값 넣어주기
			CreateData.push_back(data[NextIndex]);
		}

		(*it)->m_AniData = CreateData;
		(*it)->m_TicksPerFrame /= (InputKeyCount +2);
		(*it)->m_EndFrame = (Size * (InputKeyCount + 2)) - (InputKeyCount + 2);
	}
}

void LoadManager::SetData(LoadMeshData* MeshData, ParserData::Mesh* LoadData)
{
	MeshData->Name				= LoadData->m_NodeName;
	MeshData->ParentName		= LoadData->m_ParentName;
	MeshData->Top_Object		= LoadData->m_TopNode;
	MeshData->MeshType			= LoadData->m_MeshType;
	MeshData->BoneIndex			= LoadData->m_BoneIndex;

	//기존 데이터 그냥 읽어옴
	MeshData->Animation = LoadData->m_Animation;
	MeshData->Material = LoadData->m_MaterialData;
	
	// Texture Buffer 삽입..
	CMaterial* mat = LoadData->m_MaterialData;
	if (mat)
	{
		if (mat->m_TextureBind & DIFFUSE_TEXTURE)
		{
			MeshData->Albedo = GEngine->CreateTextureBuffer(mat->m_DiffuseMap->m_BitMap);
		}
		if (mat->m_TextureBind & NORMAL_TEXTURE)
		{
			MeshData->Normal = GEngine->CreateTextureBuffer(mat->m_NormalMap->m_BitMap);
		}
	}

	// Index List 삽입..
	for (int i = 0; i < LoadData->m_IndexList.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			MeshData->Index_List.push_back(LoadData->m_IndexList[i]->m_Index[j]);
		}
	}

	// Vertex List 삽입..
	for (int i = 0; i < LoadData->m_VertexList.size(); i++)
	{
		DirectX::XMVECTOR  LocalVertex	= LoadData->m_VertexList[i]->m_Pos;
		DirectX::XMMATRIX  LocalTM		= LoadData->m_LocalTM;

		DirectX::XMVECTOR WorldVertex = DirectX::XMVector3Transform(LocalVertex, LocalTM);
		DirectX::SimpleMath::Vector3 world = WorldVertex;
		//DirectX::SimpleMath::
		//LocalVertex* LocalTM;

		
		MeshData->Vertex_List.push_back(world);
	}

	//매트릭스 정보 받기
	MeshData->WorldTM = &LoadData->m_WorldTM;
	MeshData->LocalTM = &LoadData->m_LocalTM;
}











