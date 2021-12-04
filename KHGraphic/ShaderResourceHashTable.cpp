#include "ShaderResourceHashTable.h"
#include "ConstantBufferDefine.h"
#include "SamplerBufferDefine.h"
#include "ShaderResourceBufferDefine.h"
#include "UnorderedAccessBufferDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "RasterizerStateDefine.h"
#include "BlendStateDefine.h"

ShaderResourceHashTable* ShaderResourceHashTable::instance = nullptr;

ShaderResourceHashTable* ShaderResourceHashTable::Get()
{
	if (instance == nullptr)
	{
		instance = new ShaderResourceHashTable();
	}

	return instance;
}

bool ShaderResourceHashTable::Push(eResourceType type, std::string name, Hash_Code hash_code)
{
	switch (type)
	{
	case eResourceType::CB:
	{
		if (DEFINE_MASK & DEFINE_CB)
			return false;
		else
			return CheckHashCode(g_CBuffer_HashTable, name, hash_code);
	}
	case eResourceType::SS:
	{
		if (DEFINE_MASK & DEFINE_SS)
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
	default:
		break;
	}

	return true;
}

size_t ShaderResourceHashTable::FindHashCode(eResourceType type, std::string cBufName)
{
	std::unordered_map<std::string, Hash_Code>::iterator cHash;
	
	// Resource Type에 따른 Hash Code 반환..
	switch (type)
	{
	case eResourceType::CB:
	{
		cHash = g_CBuffer_HashTable.find(cBufName);
	
		if (cHash == g_CBuffer_HashTable.end())
		{
			return 0;
		}
	}
	break;
	case eResourceType::SS:
	{
		cHash = g_Sampler_HashTable.find(cBufName);
	
		if (cHash == g_Sampler_HashTable.end())
		{
			return 0;
		}
	}
	break;
	case eResourceType::SRV:
	{
		cHash = g_SRV_HashTable.find(cBufName);
	
		if (cHash == g_SRV_HashTable.end())
		{
			return 0;
		}
	}
	break;
	case eResourceType::UAV:
	{
		cHash = g_UAV_HashTable.find(cBufName);
	
		if (cHash == g_UAV_HashTable.end())
		{
			return 0;
		}
	}
	break;
	default:
		return 0;
	}
	
	return cHash->second;
}

bool ShaderResourceHashTable::DefineCheck(Define_Mask nowDefine)
{
	DEFINE_MASK |= nowDefine;

	return true;
}

void ShaderResourceHashTable::Destroy()
{
	DEFINE_MASK = 0x00000000;

	g_CBuffer_HashTable.clear();
	g_Sampler_HashTable.clear();
	g_SRV_HashTable.clear();
	g_UAV_HashTable.clear();
}

bool ShaderResourceHashTable::CheckHashCode(std::unordered_map<std::string, Hash_Code>& table, std::string name, Hash_Code hash_code)
{
	if (table.find(name) == table.end())
	{
		table.insert(std::make_pair(name, hash_code));
	}

	return true;
}
