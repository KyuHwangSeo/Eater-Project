#pragma once

#include <functional>
#include <string>
#include <windows.h>
#include "EaterEngineDLL.h"

class GameObject;
class Scene;

typedef enum PARSING_MODE
{
	SCALING			= 0x00000001,
	ANIMATION_ONLY	= 0x00000010,
	ROTATE_AXIS		= 0x00000100
}PARSING_MODE;

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
extern "C" EATER_ENGINEDLL void LoadMesh(std::string mMeshName, UINT parsingMode = 0);	//매쉬 로드
extern "C" EATER_ENGINEDLL void LoadTerrainMesh(std::string mMeshName, std::string maskName, UINT parsingMode = 0);	//매쉬 로드
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

///카메라 관련
EATER_ENGINEDLL Vector3 GetCameraLocalPosition_UP();
EATER_ENGINEDLL Vector3 GetCameraLocalPosition_Right();
EATER_ENGINEDLL Vector3 GetCameraLocalPosition_Look();

///시간 관련
extern "C" EATER_ENGINEDLL float GetDeltaTime();


template<typename T>
inline void CreateScene(std::string Name)
{
	T* temp = new T();
	CreateSceneSub(temp, Name);
}