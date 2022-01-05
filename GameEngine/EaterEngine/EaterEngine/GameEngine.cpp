#include "GameEngine.h"

//�Ŵ�����
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
//������Ʈ
#include "GameObject.h"

//���۳�Ʈ
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

	//�⺻ ������ ������ ����
	WinSizeWidth	= 1920;
	WinSizeHeight	= 1080;

	//������ �ڵ�
	mHwnd = NULL;

	ConsoleDebug = true;
}

GameEngine::~GameEngine()
{

}

///���� ���� ����
void GameEngine::Initialize(HWND Hwnd, bool mConsoleDebug)
{
	//Ŭ���ʿ��� �Ѱ��� ������ �ޱ�
	mHwnd = Hwnd;

	//�Ŵ����� ����
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
	mNetworkManager		= new NetworkManager();

	//�Ŵ����� �ʱ�ȭ
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
	mNetworkManager->Initialize();

	Component::SetManager(mTimeManager, mKeyManager);
}

void GameEngine::Start()
{
	//�Ŵ��� �ʱ�ȭ �� �߰� �۾���
	mLoadManager->Start();

	// ���⿡ ������Ʈ Awake, Setup, Start �־ �Ƿ���?


	// �ε� �� �ʱ�ȭ�ܰ�
	//mTimeManager->ResetTime();
}

void GameEngine::Update()
{
	//�Ŵ����� ������Ʈ (���۳�Ʈ ������Ʈ�� ����� ������ ������ ���ӿ�����Ʈ OneMeshData�� ����)
	//Ÿ�ӸŴ����� ��������Ǿ����
	mNetworkManager->Update();
	mTimeManager->Update();
	mKeyManager->Update();
	mSceneManager->Update();
	mDebugManager->Update();
	mObjectManager->PlayUpdate();
	mPhysManager->Update(mTimeManager->DeltaTime());
	// ��� ������Ʈ�� �Ͼ �� ������ ����..
	BaseManager::UpdateGlobalData();

	//���۳�Ʈ ������Ʈ ��
	
	// ���� ������ �ɼ� ����..
	RenderOptionCheck();
	

	//����ť �Ѱ���
	mGraphicManager->BeginRender(m_RenderOption);
	mGraphicManager->ShadowRender(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->Render(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->SSAORender(mObjectManager->GetGlobalData());
	mGraphicManager->UIRender(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->LightRender(mObjectManager->GetGlobalData());
	mGraphicManager->AlphaRender(mObjectManager->GetRenderQueue(), mObjectManager->GetGlobalData());
	mGraphicManager->EndRender();



	//�������� ������ ������Ʈ Delete
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
	// â �ּ�ȭ�ô� ����
	if (Change_Width == 0 || Change_Height == 0) return;
	if (mGraphicManager == nullptr) return;

	//������ ũ�� �缳��
	WinSizeWidth	= Change_Width;
	WinSizeHeight	= Change_Height;

	DebugManager::Print(std::to_string(Change_Width).c_str());
	DebugManager::Print(std::to_string(Change_Height).c_str());

	//ī�޶��� ��ȭ�� ����� �־��ش�
	Camera::g_MainCam->SetSize(Change_Width, Change_Height);

	//�׷����ʿ� ����Ÿ���� �����ؾ��ϱ⶧���� 
	mGraphicManager->OnReSize(Change_Width, Change_Height);
	Camera::g_MainCam->CreateProj(Change_Width, Change_Height);


	std::string Width = std::to_string(Change_Width);
	std::string Height = std::to_string(Change_Height);;
	std::string temp = "������ ������ ����:"+ Width+","+ Height;
	Camera::g_MainCam->SetSize(Change_Width, Change_Height);
}

///������Ʈ ���� ����
GameObject* GameEngine::Instance(std::string ObjName)
{
	DebugManager::Line();
	//������Ʈ ����
	GameObject* temp = new GameObject();
	temp->Name = ObjName;
	DebugManager::Print(DebugManager::MSG_TYPE::MSG_CREATE, "GameObject", ObjName, false);

	//Transform �� �⺻���� �־��ش�
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

/// ��ũ�� ���� �Լ���
void GameEngine::PushScene(Scene* mScene, std::string name)
{
	std::string mStr = "�� ���� :" + name;
	mSceneManager->PushScene(mScene,name);
}

void GameEngine::ChoiceScene(std::string name)
{
	std::string mStr = "���� �� ���� :" + name;
	
	//�� ������ ���� �� �� �������� �������
	mObjectManager->AllDeleteObject();
	mObjectManager->ClearFunctionList();
	mSceneManager->ChoiceScene(name);

	//�� ������ �Ǹ� ����ü�� Awack�� Start �Լ� ���� �׸������� ���۳�Ʈ�� Awack�� Start �� ���� 
	mSceneManager->SceneStart();
	//mObjectManager->PlayStart();
}

///�ε� ���� �Լ���
void GameEngine::LoadMesh(std::string mMeshName, UINT parsingMode)
{
	std::string temp = "�Ž��� �ε��մϴ� : " + mMeshName;
	mLoadManager->LoadMesh(mMeshName, parsingMode);
}

void GameEngine::LoadTerrain(std::string mMeshName, std::string mMaskName, UINT parsingMode)
{
	std::string temp = "�ͷ����� �ε��մϴ� : " + mMeshName;
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

///Ű��ǲ �Լ���
bool GameEngine::GetKeyDown(byte number)
{
	//Ű�� �ѹ� ��������  true
	return mKeyManager->GetKeyDown(number);
}

bool GameEngine::GetKeyUp(byte number)
{
	//Ű�� �ѹ������� true
	return mKeyManager->GetKeyUp(number);
}

bool GameEngine::GetKey(byte number)
{
	//Ű�������������� �����Ӹ��� true
	return mKeyManager->GetKey(number);
}

bool GameEngine::GetTogle(byte number)
{
	//�ѹ� Ű�� ������ on �ٽô����� off
	return mKeyManager->GetKeyToggle(number);
}

float GameEngine::GetMousePosX()
{
	//���콺 ��ġ X��ǥ
	return (float)mKeyManager->GetMousePos()->x;
}

float GameEngine::GetMousePosY()
{
	//���콺 ��ġ y��ǥ
	return (float)mKeyManager->GetMousePos()->y;
}

float GameEngine::GetdeltaTime()
{
	return mTimeManager->DeltaTime();
}

void GameEngine::SetNetworkManager(NetworkManagerComponent* Manager)
{
	mNetworkManager->SetClientNetworkManager(Manager);
}

void GameEngine::NETWORK_SEND(flatbuffers::FlatBufferBuilder* Builder, int Type)
{
	mNetworkManager->NETWORK_SEND(Builder, Type);
}

void GameEngine::NETWORK_LOADING_SEND()
{
	mNetworkManager->C2S_LOADING_COMPLETE_SEND();
}

void GameEngine::CreateObject()
{
	GameObject* light = Instance();
	light->AddComponent<DirectionLight>();
}

void GameEngine::RenderOptionCheck()
{
	if (mKeyManager->GetKeyUp(VK_F9))
	{
		// Debug On/Off
		m_RenderOption |= RENDER_DEBUG;
	}
	//if (mKeyManager->GetKeyUp(VK_F2))
	//{
	//	// Gamma Correction On/Off
	//	m_RenderOption |= RENDER_GAMMA_CORRECTION;
	//}
	//if (mKeyManager->GetKeyUp(VK_F3))
	//{
	//	// Shadow On/Off
	//	m_RenderOption |= RENDER_SHADOW;
	//}
	if (mKeyManager->GetKeyUp(VK_F4))
	{
		// SSAO On/Off
		m_RenderOption |= RENDER_SSAO;
	}
}