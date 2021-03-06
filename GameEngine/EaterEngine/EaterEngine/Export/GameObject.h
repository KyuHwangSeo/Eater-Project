#pragma once

#include <string>
#include <vector>
#include <functional>

#include "EaterEngineDLL.h"
#include "Component.h"


#define AWAKE				0x00000001
#define START				0x00000010
#define SETUP				0x10000000
#define START_UPDATE		0x00000100
#define Transform_UPDATE	0x00001000
#define Physics_UPDATE		0x00010000
#define UPDATE				0x00100000
#define END_UPDATE			0x01000000


/// <summary>
/// 기본 게임 오브젝트
/// </summary>
class MeshData;
class Transform;
class GameObject
{
public:
	EATER_ENGINEDLL GameObject();
	~GameObject();

public:
	std::string Name;						//이름

	MeshData* OneMeshData;					//그래픽 엔진으로 넘겨줄 데이터
	Transform* transform;					//기본적으로 생성해주는 Transform 커너넌트
public:
	EATER_ENGINEDLL void SetActive(bool active);			//모든 컨퍼넌트 기능중지 여부

	EATER_ENGINEDLL GameObject* GetChildBone(std::string Name);	//자식객체를 가져옴
	EATER_ENGINEDLL GameObject* GetChildBone(int num);	//자식객체를 가져옴
	EATER_ENGINEDLL GameObject* GetChildMesh(std::string Name);	//자식객체를 가져옴
	EATER_ENGINEDLL GameObject* GetChildMesh(int num);
	EATER_ENGINEDLL	Transform* GetTransform();				//기본 컨퍼넌트인 Transform을 가져옴

	EATER_ENGINEDLL void ChoiceParent(GameObject* obj);		//나자신을 선택한 오브젝트의 자식으로 넣는다
	EATER_ENGINEDLL void ChoiceChild(GameObject* obj);		//선택한 오브젝트를 나의 자식으로 넣는다

public:
	Component* GetDeleteComponent(int i);	//삭제할 컨퍼넌트를 가져옴
	int GetComponentCount();				//오브젝트의 컨퍼넌트 갯수를 가져옴
	void PushChildList(GameObject* obj);	//오브젝트를 자식 리스트에 넣어준다

	void PushChildMeshObject(GameObject* obj, std::string Name = "Obj"); //나의 자식으로 기본 게임오브젝트를 넣는다
	void PushChildBoneObject(GameObject* obj, std::string Name = "Obj"); //나의 자식으로 기본 게임오브젝트를 넣는다
public:
	//컨퍼넌트를 추가 시킨다
	template<typename T>
	T* AddComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);
	
	//컨퍼넌트를 가져온다
	template<typename T>
	T* GetComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);
	
	//자식객체에서 찾고자하는 컨퍼넌트가있다면 그오브젝트에 컨퍼넌트를 가져옴
	template<typename T>
	T* GetChildComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t = std::is_base_of<Component, T>::value);
	
	//테그를 넣어준다
	template<typename T>
	void SetTag();
private:
	bool IsActive;							//기능 중지여부
	unsigned int FunctionMask;				//어떤 함수포인터에 넣었는지 여부
	size_t	Tag;							//오브젝트의 테그
private:
	std::vector<Component*> ComponentList;
	std::vector<GameObject*> ChildMeshList;
	std::vector<GameObject*> ChildBoneList;
	
	EATER_ENGINEDLL void PushComponentFunction(Component* con, unsigned int type);
};

template<typename T>
inline T* GameObject::AddComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t)
{
	//이함수는 무조건 Component를 상속받은 클래스만 들어올수있다
	//그래서 Component 안에 함수만 사용함

	T* ComponentBox = new T();
	//게임오브젝트 설정
	ComponentBox->gameobject = this;

	//생성한 컨퍼넌트를 리스트에 넣는다
	ComponentList.push_back(ComponentBox);


	//게임오브젝트 추가 데이터 설정
	ComponentBox->SetObjectData();

	//나중에 이타입으로 찾아서 가져올수있도록 타입 설정
	ComponentBox->ComponentType = typeid(T).hash_code();

	///오버라이딩 확인 각각에맞는 함수포인터 리스트에 넣는다 
	//Awake
	if (typeid(&Component::Awake).hash_code() != typeid(&T::Awake).hash_code())
	{
		PushComponentFunction(ComponentBox, AWAKE);
	}

	//Start
	if (typeid(&Component::Start).hash_code() != typeid(&T::Start).hash_code())
	{
		PushComponentFunction(ComponentBox, START);
	}

	//StartUpdate
	if (typeid(&Component::SetUp).hash_code() != typeid(&T::SetUp).hash_code())
	{
		PushComponentFunction(ComponentBox, SETUP);
	}

	//StartUpdate
	if (typeid(&Component::StartUpdate).hash_code() != typeid(&T::StartUpdate).hash_code())
	{
		PushComponentFunction(ComponentBox, START_UPDATE);
	}

	//이동 행렬
	if (typeid(&Component::TransformUpdate).hash_code() != typeid(&T::TransformUpdate).hash_code())
	{
		PushComponentFunction(ComponentBox, Transform_UPDATE);
	}

	//물리 
	if (typeid(&Component::PhysicsUpdate).hash_code() != typeid(&T::PhysicsUpdate).hash_code())
	{
		PushComponentFunction(ComponentBox, Physics_UPDATE);
	}

	//DefaultUpdate
	if (typeid(&Component::Update).hash_code() != typeid(&T::Update).hash_code())
	{
		PushComponentFunction(ComponentBox, UPDATE);
	}

	//EndUpdate
	if (typeid(&Component::EndUpdate).hash_code() != typeid(&T::EndUpdate).hash_code())
	{
		PushComponentFunction(ComponentBox, END_UPDATE);
	}

	return ComponentBox;
}

template<typename T>
inline T* GameObject::GetComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t)
{
	//같은 컨퍼넌트가 2개가있으면 찾지못한다
	int count = (int)ComponentList.size();
	for (int i = 0; i < count; i++)
	{
		//현재 컨퍼넌트와 찾고자하는 컨퍼넌트의 타입 비교
		if (ComponentList[i]->ComponentType == typeid(T).hash_code())
		{
			//찾은 컨퍼넌트를 타입변환
			T* temp = dynamic_cast<T*>(ComponentList[i]);
			return temp;
		}
	}

	//만약 찾지못했다면 에러코드를 내보내주면될듯..
	return nullptr;
}

template<typename T>
inline T* GameObject::GetChildComponent(typename std::enable_if<std::is_base_of<Component, T>::value, bool>::type t)
{
	//자식오브젝트를 돌다가 가장 먼저찾게되는 컨퍼넌트를 가져온다
	std::vector<GameObject*>::iterator it = ChildMeshList.begin();
	for (it; it != ChildMeshList.end(); it++)
	{
		int ComSize = (*it).ComponentList.size();
		for (int i = 0; i < ComSize; i++)
		{
			//현재 컨퍼넌트와 찾고자하는 컨퍼넌트의 타입 비교
			if (ComponentList[i]->ComponentType == typeid(T).hash_code())
			{
				//찾은 컨퍼넌트를 타입변환
				T* temp = dynamic_cast<T*>(ComponentList[i]);
				return temp;
			}
		}
	}
	return nullptr;
}

template<typename T>
inline void GameObject::SetTag()
{
	//클래스를 해쉬코드값으로 변경후 저장
	Tag = typeid(T).hash_code();
}