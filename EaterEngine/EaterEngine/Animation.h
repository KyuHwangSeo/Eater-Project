#pragma once

/// <summary>
/// 본에 애니메이션을 돌리기위해 본마다 들어있는 컨퍼넌트 
/// </summary>

namespace ParserData
{
	struct OneAnimation;
}


#include "Component.h"
class Animation : public Component
{
public:
	void Update();

	void SetAnimation(ParserData::OneAnimation*);
private:
	ParserData::OneAnimation* AnimationData;


};