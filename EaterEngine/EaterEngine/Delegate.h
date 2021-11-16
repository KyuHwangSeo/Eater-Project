#pragma once

#include <functional>
#include <vector>
#include <map>

/// ////////////////////////////////////////////////////////////////
/// // 유니티의 델리게이트를 참고하여 제작한 함수 포인터 리스트 클래스 //
/// ////////////////////////////////////////////////////////////////
/// 
/// 두개의 델리게이트
/// 1.vector 를 사용한 함수포인터 리스트
/// 2.map 을 사용한 함수포인터 리스트
/// 
/// vector를 사용한 함수포인터를 사용할경우는 개별적으로 삭제가필요없는경우 사용
/// map을 사용한 함수포인터를 사용할경우 중간에 어떠한 함수포인터만 빼고싶을경우 사용
///
/// 참고:https://modoocode.com/254


///1.Map을 사용한 함수포인터 리스트 
class Component;
template<typename T>
class Delegate_Map
{
public:
	Delegate_Map();
	~Delegate_Map();
	
	//키값을 넣으면 Map을 사용한 함수포인터로 사용한다
	
	void Push(T* ComponentKey, std::function<void()> temp);
	void Pop(T* Key);
	void Clear();
	

	//모든 함수 포인터 리스트 실행
	void Play();
private:
	std::map<T*,std::function<void()>> FunctionList;
};

template<typename T>
Delegate_Map<T>::Delegate_Map()
{


}

template<typename T>
Delegate_Map<T>::~Delegate_Map()
{


}

template<typename T>
//template<typename Key>
inline void Delegate_Map<T>::Push(T* ComponentKey, std::function<void()> temp)
{
	FunctionList.insert({ ComponentKey,temp });
}

template<typename T>
inline void Delegate_Map<T>::Pop(T* Key)
{
	FunctionList.erase(Key);
}

template<typename T>
inline void Delegate_Map<T>::Clear()
{
	FunctionList.clear();
}

template<typename T>
inline void Delegate_Map<T>::Play()
{
	//map의 처음부터 끝까지 순환
	auto list = FunctionList.begin();

	while (list != FunctionList.end())
	{
		if (list->second != nullptr)
		{
			list->second();
		}
		++list;
	}
}
