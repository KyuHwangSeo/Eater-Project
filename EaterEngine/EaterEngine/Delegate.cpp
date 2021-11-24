#include "Delegate.h"

Delegate_Map::Delegate_Map()
{
}

Delegate_Map::~Delegate_Map()
{
}

void Delegate_Map::Push(ComponentFunctionData temp)
{
	FunctionList.push_back(temp);
}

void Delegate_Map::Pop(Component* Key)
{
	int size = (int)FunctionList.size();
	for (int i = 0; i < size; i++)
	{
		if (FunctionList[i].ComponentPoiner == Key)
		{
			FunctionList.erase(FunctionList.begin() + i);
			break;
		}
	}
}

void Delegate_Map::Clear()
{
	FunctionList.clear();
}

void Delegate_Map::Play(bool OnePlay)
{
	int size = (int)FunctionList.size();
	for (int i = 0; i < size; i++)
	{
		//함수포인터가 없다면 통과
		if (FunctionList[i].FunctionPointer == nullptr) { continue; }
		//기능을꺼놨다면 통과
		if (*(FunctionList[i].Enabled) == false) { continue; }


		//함수 실행
		FunctionList[i].FunctionPointer();
	}

	//한번만 실행되는 함수포인터라면 모두 실행후 삭제
	if (OnePlay == true)
	{
		FunctionList.clear();
	}
}
