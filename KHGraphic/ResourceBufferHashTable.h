#pragma once
#include <unordered_map>
#include "SingleTon.h"

typedef size_t Hash_Code;

/// 
/// ConstantBufferHashTable Class
/// 
/// - ConstantBuffer Struct 기준 Hash Code 키값으로 설정하기 위한 Class
/// - Shader Reflect Load 시 Description과 비교하기 위해 필요하므로 Shader Load 전에 초기화 필수
/// - 동적으로 해당 Resource Class 선언과 동시에 Hash Table에 올리고싶지만 아직은 방법이 떠오르지 않는다..
///   일단 Initalize부분에 귀찮더라도 해당 Resource Class를 올려두자..

class ShaderResourceHashTable : public Singleton<ShaderResourceHashTable>
{
public:
	enum class BufferType
	{
		CBUFFER,
		SAMPLER,
		SRV,
		UAV
	};

public:
	// ConstantBuffer Hash Table..
	std::unordered_map<std::string, Hash_Code> g_CBuffer_HashTable;
	// Sampler Hash Table..
	std::unordered_map<std::string, Hash_Code> g_Sampler_HashTable;
	// ShaderResourceView Hash Table..
	std::unordered_map<std::string, Hash_Code> g_SRV_HashTable;
	// UnorderedAccessView Hash Table..
	std::unordered_map<std::string, Hash_Code> g_UAV_HashTable;

	// Hash Table Reset 함수..
	void Reset();

	// Hash Code Push 함수..
	template<typename T> bool Push(BufferType type, std::string name, Hash_Code hash_code);

	// 해당 Hash Code 반환 함수..
	size_t FindHashCode(BufferType type, std::string cBufName);

private:
	// Hash Code Push Check 함수..
	bool CheckHashCode(std::unordered_map<std::string, Hash_Code>& table, std::string name, Hash_Code hash_code);
};

template<typename T>
bool ShaderResourceHashTable::Push(BufferType type, std::string name, Hash_Code hash_code)
{
	switch (type)
	{
	case ShaderResourceHashTable::BufferType::CBUFFER:
		return CheckHashCode(g_CBuffer_HashTable, name, hash_code);
	case ShaderResourceHashTable::BufferType::SAMPLER:
		return CheckHashCode(g_Sampler_HashTable, name, hash_code);
	case ShaderResourceHashTable::BufferType::SRV:
		return CheckHashCode(g_SRV_HashTable, name, hash_code);
	case ShaderResourceHashTable::BufferType::UAV:
		return CheckHashCode(g_UAV_HashTable, name, hash_code);
	default:
		break;
	}

	return false;
}