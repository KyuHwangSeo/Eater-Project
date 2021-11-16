#include "Component.h"
#include "GameObject.h"

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

