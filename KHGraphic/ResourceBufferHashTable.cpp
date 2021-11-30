#include "ResourceBufferHashTable.h"
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
	case eResourceType::DSV:
	{
		cHash = g_DSV_HashTable.find(cBufName);

		if (cHash == g_DSV_HashTable.end())
		{
			return 0;
		}
	}
	break;
	case eResourceType::DSS:
	{
		cHash = g_DSS_HashTable.find(cBufName);

		if (cHash == g_DSS_HashTable.end())
		{
			return 0;
		}
	}
	break;
	case eResourceType::RS:
	{
		cHash = g_RS_HashTable.find(cBufName);

		if (cHash == g_RS_HashTable.end())
		{
			return 0;
		}
	}
	break;
	case eResourceType::BS:
	{
		cHash = g_BS_HashTable.find(cBufName);

		if (cHash == g_BS_HashTable.end())
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

bool ShaderResourceHashTable::DefineCheck(Define_Mask nowDefine)
{
	DEFINE_MASK |= nowDefine;

	return true;
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
	g_DSV_HashTable.clear();
	g_DSS_HashTable.clear();
	g_RS_HashTable.clear();
	g_BS_HashTable.clear();
}
