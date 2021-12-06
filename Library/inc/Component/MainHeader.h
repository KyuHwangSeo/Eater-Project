#pragma once

#include <functional>
#include <string>
#include <windows.h>
#include "EaterEngineDLL.h"

class GameObject;
class Scene;

extern "C" EATER_ENGINEDLL void EngineInitialize(HWND _g_hWnd);		//GameEngine 시작
extern "C" EATER_ENGINEDLL void EndEngine();						//GameEngine 종료
extern "C" EATER_ENGINEDLL void UpdateEngine();						//GameEngine 업데이트
extern "C" EATER_ENGINEDLL void OnReSize(int X,int Y);				//윈도우 사이즈 재조정	

///게임 오브젝트 생성
extern "C" EATER_ENGINEDLL GameObject*	Instance(std::string ObjName= "GameObject");//오브젝트 생성
extern "C" EATER_ENGINEDLL void			Destroy(GameObject* obj);					//오브젝트 삭제


///스크린 생성하기
template<typename T>
void CreateScene(std::string Name);
extern "C" EATER_ENGINEDLL void ChoiceScene(std::string name);					//스크린 선택
EATER_ENGINEDLL void CreateSceneSub(Scene* mSceneTemp,std::string SceneName);	//스크린 생성


///매쉬 불러오기
extern "C" EATER_ENGINEDLL void LoadMesh(std::string mMeshName, bool Scale = true,bool LoadAnime = false);	//매쉬 로드
extern "C" EATER_ENGINEDLL void LoadMeshPath(std::string mPath);											//매쉬 로드 경로
extern "C" EATER_ENGINEDLL void LoadTexture(std::string mPath);
extern "C" EATER_ENGINEDLL void LoadTexturePath(std::string mPath);


///키입력
extern "C" EATER_ENGINEDLL bool  GetKeyDown(byte number);	//키다운
extern "C" EATER_ENGINEDLL bool  GetKeyUp(byte number);		//키업
extern "C" EATER_ENGINEDLL bool  GetKey(byte number);		//키누르고있을때
extern "C" EATER_ENGINEDLL bool  GetTogle(byte number);		//키 토글
extern "C" EATER_ENGINEDLL float GetMousePosX();			//마우스 위치 X
extern "C" EATER_ENGINEDLL float GetMousePosY();			//마우스 위치 Y

///시간 관련
extern "C" EATER_ENGINEDLL float GetDeltaTime();


template<typename T>
inline void CreateScene(std::string Name)
{
	T* temp = new T();
	CreateSceneSub(temp, Name);
}