#pragma once
/// <summary>
/// 컨퍼넌트들의 최상위 클래스
/// 이클래스를 상속하고 만든 컨퍼넌트들은
/// 나중에 게임오브젝트를 만들고 AddComponent를 할떄 오버라이딩을 했는지 안했는지 검사 후
/// 업데이트 함수 포인터 리스트에 넣어줌
/// </summary>

#include "EaterEngineDLL.h"

class GameObject;
class Component
{
public:
	EATER_ENGINEDLL Component();
	virtual EATER_ENGINEDLL ~Component();

	//시작 단계에 가장먼저 실행되는 함수
	virtual void Awake() {};
	//시작 함수
	virtual void Start() {};
	//처음
	virtual void StartUpdate() {};
	//이동 행렬 업데이트
	virtual void TransformUpdate() {};
	//물리 업데이트
	virtual void PhysicsUpdate() {};
	//일반 업데이트
	virtual void Update() {};
	//마지막 업데이트
	virtual void EndUpdate() {};

	
	//현재 이컨퍼넌트의 어떤함수가 오버라이딩되어있는지 확인하기위해
	unsigned int FUNCTION_MASK = 0x00000000;
	//클래스의 타입 GetComponent에서 쓸떄 사용
public:
	//컨퍼넌트 타입을 넣어준다
	EATER_ENGINEDLL void SetComponentType(size_t type);
	//컨퍼넌트 타입을 받아온다
	EATER_ENGINEDLL size_t GetComponentType();
	//오브젝트를 넣어준다
	EATER_ENGINEDLL void SetObject(GameObject* obj);
private:
protected:
	//이컨퍼넌트를 가지고 있는 게임오브젝트
	GameObject* gameobject;
	size_t ComponentType;
};



