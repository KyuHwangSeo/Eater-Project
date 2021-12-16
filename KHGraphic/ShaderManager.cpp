#include "DirectDefine.h"
#include "D3D11GraphicBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ShaderManagerBase.h"
#include "ShaderManager.h"
#include "ShaderResourceHashTable.h"

using namespace Microsoft::WRL;

ShaderManager::ShaderManager(ID3D11Graphic* graphic)
{
	// Shader Global Initialize..
	IShader::Initialize(graphic->GetDevice(), graphic->GetContext());
}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::Initialize()
{
	// Shader File Route Setting..
	IShader::SetShaderRoute("../Resources/Shader/SKH/");

	// Global Shader Create..
	CreateShader();
}

void ShaderManager::Release()
{
	IShader::Reset();

	for (std::pair<std::string, ShaderBase*> shader : m_ShaderList)
	{
		RELEASE_COM(shader.second);
	}

	m_ShaderList.clear();
}

VertexShader* ShaderManager::GetVertexShader(std::string shaderName)
{
	std::unordered_map<std::string, ShaderBase*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader가 없을 경우..
	if (shader == m_ShaderList.end()) return nullptr;

	switch (shader->second->GetType())
	{
	case eShaderType::VERTEX:
		return reinterpret_cast<VertexShader*>(shader->second);
	default:
		return nullptr;
	}
}

PixelShader* ShaderManager::GetPixelShader(std::string shaderName)
{
	std::unordered_map<std::string, ShaderBase*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader가 없을 경우..
	if (shader == m_ShaderList.end()) return nullptr;

	switch (shader->second->GetType())
	{
	case eShaderType::PIXEL:
		return reinterpret_cast<PixelShader*>(shader->second);
	default:
		return nullptr;
	}
}

ComputeShader* ShaderManager::GetComputeShader(std::string shaderName)
{
	std::unordered_map<std::string, ShaderBase*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader가 없을 경우..
	if (shader == m_ShaderList.end()) return nullptr;

	switch (shader->second->GetType())
	{
	case eShaderType::COMPUTE:
		return reinterpret_cast<ComputeShader*>(shader->second);
	default:
		return nullptr;
	}
}

void ShaderManager::CreateShader()
{
	// Shader Macro
	D3D_SHADER_MACRO light0_macro[] = { {"GAMMA_CORRECTION"}, {"SHADOW"}, {"SSAO"}, {NULL, NULL} };
	D3D_SHADER_MACRO light1_macro[] = { {"SHADOW"}, {"SSAO"}, {NULL, NULL} };
	D3D_SHADER_MACRO light2_macro[] = { {"GAMMA_CORRECTION"}, {"SSAO"}, {NULL, NULL} };
	D3D_SHADER_MACRO light3_macro[] = { {"GAMMA_CORRECTION"}, {"SHADOW"}, {NULL, NULL} };
	D3D_SHADER_MACRO light4_macro[] = { {"GAMMA_CORRECTION"}, {NULL, NULL} };
	D3D_SHADER_MACRO light5_macro[] = { {"SHADOW"}, {NULL, NULL} };
	D3D_SHADER_MACRO light6_macro[] = { {"SSAO"}, {NULL, NULL} };

	// Light Shader
	LoadShader(eShaderType::VERTEX, "LightVS.hlsl", "Light_VS");
	LoadShader(eShaderType::PIXEL, "LightPS.hlsl", "Light_PS", "Light_PS_Option0" ,light0_macro);
	LoadShader(eShaderType::PIXEL, "LightPS.hlsl", "Light_PS", "Light_PS_Option1" ,light1_macro);
	LoadShader(eShaderType::PIXEL, "LightPS.hlsl", "Light_PS", "Light_PS_Option2" ,light2_macro);
	LoadShader(eShaderType::PIXEL, "LightPS.hlsl", "Light_PS", "Light_PS_Option3" ,light3_macro);
	LoadShader(eShaderType::PIXEL, "LightPS.hlsl", "Light_PS", "Light_PS_Option4" ,light4_macro);
	LoadShader(eShaderType::PIXEL, "LightPS.hlsl", "Light_PS", "Light_PS_Option5" ,light5_macro);
	LoadShader(eShaderType::PIXEL, "LightPS.hlsl", "Light_PS", "Light_PS_Option6" ,light6_macro);

	//D3D_SHADER_MACRO terrain_macro[] = { {"TERRAIN_MESH"}, {NULL, NULL} };
	//D3D_SHADER_MACRO skin_macro[] = { {"SKIN_MESH"}, {NULL, NULL} };

	// Object Shader
	LoadShader(eShaderType::VERTEX, "ObjectVS.hlsl", "Mesh_VS");
	LoadShader(eShaderType::VERTEX, "ObjectVS.hlsl", "Skin_VS");

	// Deffered Shader
	LoadShader(eShaderType::PIXEL, "DeferredPS.hlsl", "Deferred_PS");

	// Terrain Shader
	LoadShader(eShaderType::VERTEX, "TerrainVS.hlsl", "Terrain_VS");
	LoadShader(eShaderType::PIXEL, "TerrainPS.hlsl", "Terrain_PS");

	// Forward Shader
	LoadShader(eShaderType::PIXEL, "ForwardPS.hlsl", "Forward_PS");

	// Shadow Shader
	LoadShader(eShaderType::VERTEX, "ShadowVS.hlsl", "MeshShadow_VS");
	LoadShader(eShaderType::VERTEX, "ShadowVS.hlsl", "SkinShadow_VS");

	// SSAO Shader
	LoadShader(eShaderType::VERTEX, "SSAOVS.hlsl", "SSAO_VS");
	LoadShader(eShaderType::PIXEL, "SSAOPS.hlsl", "SSAO_PS");
	LoadShader(eShaderType::VERTEX, "SSAOBlurVS.hlsl", "SSAOBlur_VS");
	LoadShader(eShaderType::PIXEL, "SSAOBlurPS.hlsl", "SSAOBlur_PS");
}

void ShaderManager::AddSampler(Hash_Code hash_code, ID3D11SamplerState* sampler)
{
	for (std::pair<std::string, ShaderBase*> shader : m_ShaderList)
	{
		ShaderBase* pShader = shader.second;

		switch (pShader->GetType())
		{
		case eShaderType::VERTEX:
		case eShaderType::PIXEL:
		case eShaderType::COMPUTE:
		{
			pShader->SetSamplerState(hash_code, sampler);
		}
		break;
		default:
			break;
		}
	}
}

ShaderBase* ShaderManager::LoadShader(eShaderType shaderType, std::string shaderName, std::string entry_point, std::string shaderKey, const D3D_SHADER_MACRO* pDefines)
{
	// Shader Type에 맞는 Shader 생성..
	ShaderBase* newShader = nullptr;

	switch (shaderType)
	{
	case eShaderType::VERTEX:
		newShader = new VertexShader(shaderName.c_str(), entry_point.c_str(), "vs_5_0", pDefines);
		break;
	case eShaderType::PIXEL:
		newShader = new PixelShader(shaderName.c_str(), entry_point.c_str(), "ps_5_0", pDefines);
		break;
	case eShaderType::COMPUTE:
		newShader = new ComputeShader(shaderName.c_str(), entry_point.c_str(), "cs_5_0", pDefines);
		break;
	default:
		return nullptr;
	}

	// 파일을 제대로 읽지 못하여 생성하지 못한경우 nullptr..
	if (newShader == nullptr)
		return nullptr;

	if (shaderKey.empty())
	{
		// 새로 생성한 Shader 삽입..
		m_ShaderList.insert(std::make_pair(entry_point, newShader));
	}
	else
	{
		// 새로 생성한 Shader 삽입..
		m_ShaderList.insert(std::make_pair(shaderKey, newShader));
	}

	return newShader;
}

OriginalShader ShaderManager::GetShader(std::string shaderName)
{
	return OriginalShader{ this, shaderName };
}