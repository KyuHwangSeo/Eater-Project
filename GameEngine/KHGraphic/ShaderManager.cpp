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
	// Light Shader Macro
	D3D_SHADER_MACRO light1_macro[] = { {"SHADOW"}, {NULL, NULL} };
	D3D_SHADER_MACRO light2_macro[] = { {"SSAO"}, {NULL, NULL} };
	D3D_SHADER_MACRO light3_macro[] = { {"SHADOW"}, {"SSAO"}, {NULL, NULL} };

	// Light Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Light_VS.hlsl", "Light_VS", "Light_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PS.hlsl", "Light_PS", "Light_PS_Option0");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PS.hlsl", "Light_PS", "Light_PS_Option1", light1_macro);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PS.hlsl", "Light_PS", "Light_PS_Option2", light2_macro);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Light_PS.hlsl", "Light_PS", "Light_PS_Option3", light3_macro);

	// Deferred Shader Macro
	D3D_SHADER_MACRO deferred_macro1[] = { {"TERRAIN_MESH"}, {NULL, NULL} };
	D3D_SHADER_MACRO deferred_macro2[] = { {"GAMMA_CORRECTION"}, {NULL, NULL} };
	D3D_SHADER_MACRO deferred_macro3[] = { {"GAMMA_CORRECTION"}, {"TERRAIN_MESH"}, {NULL, NULL} };

	// Deferred Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Object_VS.hlsl", "Mesh_VS", "Mesh_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Object_VS.hlsl", "Skin_VS", "Skin_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Object_VS.hlsl", "Mesh_VS", "Terrain_VS", deferred_macro1);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Deferred_PS.hlsl", "Deferred_PS", "Deferred_PS_Option0");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Deferred_PS.hlsl", "Deferred_PS", "Deferred_PS_Option1", deferred_macro2);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Deferred_PS.hlsl", "Deferred_PS", "Terrain_PS_Option0", deferred_macro1);
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Deferred_PS.hlsl", "Deferred_PS", "Terrain_PS_Option1", deferred_macro3);

	// OIT Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "OIT_Blend_VS.hlsl", "OIT_VS", "OIT_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Blend_PS.hlsl", "OIT_PS", "OIT_PS");

	LoadShader(SHADER_TYPE::PIXEL_SHADER, "OIT_Particle_PS.hlsl", "OIT_Particle_PS", "OIT_Particle_PS");

	// Forward Shader
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Forward_PS.hlsl", "Forward_PS", "Forward_PS");

	// Shadow Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Shadow_VS.hlsl", "MeshShadow_VS", "MeshShadow_VS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Shadow_VS.hlsl", "SkinShadow_VS", "SkinShadow_VS");

	// SSAO Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "SSAO_VS.hlsl", "SSAO_VS", "SSAO_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SSAO_PS.hlsl", "SSAO_PS", "SSAO_PS");
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "SSAO_Blur_VS.hlsl", "SSAOBlur_VS", "SSAOBlur_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "SSAO_Blur_PS.hlsl", "SSAOBlur_PS", "SSAOBlur_PS");

	// Particle Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Particle_VS.hlsl", "Particle_VS", "Particle_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Particle_PS.hlsl", "Particle_PS", "Particle_PS");

	// Debug Shader
	LoadShader(SHADER_TYPE::VERTEX_SHADER, "Debug_VS.hlsl", "Debug_VS", "Debug_VS");
	LoadShader(SHADER_TYPE::PIXEL_SHADER, "Debug_PS.hlsl", "Debug_PS", "Debug_PS");
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