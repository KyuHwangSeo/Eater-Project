#include <vector>
#include "DirectDefine.h"
#include "EnumDefine.h"
#include "D3D11Graphic.h"
#include "BufferData.h"
#include "GraphicState.h"
#include "BufferData.h"
#include "ViewPort.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "ComputeRenderTarget.h"
#include "ShaderManagerBase.h"
#include "ResourceManager.h"

#include "VertexDefine.h"

GraphicResourceManager::GraphicResourceManager(D3D11Graphic* graphic, IShaderManager* shaderManager)
	:m_ShaderManager(shaderManager), m_BackBuffer(nullptr)
{
	m_Device = graphic->GetDevice();
	m_SwapChain = graphic->GetSwapChain();
}

GraphicResourceManager::~GraphicResourceManager()
{
}

void GraphicResourceManager::Initialize()
{
	for (std::pair<Hash_Code, SamplerState*> sampler : m_SamplerStateList)
	{
		m_ShaderManager->AddSampler(sampler.first, sampler.second->GetAddress());
	}

	// Shader Hash Table Reset..
	ShaderResourceHashTable::Get()->Destroy();
}

void GraphicResourceManager::OnResize(int width, int height)
{
	ComPtr<ID3D11Texture2D> tex2D = nullptr;

	bool isRTV, isSRV, isUAV = false;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	// BackBuffer Reset..
	m_BackBuffer->Reset();

	// Swap Chain, Render Target View Resize
	HR(m_SwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	// Get Swap Chain Back Buffer Pointer..
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(tex2D.GetAddressOf())));

	// BackBuffer Resize..
	BasicRenderTarget* bRenderTarget = reinterpret_cast<BasicRenderTarget*>(m_BackBuffer);
	HR(m_Device->CreateRenderTargetView(tex2D.Get(), nullptr, bRenderTarget->GetAddressRTV()));
	HR(m_Device->CreateShaderResourceView(tex2D.Get(), nullptr, bRenderTarget->GetAddressSRV()));

	// Texture2D Reset..
	RESET_COM(tex2D);

	// RenderTarget Resize..
	for (std::pair<Hash_Code, RenderTarget*> rt : m_RenderTargetList)
	{
		RenderTarget* renderTarget = rt.second;
		
		// Texture2D Description 추출..
		texDesc = renderTarget->GetTextureDesc(width, height);

		// Texture2D Resize..
		HR(m_Device->CreateTexture2D(&texDesc, 0, tex2D.GetAddressOf()));

		switch (renderTarget->GetType())
		{
		case eRenderTargetType::BASIC:
		{
			BasicRenderTarget* bRenderTarget = reinterpret_cast<BasicRenderTarget*>(renderTarget);
			
			// Resource 여부 파악..
			isRTV = bRenderTarget->IsRTV();
			isSRV = bRenderTarget->IsSRV();

			// RenderTargetView Description 추출..
			if (isRTV)
				rtvDesc = bRenderTarget->GetRTVDesc();

			// ShaderResourceView Description 추출..
			if (isSRV)
				srvDesc = bRenderTarget->GetSRVDesc();

			// Resource Reset..
			bRenderTarget->Reset();

			// RenderTargetView Resize..
			if (isRTV)
				HR(m_Device->CreateRenderTargetView(tex2D.Get(), &rtvDesc, bRenderTarget->GetAddressRTV()));

			// ShaderResourceView Resize..
			if (isSRV)
				HR(m_Device->CreateShaderResourceView(tex2D.Get(), &srvDesc, bRenderTarget->GetAddressSRV()));
		}
			break;
		case eRenderTargetType::COMPUTE:
		{
			ComputeRenderTarget* cRenderTarget = reinterpret_cast<ComputeRenderTarget*>(renderTarget);

			// Resource 여부 파악..
			isRTV = cRenderTarget->IsRTV();
			isUAV = cRenderTarget->IsUAV();

			// RenderTargetView Description 추출..
			if (isRTV)
				rtvDesc = cRenderTarget->GetRTVDesc();

			// UnorderedAccessView Description 추출..
			if (isUAV)
				uavDesc = cRenderTarget->GetUAVDesc();

			// Resource Reset..
			cRenderTarget->Reset();

			// RenderTargetView Resize..
			if (isRTV)
				HR(m_Device->CreateRenderTargetView(tex2D.Get(), &rtvDesc, cRenderTarget->GetAddressRTV()));

			// UnorderedAccessView Resize..
			if (isUAV)
				HR(m_Device->CreateUnorderedAccessView(tex2D.Get(), &uavDesc, cRenderTarget->GetAddressUAV()));
		}
			break;
		default:
			break;
		}

		// Texture2D Reset..
		RESET_COM(tex2D);
	}

	// DepthStecilView Resize..
	for (std::pair<Hash_Code, DepthStencilView*> dsv : m_DepthStencilViewList)
	{
		DepthStencilView* depthStencilView = dsv.second;

		// Texture2D Description 추출..
		texDesc = depthStencilView->GetTextureDesc(width, height);

		HR(m_Device->CreateTexture2D(&texDesc, 0, tex2D.GetAddressOf()));

		// DepthStencilView Description 추출..
		dsvDesc = depthStencilView->GetDesc();
		
		// Resource Reset..
		depthStencilView->Reset();

		// DepthStencilView Resize..
		HR(m_Device->CreateDepthStencilView(tex2D.Get(), &dsvDesc, depthStencilView->GetAddress()));

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
	RESET_COM(m_Device);
	RESET_COM(m_SwapChain);

	SAFE_DELETE(m_BackBuffer);

	for (std::pair<Hash_Code, RenderTarget*> rt : m_RenderTargetList)
	{
		SAFE_DELETE(rt.second);
	}

	for (std::pair<Hash_Code, DepthStencilView*> dsv : m_DepthStencilViewList)
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
	m_DepthStencilViewList.clear();
	m_ViewPortList.clear();
	m_DepthStencilStateList.clear();
	m_RasterizerStateList.clear();
	m_BlendStateList.clear();
	m_SamplerStateList.clear();
	m_BufferList.clear();
}

BasicRenderTarget* GraphicResourceManager::GetMainRenderTarget()
{
	return reinterpret_cast<BasicRenderTarget*>(m_BackBuffer);
}

void GraphicResourceManager::AddMainRenderTarget(RenderTarget* rtv)
{
	m_BackBuffer = rtv;
}

BasicRenderTarget* GraphicResourceManager::GetBasicRenderTarget(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, RenderTarget*>::iterator itor = m_RenderTargetList.find(hash_code);

	if (itor == m_RenderTargetList.end()) return nullptr;

	RenderTarget* renderTarget = itor->second;

	switch (renderTarget->GetType())
	{
	case eRenderTargetType::BASIC:
		return reinterpret_cast<BasicRenderTarget*>(renderTarget);
	default:
		return nullptr;
	}
}

ComputeRenderTarget* GraphicResourceManager::GetComputeRenderTarget(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, RenderTarget*>::iterator itor = m_RenderTargetList.find(hash_code);

	if (itor == m_RenderTargetList.end()) return nullptr;

	RenderTarget* renderTarget = itor->second;

	switch (renderTarget->GetType())
	{
	case eRenderTargetType::COMPUTE:
		return reinterpret_cast<ComputeRenderTarget*>(renderTarget);
	default:
		return nullptr;
	}
}

OriginalRenderTarget GraphicResourceManager::GetRenderTarget(Hash_Code hash_code)
{
	return OriginalRenderTarget{ this, hash_code };
}

DepthStencilView* GraphicResourceManager::GetDepthStencilView(Hash_Code hash_code)
{
	std::unordered_map<Hash_Code, DepthStencilView*>::iterator itor = m_DepthStencilViewList.find(hash_code);

	if (itor == m_DepthStencilViewList.end()) return nullptr;

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
	case eResourceType::DSV:
		m_DepthStencilViewList.insert(std::make_pair(hash_code, (DepthStencilView*)resource));
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
