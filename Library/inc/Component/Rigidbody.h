#pragma once

#include "EaterEngineDLL.h"
#include "PhysX_API.h"
#include "Component.h"

class Transform;
class Rigidbody : public Component
{
public:
	

	EATER_ENGINEDLL Rigidbody();
	EATER_ENGINEDLL ~Rigidbody();

	virtual void Start();
	virtual void TransformUpdate();

	EATER_ENGINEDLL void SetType(int Type);

	static const int DINAMIC	= 0;
	static const int STATIC		= 1;
private:
	int ACTOR_TYPE;

	Transform* Tr;
	int MyIndex		= 0;
	int typeIndex	= 0;
};

