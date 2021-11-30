#pragma once
#include <unordered_map>
#include "EnumDefine.h"

#define DEFINE_CBUFFER			0x00000001
#define DEFINE_SAMPLER			0x00000010
#define DEFINE_SRV				0x00000100
#define DEFINE_UAV				0x00001000
#define DEFINE_DSV				0x00010000
#define DEFINE_DSS				0x00100000
#define DEFINE_RS				0x01000000
#define DEFINE_BS				0x10000000

typedef unsigned int	Define_Mask;
typedef size_t			Hash_Code;

/// 
/// ConstantBufferHashTable Class
/// 
/// - ConstantBuffer Struct 기준 Hash Code 키값으로 설정하기 위한 Class
/// - Shader Reflect Load 시 Description과 비교하기 위해 필요하므로 Shader Load 전에 초기화 필수
/// - 동적으로 해당 Resource Class 선언과 동시에 Hash Table에 올리고싶지만 아직은 방법이 떠오르지 않는다..
///   일단 Initalize부분에 귀찮더라도 해당 Resource Class를 올려두자..

class ShaderResourceHashTable
{
public:
	static ShaderResourceHashTable* Get();

	static ShaderResourceHashTable* instance;

public:
	// ConstantBuffer Hash Table..
	std::unordered_map<std::string, Hash_Code> g_CBuffer_HashTable;
	// Sampler Hash Table..
	std::unordered_map<std::string, Hash_Code> g_Sampler_HashTable;
	// ShaderResourceView Hash Table..
	std::unordered_map<std::string, Hash_Code> g_SRV_HashTable;
	// UnorderedAccessView Hash Table..
	std::unordered_map<std::string, Hash_Code> g_UAV_HashTable;

	// DepthStencilView Hash Table..
	std::unordered_map<std::string, Hash_Code> g_DSV_HashTable;
	// DepthStencilState Hash Table..
	std::unordered_map<std::string, Hash_Code> g_DSS_HashTable;
	// RasterizerState Hash Table..
	std::unordered_map<std::string, Hash_Code> g_RS_HashTable;
	// BlendState Hash Table..
	std::unordered_map<std::string, Hash_Code> g_BS_HashTable;

public:
	// Hash Code Push 함수..
	bool Push(eResourceType type, std::string name, Hash_Code hash_code);

	// 해당 Hash Code 반환 함수..
	size_t FindHashCode(eResourceType type, std::string cBufName);

	bool DefineCheck(Define_Mask nowDefine);

	// Hash Table Reset 함수..
	void Destroy();

private:
	// Hash Code Push Check 함수..
	bool CheckHashCode(std::unordered_map<std::string, Hash_Code>& table, std::string name, Hash_Code hash_code);

	Define_Mask DEFINE_MASK = 0x00000000;
};

inline bool ShaderResourceHashTable::Push(eResourceType type, std::string name, Hash_Code hash_code)
{
	switch (type)
	{
	case eResourceType::CB:
	{
		if (DEFINE_MASK & DEFINE_CBUFFER)	
			return false;
		else
			return CheckHashCode(g_CBuffer_HashTable, name, hash_code);
	}
	case eResourceType::SS:
	{
		if (DEFINE_MASK & DEFINE_SAMPLER)
			return false;
		else
		return CheckHashCode(g_Sampler_HashTable, name, hash_code);
	}
	case eResourceType::SRV:
	{
		if (DEFINE_MASK & DEFINE_SRV)
			return false;
		else
			return CheckHashCode(g_SRV_HashTable, name, hash_code);
	}
	case eResourceType::UAV:
	{
		if (DEFINE_MASK & DEFINE_UAV)
			return false;
		else
			return CheckHashCode(g_UAV_HashTable, name, hash_code);
	}
	case eResourceType::DSV:
	{
		if (DEFINE_MASK & DEFINE_DSV)
			return false;
		else
			return CheckHashCode(g_DSV_HashTable, name, hash_code);
	}
	case eResourceType::DSS:
	{
		if (DEFINE_MASK & DEFINE_DSS)
			return false;
		else
			return CheckHashCode(g_DSS_HashTable, name, hash_code);
	}
	case eResourceType::RS:
	{
		if (DEFINE_MASK & DEFINE_RS)
			return false;
		else
			return CheckHashCode(g_RS_HashTable, name, hash_code);
	}
	case eResourceType::BS:
	{
		if (DEFINE_MASK & DEFINE_BS)
			return false;
		else
			return CheckHashCode(g_BS_HashTable, name, hash_code);
	}
	default:
		break;
	}

	return true;
}