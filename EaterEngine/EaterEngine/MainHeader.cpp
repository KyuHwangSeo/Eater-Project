#include "MainHeader.h"
#include "GameEngine.h"
GameEngine* gGameEngine;

///엔진 관련
 void EngineInitialize(HWND _g_hWnd)
{
	gGameEngine = new GameEngine();
	gGameEngine->Initialize(_g_hWnd);
}

void EndEngine()
{
	//gDebugManger->Delete();
}

 void UpdateEngine()
{
	gGameEngine->Update();
}

 void OnReSize(int X, int Y)
{
	//처음 시작 상태가 아닐때
	if (gGameEngine != nullptr)
	{
		gGameEngine->OnResize(X, Y);
	}
}

///오브젝트 관련
 GameObject* Instance(std::string ObjName)
{
	return gGameEngine->Instance(ObjName);
}

 void Destroy(GameObject* obj)
{
	gGameEngine->Destroy(obj);
}


///씬 관련
 void ChoiceScene(std::string name)
{
	gGameEngine->ChoiceScene(name);
}

 void CreateSceneSub(Scene* mSceneTemp, std::string SceneName)
{
	gGameEngine->PushScene(mSceneTemp, SceneName);
}

 void LoadMesh(std::string mMeshName, bool Scale,bool LoadAnime)
{
	gGameEngine->LoadMesh(mMeshName,Scale,LoadAnime);
}

///로드 관련
 void LoadMeshPath(std::string mPath)
{
	gGameEngine->LoadMeshPath(mPath);
}

 void LoadTesture(std::string mPath)
{
	gGameEngine->LoadTexture(mPath);
}

 void LoadTesturePath(std::string mPath)
{
	gGameEngine->LoadTexturePath(mPath);
}

///키관련
 bool GetKeyDown(byte number)
{
	return gGameEngine->GetKeyDown(number);
}

 bool GetKeyUp(byte number)
{
	return gGameEngine->GetKeyUp(number);
}

 bool GetKey(byte number)
{
	return gGameEngine->GetKey(number);
}

 bool GetTogle(byte number)
{
	return gGameEngine->GetTogle(number);
}

 float GetMousePosX()
{
	return gGameEngine->GetMousePosX();
}

 float GetMousePosY()
{
	return gGameEngine->GetMousePosY();
}


 /// 시간 관련
 float GetDeltaTime()
 {
	 return gGameEngine->GetdeltaTime();
 }

