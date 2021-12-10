#include <vector>
#include "DirectDefine.h"
#include "EnumDefine.h"
#include "D3D11GraphicBase.h"
#include "BufferData.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "BufferData.h"
#include "Texture2D.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "ShaderManagerBase.h"
#include "ResourceManager.h"

#include "VertexDefine.h"

GraphicResourceManager::GraphicResourceManager(ID3D11Graphic* graphic, IShaderManager* shaderManager)
	:m_Graphic(graphic), m_ShaderManager(shaderManager), m_BackBuffer(nullptr)
{
}

GraphicResourceManager::~GraphicResourceManager()
{
}

void GraphicResourceManager::Initialize()
{
	// Set Binded Sampler..
	SetShaderSampler();

	// Shader Hash Table Reset..
	ShaderResourceHashTable::Get()->Destroy();
}

void GraphicResourceManager::OnResize(int width, int height)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;

	// BackBuffer Resize..
	m_Graphic->CreateBackBuffer((UINT)width, (UINT)height, tex2D.GetAddressOf(), m_BackBuffer->GetAddressRTV(true), m_BackBuffer->GetAddressSRV(true));

	// Texture2D Reset..
	RESET_COM(tex2D);

	// RenderTarget Resize..
	for (std::pair<Hash_Code, RenderTarget*> rt : m_RenderTargetList)
	{
		RenderTarget* renderTarget = rt.second;

		// RenderTarget Resize..
		renderTarget->OnResize(width, height);

		// Texture2D Resize..
		m_Graphic->CreateTexture2D(renderTarget->GetTextureDesc(), tex2D.GetAddressOf());

		// Resource Bind Type..
		Bind_Mask bindType = renderTarget->GetBindType();

		if (bindType & BIND_RTV)
		{
			// RenderTargetView Description 추출..
			renderTarget->GetRTVDesc(&rtvDesc);

			// RenderTargetView Resize..
			m_Graphic->CreateRenderTargetView(tex2D.Get(), &rtvDesc, renderTarget->GetAddressRTV(true));

		}
		if (bindType & BIND_SRV)
		{
			// ShaderResourceView Description 추출..
			renderTarget->GetSRVDesc(&srvDesc);

			// ShaderResourceView Resize..
			m_Graphic->CreateShaderResourceView(tex2D.Get(), &srvDesc, renderTarget->GetAddressSRV(true));
		}
		if (bindType & BIND_UAV)
		{
			// UnorderedAccessView Description 추출..
			renderTarget->GetUAVDesc(&uavDesc);

			// UnorderedAccessView Resize..
			m_Graphic->CreateUnorderedAccessView(tex2D.Get(), &uavDesc, renderTarget->GetAddressUAV(true));
		}

		// Texture2D Reset..
		RESET_COM(tex2D);
	}

	// DepthStecilView Resize..
	for (std::pair<Hash_Code, DepthStencil*> dsv : m_DepthStencilList)
	{
		DepthStencil* depthStencilView = dsv.second;

		// RenderTarget Resize..
		depthStencilView->OnResize(width, height);

		// Texture2D Resize..
		m_Graphic->CreateTexture2D(depthStencilView->GetTextureDesc(), tex2D.GetAddressOf());

		// Resource Bind Type..
		Bind_Mask bindType = depthStencilView->GetBindType();

		if (bindType & BIND_DSV)
		{
			// DepthStencilView Description 추출..
			depthStencilView->GetDSVDesc(&dsvDesc);

			// DepthStencilView Resize..
			m_Graphic->CreateDepthStencilView(tex2D.Get(), &dsvDesc, depthStencilView->GetAddressDSV(true));
		}
		if (bindType & BIND_SRV)
		{
			// ShaderResourceView Description 추출..
			depthStencilView->GetSRVDesc(&srvDesc);

			// ShaderResourceView Resize..
			m_Graphic->CreateShaderResourceView(tex2D.Get(), &srvDesc, depthStencilView->GetAddressSRV(true));
		}

		// Texture2D Reset..
		RESET_COM(tex2D);
	}

	// ViewPort Resize..
	for (std::pair<Hash_Code, ViewPort*> viewport : m_ViewPortList)
	{
		viewport.second->OnResize(width, height);
	}
}

void GraphicResourceManager::Release()
{
	RELEASE_COM(m_Graphic);

	SAFE_DELETE(m_BackBuffer);

	for (std::pair<Hash_Code, RenderTarget*> rt : m_RenderTargetList)
	{
		SAFE_DELETE(rt.second);
	}

	for (std::pair<Hash_Code, DepthStencil*> dsv : m_DepthStencilList)
	{
		SAFE_DELETE(dsv.second);
	}

	for (std::pair<Hash_Code, ViewPort*> viewport : m_ViewPortList)
	{
		SAFE_DELETE(viewport.second);
	}

	for (std::pair<Hash_Code, DepthStencilState*> dss : m_DepthStencilStateList)
	{
		SAFE_DELETE(dss.second);
	}

	for (std::pair<Hash_Code, RasterizerState*> rs : m_RasterizerStateList)
	{
		SAFE_DELETE(rs.second);
	}

	for (std::pair<Hash_Code, BlendState*> bs : m_BlendStateList)
	{
		SAFE_DELETE(bs.second);
	}

	for (std::pair<Hash_Code, SamplerState*> ss : m_SamplerStateList)
	{
		SAFE_DELETE(ss.second);
	}

	for (std::pair<Hash_Code, BufferData*> buffer : m_BufferList)
	{
		SAFE_DELETE(buffer.second);
	}

	m_RenderTargetList.clear();
	m_DepthStencilList.clear();
	m_ViewPortList.clear();
	m_DepthStencilStateList.clear();
	m_RasterizerStateList.clear();
	m_BlendStateList.clear();
	m_SamplerStateList.clear();
	m_BufferList.clear();
}

RenderTarget* GraphicResourceManager::GetMainRenderTarget()
{
	return m_BackBuffer;
}

void GraphicResourceManager::AddMainRenderTarget(RenderTarget* rtv)
{
	m_BackBuffer = rtv;
}

void GraphicResourceManager::SetShaderSampler()
{
	for (std::pair<Hash_Code, SamplerState*> sampler : m_SamplerStateList)
	{
		m_ShaderManager->AddSampler(sampler.first, sampler.second->Get());
	}
}

RenderTarget* GraphicResourceManager::GetRenderTarget(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, RenderTarget*>::iterator itor = m_RenderTargetList.find(hash_code);

	if (itor == m_RenderTargetList.end()) return nullptr;

	return itor->second;
}

DepthStencil* GraphicResourceManager::GetDepthStencil(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, DepthStencil*>::iterator itor = m_DepthStencilList.find(hash_code);

	if (itor == m_DepthStencilList.end()) return nullptr;

	return itor->second;
}

ShaderResourceView* GraphicResourceManager::GetShaderResourceView(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, ShaderResourceView*>::iterator itor = m_ShaderResourceViewList.find(hash_code);

	if (itor == m_ShaderResourceViewList.end()) return nullptr;

	return itor->second;
}

UnorderedAccessView* GraphicResourceManager::GetUnorderedAccessView(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, UnorderedAccessView*>::iterator itor = m_UnorderedAccessViewList.find(hash_code);

	if (itor == m_UnorderedAccessViewList.end()) return nullptr;

	return itor->second;
}

BlendState* GraphicResourceManager::GetBlendState(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, BlendState*>::iterator itor = m_BlendStateList.find(hash_code);

	if (itor == m_BlendStateList.end()) return nullptr;

	return itor->second;
}

RasterizerState* GraphicResourceManager::GetRasterizerState(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, RasterizerState*>::iterator itor = m_RasterizerStateList.find(hash_code);

	if (itor == m_RasterizerStateList.end()) return nullptr;

	return itor->second;
}

DepthStencilState* GraphicResourceManager::GetDepthStencilState(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, DepthStencilState*>::iterator itor = m_DepthStencilStateList.find(hash_code);

	if (itor == m_DepthStencilStateList.end()) return nullptr;

	return itor->second;
}

ViewPort* GraphicResourceManager::GetViewPort(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, ViewPort*>::iterator itor = m_ViewPortList.find(hash_code);

	if (itor == m_ViewPortList.end()) return nullptr;

	return itor->second;
}

BufferData* GraphicResourceManager::GetBuffer(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, BufferData*>::iterator itor = m_BufferList.find(hash_code);

	if (itor == m_BufferList.end()) return nullptr;

	return itor->second;
}

void GraphicResourceManager::AddResource(Hash_Code hash_code, ResourceBase* resource)
{
	switch (resource->GetType())
	{
	case eResourceType::SRV:
		m_ShaderResourceViewList.insert(std::make_pair(hash_code, (ShaderResourceView*)resource));
		break;
	case eResourceType::UAV:
		m_UnorderedAccessViewList.insert(std::make_pair(hash_code, (UnorderedAccessView*)resource));
		break;
	case eResourceType::DSS:
		m_DepthStencilStateList.insert(std::make_pair(hash_code, (DepthStencilState*)resource));
		break;
	case eResourceType::SS:
		m_SamplerStateList.insert(std::make_pair(hash_code, (SamplerState*)resource));
		break;
	case eResourceType::RS:
		m_RasterizerStateList.insert(std::make_pair(hash_code, (RasterizerState*)resource));
		break;
	case eResourceType::BS:
		m_BlendStateList.insert(std::make_pair(hash_code, (BlendState*)resource));
		break;
	case eResourceType::DS:
		m_DepthStencilList.insert(std::make_pair(hash_code, (DepthStencil*)resource));
		break;
	case eResourceType::RT:
		m_RenderTargetList.insert(std::make_pair(hash_code, (RenderTarget*)resource));
		break;
	case eResourceType::VP:
		m_ViewPortList.insert(std::make_pair(hash_code, (ViewPort*)resource));
		break;
	case eResourceType::BD:
		m_BufferList.insert(std::make_pair(hash_code, (BufferData*)resource));
		break;
	default:
		break;
	}
}
