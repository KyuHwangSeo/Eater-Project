#include "ResourceBufferHashTable.h"
#include "ConstantBufferDefine.h"
#include "SamplerBufferDefine.h"
#include "ShaderResourceBufferDefine.h"
#include "UnorderedAccessBufferDefine.h"

ShaderResourceHashTable* ShaderResourceHashTable::instance = nullptr;

ShaderResourceHashTable* ShaderResourceHashTable::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ShaderResourceHashTable();
	}

	return instance;
}

size_t ShaderResourceHashTable::FindHashCode(BufferType type, std::string cBufName)
{
	std::unordered_map<std::string, Hash_Code>::iterator cHash;
	
	switch (type)
	{
	case BufferType::CBUFFER:
	{
		cHash = g_CBuffer_HashTable.find(cBufName);
	
		if (cHash == g_CBuffer_HashTable.end())
		{
			return 0;
		}
	}
	break;
	case BufferType::SAMPLER:
	{
		cHash = g_Sampler_HashTable.find(cBufName);
	
		if (cHash == g_Sampler_HashTable.end())
		{
			return 0;
		}
	}
	break;
	case BufferType::SRV:
	{
		cHash = g_SRV_HashTable.find(cBufName);
	
		if (cHash == g_SRV_HashTable.end())
		{
			return 0;
		}
	}
	break;
	case BufferType::UAV:
	{
		cHash = g_UAV_HashTable.find(cBufName);
	
		if (cHash == g_UAV_HashTable.end())
		{
			return 0;
		}
	}
	break;
	default:
	{
		return 0;
	}
	break;
	}
	
	
	return cHash->second;
}

bool ShaderResourceHashTable::CheckHashCode(std::unordered_map<std::string, Hash_Code>& table, std::string name, Hash_Code hash_code)
{
	if (table.find(name) == table.end())
	{
		table.insert(std::make_pair(name, hash_code));
	}

	return true;
}

void ShaderResourceHashTable::Destroy()
{
	g_CBuffer_HashTable.clear();
	g_Sampler_HashTable.clear();
	g_SRV_HashTable.clear();
	g_UAV_HashTable.clear();
}
