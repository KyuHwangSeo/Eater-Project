#pragma once

/*
	2021/12/20 10:38 - CDH
	
	< ������� >
		1. ���� �κ�Ŵ����� ���ؼ� �κ񼭹����� ����� ������ ����.. ( �κ񼭹�-����, ���Ӽ���-Ŭ���̾�Ʈ ���谡 ��)
		
*/

#include "SharedDataStruct.h"
#include <string>

class DHNetWorkAPI;

class LobbyManager
{
// SingleTon
private:
	LobbyManager();

	static LobbyManager* m_Instance;

public:
	~LobbyManager();

	void Initialize(std::string _IP, int PORT);
	static LobbyManager* GetInstance();
};

