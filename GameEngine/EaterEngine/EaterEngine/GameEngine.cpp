#include "GameEngine.h"

//매니저들
#include "KeyinputManager.h"
#include "LoadManager.h"
#include "ObjectManager.h"
#include "SceneManager.h"
#include "DebugManager.h"
#include "GraphicEngineManager.h"
#include "TimeManager.h"
#include "MaterialManager.h"
#include "LightManager.h"
#include "PhysManager.h"
#include "NetworkManager.h"

#include "ParserData.h"
#include "EngineData.h"
//오브젝트
#include "GameObject.h"

//컨퍼넌트
#include "Transform.h"
#include "Camera.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "Light.h"
#include "Rigidbody.h"
#include "Material.h"
#include "Terrain.h"
#include "ParticleSystem.h"

GameEngine::GameEngine()
{
	mDebugManager	= nullptr;
	mLoadManager	= nullptr;
	mObjectManager	= nullptr;
	mSceneManager	= nullptr;
	mKeyManager		= nullptr;
	mPhysManager	= nullptr;
	mLightManager	= nullptr;
	mMaterialManager = nullptr;
	mTimeManager	= nullptr;
	mGraphicManager = nullptr;
	mNetworkManager = nullptr;

	//기본 윈도우 사이즈 설정
	WinSizeWidth	= 1920;
	WinSizeHeight	= 1080;

	//윈도우 핸들
	mHwnd = NULL;

	ConsoleDebug = true;
}

GameEngine::~GameEngine()
{

}

///게임 엔진 관련
void GameEngine::Initialize(HWND Hwnd, bool mConsoleDebug)
{
	//클라쪽에서 넘겨준 데이터 받기
	mHwnd = Hwnd;

	//매니저들 생성
	mKeyManager			= new KeyinputManager();
	mLoadManager		= new LoadManager();
	mObjectManager		= new ObjectManager();
	mSceneManager		= new SceneManager();
	mDebugManager		= new DebugManager();
	mGraphicManager		= new GraphicEngineManager();
	mTimeManager		= new TimeManager();
	mMaterialManager	= new MaterialManager();
	mLightManager		= new LightManager();
	mPhysManager		= new PhysManager();
	//mNetworkManager		= new NetworkManager();

	//매니저들 초기화
	BaseManager::Initialize();
	mGraphicManager->Initialize(Hwnd, WinSizeWidth, WinSizeHeight, mObjectManager); 
	mKeyManager->Initialize(mHwnd);
	mDebugManager->Initialize(mKeyManager,mConsoleDebug);
	mSceneManager->Initialize();
	mObjectManager->Initialize(mHwnd);
	mLoadManager->Initialize(mGraphicManager);
	mTimeManager->Initialize();
	mLightManager->Initialize();
	mMaterialManager->Initialize();
	mPhysManager->Initialize();
	//mNetworkManager->Initialize();

	Component::SetManager(mTimeManager, mKeyManager);
}

void GameEngine::Start()
{
	//매니저 초기화 후 추가 작업들
	mLoadManager->Start();

	// 여기에 오브젝트 Awake, Setup, Start 넣어도 되려나?


	// 로드 후 초기화단계
	//mTimeManager->ResetTime();
}

void GameEngine::Update()
{
	//매니저들 업데이트 (컨퍼넌트 업데이트후 변경된 사항을 각각의 게임오브젝트 OneMeshData에 전달)
	//타임매니저는 먼저실행되어야함
	mTimeManager->Update();
	mKeyManager->Update();
	mSceneManager->Update();
	mDebugManager->Update();
	mObjectManager->PlayUpdate();
	mPhysManager->Update(mTimeManager->DeltaTime());
	//mNetworkManager->Update();
	
	// 모든 업데이트가 일어난 후 데이터 세팅..
	BaseManager::UpdateGlobalData();

	//컨퍼넌트 업데이트 끝
	


	// 현재 랜더링 옵션 설정..
	RenderOptionCheck();

	//랜더큐 넘겨줌
	mGraphicManager->BeginRender(m_RenderOption);
	mGraphicManager->ShadowRender(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->Render(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->SSAORender(mObjectManager->GetGlobalData());
	mGraphicManager->UIRender(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->LightRender(mObjectManager->GetGlobalData());
	mGraphicManager->EndRender();

	//랜더링이 끝나고 오브젝트 Delete
	mObjectManager->DeleteObject();
}

void GameEngine::Finish()
{
	delete mKeyManager;
	delete mLoadManager;

	mDebugManager->Delete();
	mSceneManager->Delete();

	BaseManager::Reset();
}

void GameEngine::OnResize(int Change_Width, int Change_Height)
{
	// 창 최소화시는 제외
	if (Change_Width == 0 || Change_Height == 0) return;

	//윈도우 크기 재설정
	WinSizeWidth	= Change_Width;
	WinSizeHeight	= Change_Height;
		

	//카메라의 변화할 사이즈를 넣어준다
	Camera::g_MainCam->SetSize(Change_Width, Change_Height);

	//그래픽쪽에 랜더타겟을 변경해야하기때문에 
	mGraphicManager->OnReSize(Change_Width, Change_Height);
	Camera::g_MainCam->CreateProj(Change_Width, Change_Height);


	std::string Width = std::to_string(Change_Width);
	std::string Height = std::to_string(Change_Height);;
	std::string temp = "윈도우 사이즈 변경:"+ Width+","+ Height;
	Camera::g_MainCam->SetSize(Change_Width, Change_Height);
}

///오브젝트 생성 삭제
GameObject* GameEngine::Instance(std::string ObjName)
{
	DebugManager::Line();
	//오브젝트 생성
	GameObject* temp = new GameObject();
	temp->Name = ObjName;
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_CREATE, "GameObject", ObjName, false);

	//Transform 은 기본으로 넣어준다
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;


	return temp;
}

GameObject* GameEngine::InstanceTerrain(std::string ObjName)
{
	GameObject* temp = new GameObject();
	temp->Name = ObjName;
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_CREATE, "Terrain", ObjName, false);

	//Transform
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	temp->AddComponent<MeshFilter>();
	temp->AddComponent<Rigidbody>();
	temp->AddComponent<Terrain>();


	return temp;
}

GameObject* GameEngine::InstanceParticle(std::string ObjName /*= "Particle"*/)
{
	GameObject* temp = new GameObject();
	temp->Name = ObjName;
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_CREATE, "Particle", ObjName, false);

	//Transform
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;

	temp->AddComponent<MeshFilter>();
	temp->AddComponent<ParticleSystem>();

	return temp;
}

void GameEngine::Destroy(GameObject* obj)
{
	mObjectManager->PushDeleteObject(obj);
}

/// 스크린 관련 함수들
void GameEngine::PushScene(Scene* mScene, std::string name)
{
	std::string mStr = "씬 생성 :" + name;
	mSceneManager->PushScene(mScene,name);
}

void GameEngine::ChoiceScene(std::string name)
{
	std::string mStr = "현재 씬 선택 :" + name;
	
	//씬 선택후 이전 씬 의 정보들을 모두지움
	mObjectManager->AllDeleteObject();
	mObjectManager->ClearFunctionList();
	mSceneManager->ChoiceScene(name);

	//씬 선택이 되면 씬자체의 Awack와 Start 함수 실행 그리고나서 컨퍼넌트의 Awack와 Start 도 실행 
	mSceneManager->SceneStart();
	//mObjectManager->PlayStart();
}

///로드 관련 함수들
void GameEngine::LoadMesh(std::string mMeshName, UINT parsingMode)
{
	std::string temp = "매쉬를 로드합니다 : " + mMeshName;
	mLoadManager->LoadMesh(mMeshName, parsingMode);
}

void GameEngine::LoadTerrain(std::string mMeshName, std::string mMaskName, UINT parsingMode)
{
	std::string temp = "터레인을 로드합니다 : " + mMeshName;
	mLoadManager->LoadTerrain(mMeshName, mMaskName, parsingMode);
}

void GameEngine::LoadTexture(std::string mTextureName)
{
	mLoadManager->LoadTexture(mTextureName);
}

void GameEngine::LoadMeshPath(std::string mPath)
{
	mLoadManager->LoadMeshPath(mPath);
}

void GameEngine::LoadTexturePath(std::string mPath)
{
	mLoadManager->LoadTexturePath(mPath);
}

///키인풋 함수들
bool GameEngine::GetKeyDown(byte number)
{
	//키를 한번 눌렀을때  true
	return mKeyManager->GetKeyDown(number);
}

bool GameEngine::GetKeyUp(byte number)
{
	//키를 한번땠을때 true
	return mKeyManager->GetKeyUp(number);
}

bool GameEngine::GetKey(byte number)
{
	//키를누르고있을때 프레임마다 true
	return mKeyManager->GetKey(number);
}

bool GameEngine::GetTogle(byte number)
{
	//한번 키를 누르면 on 다시누르면 off
	return mKeyManager->GetKeyToggle(number);
}

float GameEngine::GetMousePosX()
{
	//마우스 위치 X좌표
	return (float)mKeyManager->GetMousePos()->x;
}

float GameEngine::GetMousePosY()
{
	//마우스 위치 y좌표
	return (float)mKeyManager->GetMousePos()->y;
}

float GameEngine::GetdeltaTime()
{
	return mTimeManager->DeltaTime();
}

void GameEngine::CreateObject()
{
	GameObject* light = Instance();
	light->AddComponent<DirectionLight>();
}

void GameEngine::RenderOptionCheck()
{
	if (mKeyManager->GetKeyUp(VK_F1))
	{
		// Gamma Correction On/Off
		m_RenderOption |= RENDER_GAMMA_CORRECTION;
	}
	if (mKeyManager->GetKeyUp(VK_F2))
	{
		// Shadow On/Off
		m_RenderOption |= RENDER_SHADOW;
	}
	if (mKeyManager->GetKeyUp(VK_F3))
	{
		// SSAO On/Off
		m_RenderOption |= RENDER_SSAO;
	}
}
