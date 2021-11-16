#pragma
#include "MainHeader.h"
#include "GameManager.h"
#include "intro.h"
#include "inGame.h"




GameManager::GameManager()
{
}

GameManager::~GameManager()
{

}

void GameManager::Start(HWND _g_hWnd)
{
	//엔진 초기화 (매니저들 생성)
	EngineInitialize(_g_hWnd);
	
	CreateScene<intro>("intro");
	CreateScene<InGame>("inGame");
	ChoiceScene("intro");

}

void GameManager::Update()
{
	//넘겨준 스크린 바탕으로 스크린의 업데이트를 돌려줌
	UpdateEngine();
}