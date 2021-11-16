#pragma once
#include <windows.h>
#include <string>
#include "EnumClass.h"
/// <summary>
/// 테스트용 임시 디버깅 클래스
/// </summary>
class KeyinputManager;
class DebugManager
{
public:
	DebugManager();
	~DebugManager();

	enum class MSG_TYPE
	{
		MSG_LOAD,
		MSG_CREATE,
		MSG_DELETE,
		MSG_ENGINE,
		MSG_ERROR,
	};

	void Initialize(KeyinputManager* mkeyManager,bool mDebugOn);
	void printStart();
	static void Print(std::string Msg,int X,int Y, MSG_TYPE type,bool Error = false);

	
	void Begin();	//콘솔창 그리기 시작
	void End();		//콘솔창 그리기 종료
	void Clear();	//콘솔창 클리어

	void Update();
	void Delete();
private:
	static HANDLE hConsole[2];
	
	static int ConsoleIndex;
	bool Front = false;
	KeyinputManager* KeyManager;
	static bool DebugON;
};