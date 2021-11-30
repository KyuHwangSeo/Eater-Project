#pragma once
#include <string>
#include <typeinfo>
#include "ResourceBufferHashTable.h"

#define CREATE_EMPTY_CLASS(ClassName, ResourceName) struct ClassName : public HashClass<ClassName>, public ResourceName {};
#define RESOURCE_DEFINE(ResourceMask) static bool check_##ResourceMask = ShaderResourceHashTable::Get()->DefineCheck((int)ResourceMask);

/// <summary>
/// BufferName Struct
/// </summary>
/// 
/// - 모든 Shader Resource의 기반이 되는 Base Class
/// - Shader Reflection을 통해 Resource의 이름을 비교해야 하기 때문에 Struct 이름만 가져오는 함수 보유
///

template <typename T>
struct HashClass
{
	static std::string GetName()
	{
		std::string name = typeid(T).name();
		size_t spacePosition = name.find_first_of(" ");
		if (spacePosition != std::string::npos)
			return name.substr(spacePosition + 1, name.length());
		else
			return name;
	}
	
	static size_t GetHashCode()
	{
		return typeid(T).hash_code();
	}
};

struct CB_Resource
{
	static eResourceType GetType() { return eResourceType::CB;	}
};

struct SRV_Resource
{
	static eResourceType GetType() { return eResourceType::SRV; }
};

struct UAV_Resource
{
	static eResourceType GetType() { return eResourceType::UAV; }
};

struct DSV_Resource
{
	static eResourceType GetType() { return eResourceType::DSV; }
};

struct DSS_Resource
{
	static eResourceType GetType() { return eResourceType::DSS; }
};

struct SS_Resource
{
	static eResourceType GetType() { return eResourceType::SS; }
};

struct RS_Resource
{
	static eResourceType GetType() { return eResourceType::RS; }
};

struct BS_Resource
{
	static eResourceType GetType() { return eResourceType::BS; }
};

struct RT_Resource
{
	static eResourceType GetType() { return eResourceType::RT; }
};

struct VP_Resource
{
	static eResourceType GetType() { return eResourceType::VP; }
};