#pragma once

#include <functional>
#include "EngineData.h"
#include <vector>

/// <summary>
///	순환과정에서 함수포인터의 순서를 보장해주기위해 map 에서 vector 로변경
/// </summary>
class Component;
class Delegate_Map
{
public:
	Delegate_Map();
	~Delegate_Map();
	
	void Push(ComponentFunctionData temp);
	void Pop(Component* Key);
	void Clear();
	
	//모든 함수 포인터 리스트 실행
	void Play(bool OnePlay = false);
private:
	std::vector<ComponentFunctionData> FunctionList;
};

