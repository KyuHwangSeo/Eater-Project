#pragma once
#include "EnumDefine.h"

/// 2021/12/08 2:06
/// SeoKyuHwang
///
/// # ResourceBase Class
///
/// - Graphic Resource Base Class
/// 
class ResourceBase
{
public:
	ResourceBase(eResourceType type) : m_ResourceType(type) {}

public:
	virtual void Reset() abstract;
	virtual void Release() abstract;

public:
	eResourceType GetType() { return m_ResourceType; }

private:
	eResourceType m_ResourceType;
};