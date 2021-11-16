#pragma once

#include <queue>

template <class T>
class Non_ThreadObjectPool
{
private:
	int m_Pool_Size = 0;
	std::queue<T*> Object_Manage_Queue;

public:
	Non_ThreadObjectPool(int _Pool_Size = 1000);
	~Non_ThreadObjectPool();

	// 오브젝트 하나를 받아옴.
	T* GetObject();
	// 밖에서 다 쓴 후 생성자를 호출해 초기화 하여 반납하는 경우.
	void ResetObject(T* _Used_Object);
	// 초기값을 밖에서 셋팅해서 큐에 넣어야 되는 경우..
	void Non_ResetObject(T* _Used_Object);
};

template <class T>
Non_ThreadObjectPool<T>::Non_ThreadObjectPool(int _Pool_Size /* = 1000 */)
{
	m_Pool_Size = _Pool_Size;
	// Pool_Size 만큼 생성해서 큐에 넣어둠.
	for (int i = 0; i < _Pool_Size; i++)
	{
		T* Pool_Data = new T;
		Object_Manage_Queue.push(Pool_Data);
	}
}

template <class T>
Non_ThreadObjectPool<T>::~Non_ThreadObjectPool()
{

}

template <class T>
T* Non_ThreadObjectPool<T>::GetObject()
{
	// 큐가 비어있으면 여유분 생성.
	if (Object_Manage_Queue.empty())
	{
		for (int i = 0; i < m_Pool_Size / 10; i++)
		{
			T* Pool_Data = new T;
			Object_Manage_Queue.push(Pool_Data);
		}
	}

	T* return_Value = Object_Manage_Queue.front();
	Object_Manage_Queue.pop();

	return return_Value;
}

template <class T>
void Non_ThreadObjectPool<T>::ResetObject(T* _Used_Object)
{
	// 소멸자 호출.
	_Used_Object->~T();
	// 생성자 호출.
	T* Reset_Object = new (_Used_Object) T;

	// 초기화 후 push
	Object_Manage_Queue.push(_Used_Object);
}

template <class T>
void Non_ThreadObjectPool<T>::Non_ResetObject(T* _Used_Object)
{
	// 초기화를 진행하지 않고 push
	Object_Manage_Queue.push(_Used_Object);
}