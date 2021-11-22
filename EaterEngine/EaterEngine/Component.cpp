#include "Component.h"
#include "TimeManager.h"
#include "KeyinputManager.h"
#include "GameObject.h"

TimeManager* Component::mTimeManager = nullptr;
KeyinputManager* Component::mKeyInputManger = nullptr;
Component::Component()
{
	gameobject = nullptr;
	ComponentType = 0;
}

Component::~Component()
{
	gameobject = nullptr;
}


void Component::SetComponentType(size_t type)
{
	ComponentType = type;
}

size_t Component::GetComponentType()
{
	return ComponentType;
}

void Component::SetObject(GameObject* obj)
{
	gameobject = obj;
}

void Component::SetManager(TimeManager* time, KeyinputManager* key)
{
	mTimeManager = time;
	mKeyInputManger = key;
}

