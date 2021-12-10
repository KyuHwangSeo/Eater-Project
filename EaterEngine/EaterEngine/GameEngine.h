#pragma once
#include <windows.h>
#include <string>
#include "EaterEngineDLL.h"

/// <summary>
/// 전체 게임 엔진을 총괄하는 게임엔진 클래스
/// 커다란 게임엔진 틀
/// </summary>

//각종 매니저들
class DebugManager;
class LoadManager;
class ObjectManager;
class SceneManager;
class KeyinputManager;
class GraphicEngineManager;
class TimeManager;
class MaterialManager;
class LightManager;
class PhysManager;

class Scene;
//게임 오브젝트
class GameObject;

class GameEngine
{
public:
	GameEngine();
	~GameEngine();
	
public:
	///클라이언트 에서 만질 함수들
	///게임 엔진 관련 함수들
	void Initialize(HWND gHwnd,bool mConsoleDebug = true);	//초기화
	void Update();											//엔진 가동
	void Finish();											//엔진 중지
	void OnResize(int Change_Width, int Change_Height);		//리사이즈
public:
	///오브젝트 관련(오브젝트 매니저)
	GameObject* Instance(std::string ObjName = "GameObject");			//obj 생성 (빈 게임오브젝트)
	void		Destroy(GameObject* obj);								//obj 삭제
public:
	///스크린 관련
	void PushScene(Scene* mScene, std::string name);	//씬 생성		
	void ChoiceScene(std::string name);					//씬 선택
public:
	///로드 관련 
	void LoadMeshPath(std::string mPath);
	void LoadTexturePath(std::string mPath);
	void LoadMesh(std::string mMeshName, bool Scale = true, bool LoadAnime = false);
	void LoadTexture(std::string mMeshName);
public:
	///키관련 
	bool  GetKeyDown(byte number);	//키다운
	bool  GetKeyUp(byte number);	//키업
	bool  GetKey(byte number);		//키누르고있을때
	bool  GetTogle(byte number);	//키 on/off
	float GetMousePosX();			//마우스 위치 X
	float GetMousePosY();			//마우스 위치 Y
public:
	///카메라 관련
	//Vector3 GetCameraLocalPosition_UP();
	//Vector3 GetCameraLocalPosition_Right();
	//Vector3 GetCameraLocalPosition_Look();
public:
	///시간 관련
	float GetdeltaTime();

private:
	void CreateObject();			//기본 생성 오브젝트

private:
	///각종 매니저들
	ObjectManager*			mObjectManager;		// 오브젝트 관리 매니저
	DebugManager*			mDebugManager;		// 디버깅매니저
	LoadManager*			mLoadManager;		// 로드 관련 매니저
	SceneManager*			mSceneManager;		// 스크린 관리 매니저
	KeyinputManager*		mKeyManager;		// 키인풋 관련 매니저
	GraphicEngineManager*	mGraphicManager;	// 그래픽엔진 관리 매니저
	TimeManager*			mTimeManager;		// 시간 관련 타임매니저
	MaterialManager*		mMaterialManager;	// 메테리얼 관리 매니저
	LightManager*			mLightManager;		// 라이트 관리 매니저
	PhysManager*			mPhysManager;		// 물리 관리 매니저
private:
	///클라쪽에서 받아야 하는 데이터
	HWND mHwnd; //핸들
	int WinSizeWidth;	//윈도우 사이즈 가로
	int WinSizeHeight;	//윈도우 사이즈 세로
	bool ConsoleDebug;
private:
};

