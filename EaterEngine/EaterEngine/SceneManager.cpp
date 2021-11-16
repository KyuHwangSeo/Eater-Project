#include "SceneManager.h"
#include "ObjectManager.h"
#include "DebugManager.h"
#include "Scene.h"

SceneManager::SceneManager()
{
	NowScene = nullptr;
}

SceneManager::~SceneManager()
{

}

void SceneManager::PushScene(Scene* mScene, std::string SceneName)
{
	SceneList.insert({ SceneName,mScene});
}

void SceneManager::Initialize()
{

}

void SceneManager::ChoiceScene(std::string mSceneName)
{
	SceneName = mSceneName;
	//게임 시작할때가아닌 어떤 씬을 돌리고있다가 다른씬으로 돌릴때
	if (NowScene != nullptr)
	{
		NowScene->End();

		NowScene = nullptr;
	}

	if (SceneList.find(SceneName) == SceneList.end())
	{
		//찾지 못했을 경우 에러코드
	}

	NowScene = SceneList[SceneName];
}


void SceneManager::Delete()
{
	NowScene = nullptr;

	//모든 스크린 삭제
	SceneList.clear();
}

void SceneManager::SceneStart()
{
	NowScene->Awake();
	NowScene->Start();
}

void SceneManager::SceneEnd()
{
	if (NowScene != nullptr)
	{
		NowScene->End();
	}
}

void SceneManager::Update()
{
	std::string temp = "현재 선택된 씬 :" + SceneName;
	DebugManager::Print(temp, 80, 2, DebugManager::MSG_TYPE::MSG_ENGINE);

	//스크린 먼저 업데이트하고 그다음 컨퍼넌트들 업데이트
	NowScene->Update();
	///삭제여부?
}


