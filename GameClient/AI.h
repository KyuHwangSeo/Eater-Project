#pragma once

/// <summary>
/// 외부에서 생성한 컨퍼넌트가 제대로 들어가는지 확인하기위한 테스트 컨퍼넌트 
/// </summary>

#include "Component.h"
class AI : public Component
{
public:
	AI();
	virtual~AI();

	// Component을(를) 통해 상속됨
	virtual void Awake();
	virtual void Update();
private:
};