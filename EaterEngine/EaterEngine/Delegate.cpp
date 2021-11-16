#include "Delegate.h"



//template<typename T>
//void Delegate_Map<T>::Push(Component* ComponentKey, std::function<void()> temp)
//{
//
//}

//template<typename T>
//void Delegate_Map<T>::Push(T* ComponentKey, std::function<void()> temp)
//{
//	FunctionList.insert({ ComponentKey,temp });
//}

//template<typename T>
//void Delegate_Map<T>::Pop(T* temp)
//{
//	FunctionList[temp] = nullptr;
//}

//template<typename T>
//void Delegate_Map<T>::Play()
//{
//	//map의 처음부터 끝까지 순환
//	std::map<T*, std::function<void()>>::iterator list = FunctionList.begin();
//	
//	while(list != FunctionList.end())
//	{
//		if (list->second != nullptr)
//		{
//			list->second();
//		}
//		++list;
//	}
//}
