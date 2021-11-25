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

//테스트용
#include "HsGraphic.h"
#include "DH3DEngine.h"
#include "KHGraphic.h"

GameEngine::GameEngine()
{
	mDebugManager	= nullptr;
	mLoadManager	= nullptr;
	mObjectManager	= nullptr;
	mSceneManager	= nullptr;
	mKeyManager		= nullptr;


	//기본 윈도우 사이즈 설정
	WinSizeWidth	= 1920;
	WinSizeHeight	= 1080;

	//윈도우 핸들
	mHwnd = NULL;
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

	//매니저들 인풋
	MeshFilter::SetManager(mObjectManager, mMaterialManager);
	Light::SetManager(mLightManager);

	//매니저들 초기화
	BaseManager::Initialize();
	mKeyManager->Initialize(mHwnd);
	mDebugManager->Initialize(mKeyManager,mConsoleDebug);
	mSceneManager->Initialize();
	mObjectManager->Initialize(mHwnd);
	mLoadManager->Initialize(mGraphicManager);
	mTimeManager->Initialize();
	mLightManager->Initialize();
	mMaterialManager->Initialize();

	Component::SetManager(mTimeManager, mKeyManager);


	mGraphicManager->Initialize(Hwnd, WinSizeWidth, WinSizeHeight, mObjectManager);


	//처음시작하기전 엔진의 구조간략설명

	/// <summary>
	/// 여기 두부분만 해주면 그래픽엔진 매니저에서 알아서 해줄꺼임
	/// 다만 그래픽엔진의 순수가상함수로된건 무조건다만들어놔야함
	/// </summary>
	/////////////////////////////////////////////////////////////////

	//mGraphicManager->PushEngine<HsGraphic>("형선");
	//mGraphicManager->PushEngine<DH3DEngine>("동혁");
	//mGraphicManager->PushEngine<KHGraphic>("규황");
	//mGraphicManager->ChoiceEngine("형선");

	//윈도를 가로 2 	세로3번으로 분할시키겠다 (총 윈도우의 수 = 2 * 3)
	mGraphicManager->SplitWindow(1, 1);

	//엔진을 지정된 넘버로 넣는다
	//mGraphicManager->PushEngine(0, new HsGraphic(), "형선");
	mGraphicManager->PushEngine(0, new KHGraphic(), "규황");
	//mGraphicManager->PushEngine(2, new DH3DEngine(), "동혁");
	//mGraphicManager->PushEngine(3, new DH3DEngine(), "동혁0");



	//mGraphicManager->PushEngine(1, new HsGraphic(), "형선0");
	//mGraphicManager->PushEngine(2, new HsGraphic(), "형선1");
	//mGraphicManager->PushEngine(3, new DH3DEngine(), "동혁1");
	//mGraphicManager->PushEngine(4, new HsGraphic(), "형선2");
	//mGraphicManager->PushEngine(5, new KHGraphic(), "규황0");
	//mGraphicManager->PushEngine(6, new KHGraphic(), "규황1");
	//mGraphicManager->PushEngine(7, new HsGraphic(), "형선3");
	//mGraphicManager->PushEngine(8, new HsGraphic(), "형선4");
	//mGraphicManager->PushEngine(9, new KHGraphic(), "규황2");
	//mGraphicManager->PushEngine(10, new KHGraphic(), "규황3");
	//mGraphicManager->PushEngine(11, new HsGraphic(), "형선5");
	//mGraphicManager->PushEngine(12, new DH3DEngine(), "동혁2");
	//mGraphicManager->PushEngine(13, new HsGraphic(), "형선6");
	//mGraphicManager->PushEngine(14, new HsGraphic(), "형선7");
	//mGraphicManager->PushEngine(15, new DH3DEngine(), "동혁3");


	/////////////////////////////////////////////////////////////////
}

void GameEngine::Update()
{
	
	//매니저들 업데이트 (컨퍼넌트 업데이트후 변경된 사항을 각각의 게임오브젝트 OneMeshData에 전달)
	//타임매니저는 먼저실행되어야함
	mTimeManager->Update();
	mKeyManager->Update();
	mSceneManager->Update();
	mObjectManager->PlayUpdate();
	mDebugManager->Update();
	

	//컨퍼넌트 업데이트 끝
	//그래픽엔진으로 넘겨줄 랜더큐도 생성완료

	
	//랜더큐 넘겨줌
	mGraphicManager->ShadowRender(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->Render(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->SSAORender(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->UIRender(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());


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
	//윈도우 크기 재설정
	WinSizeWidth	= Change_Width;
	WinSizeHeight	= Change_Height;
		

	//카메라의 변화할 사이즈를 넣어준다
	Camera::SetSize(Change_Width, Change_Height);

	//그래픽쪽에 랜더타겟을 변경해야하기때문에 
	mGraphicManager->OnReSize(Change_Width, Change_Height);
	Camera::CreateProj(Change_Width, Change_Height);


	std::string Width = std::to_string(Change_Width);
	std::string Height = std::to_string(Change_Height);;
	std::string temp = "윈도우 사이즈 변경:"+ Width+","+ Height;
	Camera::SetSize(Change_Width, Change_Height);
}

///오브젝트 생성 삭제
GameObject* GameEngine::Instance(std::string ObjName)
{
	DebugManager::Line();
	//오브젝트 생성
	GameObject* temp = new GameObject();
	mObjectManager->PushCreateObject(temp);
	temp->Name = ObjName;
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_CREATE, "GameObject", ObjName, false);

	//Transform 은 기본으로 넣어준다
	Transform* Tr = temp->AddComponent<Transform>();
	temp->transform = Tr;


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
void GameEngine::LoadMesh(std::string mMeshName, bool Scale, bool LoadAnime)
{
	std::string temp = "매쉬를 로드합니다 : " + mMeshName;
	mLoadManager->LoadMesh(mMeshName, Scale, LoadAnime);
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
