#pragma once
#include "Component.h"
#include "EaterEngineDLL.h"

/// <summary>
/// Animator들을 관리하는 클래스
/// 최상위 오브젝트에만 생성될것
/// </summary>

class AnimationController : public Component
{
public:
	EATER_ENGINEDLL AnimationController();
	virtual ~AnimationController();

	void Awake();
};

