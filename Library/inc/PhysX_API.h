#pragma once

#include "PhysEngineDLL.h"


///ENGINE 생성
//1. 사용할 쓰레드 개수
//2. PhysX에서 제공하는 Debug툴을 사용할것인지
extern "C" PHYS_ENGINEDLL void PhysX_Initialize(int ThreadCount,bool OnDebug);		//생성

///ENGINE 삭제
extern "C" PHYS_ENGINEDLL void PhysX_Release();

///ENGINE 업데이트
//1.프레임단위 시간 (델타 타임)
extern "C" PHYS_ENGINEDLL void PhysX_Update(float m_time);	//실행

///Dinamic 객체 생성
//1.생성할 데이터 정보를 담은 PhysData 구조체
//retrun 객체를 찾을때 필요한 인덱스 반환
extern "C"  PHYS_ENGINEDLL int PhysX_Create_DinamicActor(PhysData data);
extern "C"  PHYS_ENGINEDLL int PhysX_Create_StaticActor(PhysData data);


///Dinamic 객체 가져오기
//1.가져올 엑터의 데이터를 가져온다
//return 찾은 엑터의 데이터
extern "C"	PHYS_ENGINEDLL PhysData Get_DinamicActors(int index);
extern "C"	PHYS_ENGINEDLL PhysData Get_StaticActors(int index);


extern "C"  PHYS_ENGINEDLL void PhysX_CreateScene();					



