#pragma once
#include "EaterEngineDLL.h"
#include "Component.h"
class Collider : public Component
{
public:
	EATER_ENGINEDLL Collider();
	EATER_ENGINEDLL ~Collider();

	//겹쳐지면서 충돌하면서 체크할것인지
	EATER_ENGINEDLL void SetTrigger(bool trigger);
	//재질 설정
	EATER_ENGINEDLL void SetMaterial();
	//중심정 설정
	EATER_ENGINEDLL void SetCenter(float x, float y, float z);
	//크기 설정
	EATER_ENGINEDLL void SetSize(float x, float y, float z);
protected:
	XMFLOAT3 Center;
	XMFLOAT3 Size;
	
	bool isTrigger;

	//메테리얼 정보
private:
};

