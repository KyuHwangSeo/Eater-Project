#include "ResourceBufferHashTable.h"
#include "ConstantBufferDefine.h"
#include "SamplerBufferDefine.h"
#include "ShaderResourceBufferDefine.h"
#include "UnorderedAccessBufferDefine.h"

void ShaderResourceHashTable::Reset()
{
	g_CBuffer_HashTable.clear();
	g_Sampler_HashTable.clear();
	g_SRV_HashTable.clear();
	g_UAV_HashTable.clear();
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
			throw std::exception("ERROR: Can not find Constant Buffer Hash Code.\n");
			return 0;
		}
	}
	break;
	case BufferType::SAMPLER:
	{
		cHash = g_Sampler_HashTable.find(cBufName);
	
		if (cHash == g_Sampler_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Constant Buffer Hash Code.\n");
			return 0;
		}
	}
	break;
	case BufferType::SRV:
	{
		cHash = g_SRV_HashTable.find(cBufName);
	
		if (cHash == g_SRV_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Shader Resource View Hash Code.\n");
			return 0;
		}
	}
	break;
	case BufferType::UAV:
	{
		cHash = g_UAV_HashTable.find(cBufName);
	
		if (cHash == g_UAV_HashTable.end())
		{
			throw std::exception("ERROR: Can not find Unordered Access View Hash Code.\n");
			return 0;
		}
	}
	break;
	default:
	{
		throw std::exception("ERROR: Can not find Resource Type.\n");
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
		return true;
	}

	return false;
}
