#include "DirectDefine.h"
#include "ShaderBase.h"
#include "PixelShader.h"

#include "ShaderResourceHashTable.h"
#include "CompilerDefine.h"
#include <sstream>
#include <fstream>

PixelShader::PixelShader(const char* fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines)
	:ShaderBase(eShaderType::PIXEL)
{
	LoadShader(g_ShaderRoute + fileName, entry_point, shader_model, pDefines);
}

PixelShader::~PixelShader()
{

}

void PixelShader::LoadShader(std::string fileName, const char* entry_point, const char* shader_model, const D3D_SHADER_MACRO* pDefines)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3D11ShaderReflection* pReflector = nullptr;
	ShaderResourceHashTable* resource_table = ShaderResourceHashTable::Get();
	
	size_t cbuffer_register_slot = 0;	// ConstantBuffer Max Register Slot
	size_t sampler_register_slot = 0;	// Sampler Max Register Slot
	size_t srv_register_slot = 0;		// ShaderResourceView Max Register Slot
	size_t hash_key = 0;				// Resource Hash Code

	std::wstring wPath(fileName.begin(), fileName.end());

	// Pixel HLSL Load..
	CreateShader(wPath.c_str(), pDefines, entry_point, shader_model, &shaderBlob);

	// Create Pixel Shader..
	HR(g_Device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &m_PS));

	// Create Reflector..
	D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);

	D3D11_SHADER_DESC shaderDesc;
	pReflector->GetDesc(&shaderDesc);

	/// ConstantBuffer Reflection
	// Pixel Shader ConstantBuffer..
	for (unsigned int cbindex = 0; cbindex < shaderDesc.ConstantBuffers; cbindex++)
	{
		ID3D11ShaderReflectionConstantBuffer* cBuffer = pReflector->GetConstantBufferByIndex(cbindex);
		D3D11_SHADER_BUFFER_DESC bufferDesc;

		if (SUCCEEDED(cBuffer->GetDesc(&bufferDesc)))
		{
			ID3D11Buffer* cBuffer = nullptr;
			CD3D11_BUFFER_DESC cBufferDesc(bufferDesc.Size, D3D11_BIND_CONSTANT_BUFFER);

			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			pReflector->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

			// 해당 Constant Buffer 생성..
			HR(g_Device->CreateBuffer(&cBufferDesc, nullptr, &cBuffer));

			// Constant Buffer Hash Code..
			hash_key = resource_table->FindHashCode(eResourceType::CB, bufferDesc.Name);

			// Constant Buffer Register Slot Number..
			cbuffer_register_slot = bindDesc.BindPoint;

			// Key (Constant Buffer HashCode) && Value (Register Slot, Constant Buffer)
			m_ConstantBufferList.insert(std::make_pair(hash_key, new ConstantBuffer(bindDesc.Name, cbuffer_register_slot, &cBuffer)));
		}
	}

	/// Shader Resource Reflection
	// Shader Resource..
	for (unsigned int rsindex = 0; rsindex < shaderDesc.BoundResources; rsindex++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		pReflector->GetResourceBindingDesc(rsindex, &bindDesc);

		// Resource Type에 맞는 해당 List에 삽입..
		switch (bindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
		{
			// SRV Hash Code..
			hash_key = resource_table->FindHashCode(eResourceType::SRV, bindDesc.Name);

			// SRV Register Slot Number..
			srv_register_slot = bindDesc.BindPoint;
			
			// SRV 추가..
			m_SRVList.insert(std::make_pair(hash_key, new ShaderResourceBuffer(bindDesc.Name, srv_register_slot)));
		}
			break;
		case D3D_SIT_SAMPLER:
		{
			// Sampler Hash Code..
			hash_key = resource_table->FindHashCode(eResourceType::SS, bindDesc.Name);

			// Sampler Register Slot Number..
			sampler_register_slot = bindDesc.BindPoint;

			// Sampler 추가..
			m_SamplerList.insert(std::make_pair(hash_key, new SamplerBuffer(bindDesc.Name, sampler_register_slot)));
		}
			break;
		default:
			break;
		}
	}

	// 마지막으로 Binding 된 Resource Register Index 기준으로 사이즈 설정..
	m_ConstantBuffers.resize(++cbuffer_register_slot);
	m_SamplerStates.resize(++sampler_register_slot);
	m_ShaderResourceViews.resize(++srv_register_slot);

	// Constant Buffer List 최초 설정..
	for (auto& cBuffer : m_ConstantBufferList)
	{
		m_ConstantBuffers[cBuffer.second->register_number] = cBuffer.second->cBuffer;
	}

	pReflector->Release();
}

void PixelShader::Update()
{
	// Pixel Shader 연결..
	g_DeviceContext->PSSetShader(m_PS.Get(), nullptr, 0);

	// Pixel Shader SamplerState 설정..
	if (!m_SamplerStates.empty())
		g_DeviceContext->PSSetSamplers(0, (UINT)m_SamplerStates.size(), m_SamplerStates[0].GetAddressOf());

	// Pixel Shader ConstantBuffer 설정..
	if (!m_ConstantBuffers.empty())
		g_DeviceContext->PSSetConstantBuffers(0, (UINT)m_ConstantBuffers.size(), m_ConstantBuffers[0].GetAddressOf());

	// Pixel Shader ShaderResourceView 설정..
	if (!m_ShaderResourceViews.empty())
		g_DeviceContext->PSSetShaderResources(0, (UINT)m_ShaderResourceViews.size(), m_ShaderResourceViews[0].GetAddressOf());
}

void PixelShader::Release()
{
	ShaderBase::Release();

	RESET_COM(m_PS);
}
