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
	ResourceBase(RESOURCE_TYPE type) : m_ResourceType(type) {}

public:
	virtual void Reset() abstract;
	virtual void Release() abstract;

public:
	RESOURCE_TYPE GetType() { return m_ResourceType; }

private:
	RESOURCE_TYPE m_ResourceType;
};