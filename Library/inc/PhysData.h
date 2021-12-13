#pragma once

 enum class COLLIDER_TYPE
{
	BOX,
	SPHERE,
};

 enum class ACTOR_TYPE
 {
	 DINAMIC,
	 STATIC,
	 KNEMATIC,
 };


struct Vec
{
	float x;
	float y;
	float z;
	float w = 0; //회전값 있을때만
};

///하나의 강체를 만드는 구조체 무슨내용인지 보고싶으면 아래쪽에 설명 
struct PhysData
{
	// 위치
	Vec WorldPosition = { 0,0,0 };
	Vec LocalPosition = { 0,0,0 };

	// 크기
	Vec Scale = {0.5f,0.5f, 0.5f};

	//회전
	Vec Rotation = { 0,0,0,0 };
	

	//강체의 재질(메테리얼) 데이터
	float MT_StaticFriction		= 0.5f;	//정지 마찰력
	float MT_DynamicFriction	= 0.5f;	//운동 마찰력
	float MT_Restitution		= 0.6f;	//복원력
	float MT_Mass				= 1;	//무게


	void Collider_length(COLLIDER_TYPE _type, float x, float y, float z)
	{
		Collider_type = _type;
		Col.x = x;
		Col.y = y;
		Col.z = z;
	}
	void Collider_length(COLLIDER_TYPE _type, float length = 0.5f)
	{
		Collider_type = _type;
		Col.x = length;
		Col.y = length;
		Col.z = length;
	}
	
	Vec Col;
	COLLIDER_TYPE	Collider_type;
	ACTOR_TYPE		Actor_TYPE;
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
