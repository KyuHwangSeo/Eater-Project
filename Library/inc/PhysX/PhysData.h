#pragma once
#include "PhysEngineDLL.h"
#include "SimpleMath.h"
#include <vector>

//typedef unsigned int UINT;
 enum class SHAPE_TYPE
{
	BOX,
	SPHERE,
	CAPSULE,
	TRIANGLE
};

 enum class ACTOR_TYPE
 {
	 DINAMIC,
	 STATIC,
	 KNEMATIC,
 };

///페이스 기준으로 콜라이더를 생성하고싶을때 필요한 데이터
struct TriangleMeshData 
{
	std::vector<DirectX::SimpleMath::Vector3>* VertexList;
	std::vector<UINT>* IndexList;
};

///하나의 강체를 만드는 구조체 무슨내용인지 보고싶으면 아래쪽에 설명
///사용하지 않는 값은 그냥 디폴트값으로 놔두면된다
struct Vec
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f; //회전값 있을때만
};

///하나의 강체를 만드는 구조체 무슨내용인지 보고싶으면 아래쪽에 설명
///사용하지 않는 값은 그냥 디폴트값으로 놔두면된다
class PHYS_ENGINEDLL PhysData
{
public:
	PhysData();
	~PhysData();
	//월드 위치
	Vec WorldPosition;
	//회전
	Vec Rotation;
	//이객체의 무게 중심점
	Vec CenterPoint;
	//월드상의 위치
	void SetWorldPosition(float x, float y, float z);
	void SetLocalPosition(float x, float y, float z);
	void SetTranlate(float x, float y, float z);

	//월드상의 회전
	void SetRotation(float x, float y, float z);
	void SetRotate(float x, float y, float z);
	void AddForce(float x, float y, float z);
public:
	///재질(메테리얼) 데이터
	//정지 마찰력
	float MT_StaticFriction;
	//운동 마찰력
	float MT_DynamicFriction;
	//복원력
	float MT_Restitution;
	//무게
	float MT_Mass;
public:
	///객체 정보 데이터
	//움직이는 객체인지 (Dinamic , Static)
	bool isDinamic;
	//중력 작용 여부
	bool isGrvity;
	//움직이진 않는데 충돌 할것인지
	bool isKinematic;
public:
	//가로 세로 높이
	void CreateBoxCollider(float x, float y, float z);
	//반지름
	void CreateBoxCollider(float Radius);
	//반지름
	void CreateSphereCollider(float Radius);
	//반지름,높이
	void CreateCapsuleCollider(float Radius, float Height);
	//페이스기준으로 콜라이더를 만들때
	void CreateTriangleCollider(TriangleMeshData*  Data);
private:
	///위치 데이터
	Vec MovePoint;
	//로컬 위치 (계층 구조일때)
	Vec LocalPosition;
	//어떠한 축 이동을 고정 시킬때 사용 
	Vec FreezePositon;
	//어떠한 축 회전을 고정 시킬때 사용
	Vec FreezeRotaticon;
	//이객체의 이동속력을 넣어준다
	Vec Velocity;
	//현재 벡터 방향으로 힘을 줄때 사용
	Vec Force;
private:
	///콜라이더 데이터
	Vec Shape_Size;			//사이즈
	SHAPE_TYPE	Shape_type; //타입
private:
	bool isMove;
	bool isForce;
private:
	void* ActorObj;
	TriangleMeshData* Triangle;
	friend class Factory;
	friend class PhysEngine;
};

//Phys 공간의 월드 데이터
struct PhysSceneData
{
	

};





/*
*
 복원력 (Restitution)
	당구공을 예로 들자면 움직이는 공이 가만히있는 공과 충돌했을떄 움직이는 공은 멈추고
	멈춰있던 공은 움직이게 된다 이때 작용되는것이 복원력 1일수록 충돌하였을떄 속도가 보존된다
	0이라면 충돌 하자마자 멈춘다 즉 1 = 완전 탄성  0 = 비탄성 충돌

 정지 마찰력 (StaticFriction) , 운동 마찰력 (DynamicFriction)
	물체가 움직이기 위한 마찰력을 의미 예를 들자면 가만히 있는 자동차를 처음움직이기는 힘들지만
	움직이기 시작하면 큰힘이 들지않는다 물체가 움직이기 시작하면 운동 마찰력이 적용된다 많은 경우에
	운동 마찰력은 정지 마찰력보다 작기때문에 
	
	F(힘) = u(마찰계수) * m(질량) * g(중력) 

Dinamic 객체
	힘에 의해서 움직일 수도 있고 충돌도 가능한 객체

Knematic 객체
	한 위치에서 다른 위치에서 이동은 가능하지만 힘에 의해서
	움직일 수 있는 객체는 아니다

	무한 질량을 가진 물체로 간주하며 moveGlobal 함수로 움직일수 있다
	일반적인 Dynamic 액터를 밀수도 있다 하지만 static 과 다른 Knematic 물체와 충돌하지않음

Static 객체
	이객체는 어떠한 방법을 쓰더라도 움직일 수 없는 객체이다
	고정된 물체를 의미한다
*/
