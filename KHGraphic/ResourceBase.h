#pragma once
#include "EnumDefine.h"

class ResourceBase
{
public:
	ResourceBase(eResourceType type) : m_ResourceType(type) {}

public:
	eResourceType GetType() { return m_ResourceType; }

private:
	eResourceType m_ResourceType;
};