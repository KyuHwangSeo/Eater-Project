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
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "LightVS.hlsl", "Light_VS", "Light_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "LightPS.hlsl", "Light_PS", "Light_PS_Option0" ,light0_macro);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "LightPS.hlsl", "Light_PS", "Light_PS_Option1" ,light1_macro);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "LightPS.hlsl", "Light_PS", "Light_PS_Option2" ,light2_macro);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "LightPS.hlsl", "Light_PS", "Light_PS_Option3" ,light3_macro);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "LightPS.hlsl", "Light_PS", "Light_PS_Option4" ,light4_macro);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "LightPS.hlsl", "Light_PS", "Light_PS_Option5" ,light5_macro);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "LightPS.hlsl", "Light_PS", "Light_PS_Option6" ,light6_macro);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "LightPS.hlsl", "Light_PS", "Light_PS_Option7");

	D3D_SHADER_MACRO terrain_macro[] = { {"TERRAIN_MESH"}, {NULL, NULL} };

	// Object Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "ObjectVS.hlsl", "Mesh_VS", "Mesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "ObjectVS.hlsl", "Skin_VS", "Skin_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "ObjectVS.hlsl", "Mesh_VS", "Terrain_VS", terrain_macro);

	// Deffered Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "DeferredPS.hlsl", "Deferred_PS", "Deferred_PS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "DeferredPS.hlsl", "Deferred_PS", "Terrain_PS", terrain_macro);

	// Forward Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "ForwardPS.hlsl", "Forward_PS", "Forward_PS");

	// Shadow Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "ShadowVS.hlsl", "MeshShadow_VS", "MeshShadow_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "ShadowVS.hlsl", "SkinShadow_VS", "SkinShadow_VS");

	// SSAO Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "SSAOVS.hlsl", "SSAO_VS", "SSAO_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SSAOPS.hlsl", "SSAO_PS", "SSAO_PS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "SSAOBlurVS.hlsl", "SSAOBlur_VS", "SSAOBlur_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SSAOBlurPS.hlsl", "SSAOBlur_PS", "SSAOBlur_PS");
}

void ShaderManager::AddSampler(Hash_Code hash_code, ID3D11SamplerState* sampler)
{
	for (std::pair<std::string, ShaderBase*> shader : m_ShaderList)
	{
		shader.second->SetSamplerState(hash_code, sampler);
	}
}

ShaderBase* ShaderManager::LoadShader(SHADER_TYPE shaderType, const char* fileName, const char* entry_point, const char* shaderName, const D3D_SHADER_MACRO* pDefines)
{
	// Shader Type에 맞는 Shader 생성..
	ShaderBase* newShader = nullptr;

	switch (shaderType)
	{
	case SHADER_TYPE::VERTEX_SHADER:
		newShader = new VertexShader(fileName, entry_point, "vs_5_0", pDefines);
		break;
	case SHADER_TYPE::PIXEL_SHADER:
		newShader = new PixelShader(fileName, entry_point, "ps_5_0", pDefines);
		break;
	case SHADER_TYPE::COMPUTE_SHADER:
		newShader = new ComputeShader(fileName, entry_point, "cs_5_0", pDefines);
		break;
	default:
		return nullptr;
	}

	// 파일을 제대로 읽지 못하여 생성하지 못한경우 nullptr..
	if (newShader == nullptr)
		return nullptr;

	// 새로 생성한 Shader 삽입..
	m_ShaderList.insert(std::make_pair(shaderName, newShader));

	return newShader;
}

OriginalShader ShaderManager::GetShader(std::string shaderName)
{
	return OriginalShader{ this, shaderName };
}


VertexShader* ShaderManager::GetVertexShader(std::string shaderName)
{
	std::unordered_map<std::string, ShaderBase*>::iterator shader = m_ShaderList.find(shaderName);

	// 해당 Shader가 없을 경우..
	if (shader == m_ShaderList.end()) return nullptr;

	switch (shader->second->GetType())
	{
	case SHADER_TYPE::VERTEX_SHADER:
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
	case SHADER_TYPE::PIXEL_SHADER:
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
	case SHADER_TYPE::COMPUTE_SHADER:
		return reinterpret_cast<ComputeShader*>(shader->second);
	default:
		return nullptr;
	}
}