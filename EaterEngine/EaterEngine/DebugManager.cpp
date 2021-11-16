#include "DebugManager.h"
#include "KeyinputManager.h"

HANDLE DebugManager::hConsole[2];
int	DebugManager::ConsoleIndex = 0;

bool DebugManager::DebugON = true;
DebugManager::DebugManager()
{
	KeyManager = nullptr;
	DebugON = true;
}

DebugManager::~DebugManager()
{
}

void DebugManager::Initialize(KeyinputManager* mkeyManager, bool mDebugOn)
{
	DebugON = mDebugOn;
	if (DebugON == true)
	{
		AllocConsole();
		//콘솔의 버퍼를 가져온다
		hConsole[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		hConsole[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		KeyManager = mkeyManager;

		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 1;
		cci.bVisible = FALSE;

		SetConsoleCursorInfo(hConsole[0], &cci);
		SetConsoleCursorInfo(hConsole[1], &cci);
	}
}

void DebugManager::printStart()
{
	///어느시점에 어떤함수들이 실행되는지 헷갈릴까봐 엔진 순서도
	if (DebugON == false) { return; }

	DWORD dwByte(0);
	std::string Msg;

	Msg += "===============================================================\n";
	Msg += "Eater Engine 순서도\n";
	Msg += "===============================================================\n";
	Msg += "===============================================================\n";
	Msg += "\n";
	Msg += "______________엔진 순서_________________\n";
	Msg += "[엔진]게임 엔진 초기화\n";
	Msg += "[클라]게임매니저에서 각종 씬을 생성\n";
	Msg += "[클라]게임매니저에서 씬을 선택\n";
	Msg += "[엔진]오브젝트 매니저 이전씬 함수리스트 정보 전부 삭제\n";
	Msg += "[엔진]오브젝트 매니저 이전씬 오브젝트 정보 전부 삭제\n";
	Msg += "[엔진]변경된 씬의 Awack,Start 함수 실행\n";
	Msg += "[엔진]생성된 오브젝트들의 함수리스트를 생성및실행 (Awack,Start)  \n";
	Msg += "[엔진]생성된 함수리스트 기반으로 업데이트\n";
	Msg += "[엔진]업데이트된 랜더큐를 그래픽엔진쪽으로 전달\n";
	Msg += "[엔진]마지막 삭제할 오브젝트들이 있다면 삭제\n";


	Msg += "\n";
	Msg += "____________컨퍼넌트 순서_____________\n";
	Msg += "Awake				(한번 호출)\n";
	Msg += "Start				(한번 호출)\n";
	Msg += "StartUpdate			(프레임 호출)\n";
	Msg += "TransformUpdate			(프레임 호출)\n";
	Msg += "PhysicsUpdate			(프레임 호출)\n";
	Msg += "Update				(프레임 호출)\n";
	Msg += "EndUpdate			(프레임 호출)\n";
	Msg += "------------------------------\n";
	
	COORD CursorPosition = { 0, 0 };
	DWORD dw;
	SetConsoleCursorPosition(hConsole[ConsoleIndex], CursorPosition);
	WriteFile(hConsole[ConsoleIndex], Msg.c_str(), Msg.size(), &dw, NULL);
}

void DebugManager::Print(std::string Msg, int X, int Y, MSG_TYPE type, bool Error)
{
	if (DebugON == false) { return; }

	std::string MSG;
	std::string temp;
	switch ((int)type)
	{
	case (int)MSG_TYPE::MSG_LOAD:
		MSG = "[로드]" + Msg;
		break;
	case (int)MSG_TYPE::MSG_CREATE:
		MSG = "[생성]" + Msg;
		break;
	case (int)MSG_TYPE::MSG_DELETE:
		MSG = "[삭제]" + Msg;
		break;
	case (int)MSG_TYPE::MSG_ENGINE:
		MSG = "[엔진]" + Msg;
		break;
	case (int)MSG_TYPE::MSG_ERROR:
		MSG = "[에러]" + Msg;
		break;
	}

	if (Error == true)
	{
		temp = "-------------[실패]\n";
		MSG += temp;
	}
	else
	{
		temp = "\n";
		MSG += temp;
	}
	


	COORD CursorPosition = { X, Y };
	DWORD dw;
	SetConsoleCursorPosition(hConsole[ConsoleIndex], CursorPosition);
	WriteFile(hConsole[ConsoleIndex], MSG.c_str(), MSG.size(), &dw, NULL);
}

void DebugManager::Begin()
{
	//그릴곳 설정
	COORD CursorPosition = { 0, 0 };
	DWORD dw;
	SetConsoleCursorPosition(hConsole[ConsoleIndex], CursorPosition);
	std::string temp = "ㅎㅇ";
	WriteFile(hConsole[ConsoleIndex], temp.c_str(), temp.size(), &dw, NULL);

}

void DebugManager::End()
{
	//화면에 보여줄 콘솔을 스위칭 해준다
	SetConsoleActiveScreenBuffer(hConsole[ConsoleIndex]);

	ConsoleIndex = !ConsoleIndex;
}

void DebugManager::Clear()
{
	//그릴곳을 지워준다
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(hConsole[ConsoleIndex], ' ', 80 * 25, Coor, &dw);
}

void DebugManager::Update()
{
	
	

	



}

void DebugManager::Delete()
{
	CloseHandle(hConsole[0]);
	CloseHandle(hConsole[1]);
}


