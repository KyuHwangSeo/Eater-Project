#pragma once
#include <unordered_map>
#include "ResourceManagerBase.h"

typedef size_t Hash_Code;

class GraphicResourceManager : public IGraphicResourceManager
{
public:
	GraphicResourceManager();
	~GraphicResourceManager();

public:
	friend class OriginalRenderTarget;

public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain) override;
	void OnResize(int width, int height) override;
	void Release() override;

public:
	BasicRenderTarget* GetMainRenderTarget() override;
	OriginalRenderTarget GetRenderTarget(eRenderTarget state) override;

	DepthStencilView* GetDepthStencilView(eDepthStencilView state) override;

	ID3D11BlendState* GetBlendState(eBlendState state) override;
	ID3D11RasterizerState* GetRasterizerState(eRasterizerState state) override;
	ID3D11DepthStencilState* GetDepthStencilState(eDepthStencilState state) override;

	D3D11_VIEWPORT* GetViewPort(eViewPort state) override;
	BufferData* GetBuffer(eBuffer state) override;

private:
	BasicRenderTarget* GetBasicRenderTarget(eRenderTarget state);
	ComputeRenderTarget* GetComputeRenderTarget(eRenderTarget state);

public:
	template<typename T>
	void AddResource(ResourceBase* resource);

	void AddResource(BufferData* resource);


	void AddMainRenderTarget(RenderTarget* rtv) { m_BackBuffer = rtv; }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	/////////////////////////////////////////////////////////////////////////////////////////
	// RenderTarget Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	RenderTarget* m_BackBuffer;
	std::vector<RenderTarget*> m_RenderTargetList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// View Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::vector<DepthStencilView*> m_DepthStencilViewList;
	std::vector<ViewPort*> m_ViewPortList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// State Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::unordered_map<Hash_Code, Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> m_DepthStencilStateList;
	std::unordered_map<Hash_Code, Microsoft::WRL::ComPtr<ID3D11RasterizerState>> m_RasterizerStateList;
	std::unordered_map<Hash_Code, Microsoft::WRL::ComPtr<ID3D11BlendState>> m_BlendStateList;
	std::unordered_map<Hash_Code, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_SamplerStateList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Buffer Resource List
	/////////////////////////////////////////////////////////////////////////////////////////
	std::vector<BufferData*> m_BufferList;
};

template<typename T>
inline void GraphicResourceManager::AddResource(ResourceBase* resource)
{
	switch (resource->GetType())
	{
	case eResourceType::CB:
		break;
	case eResourceType::SRV:
		break;
	case eResourceType::UAV:
		break;
	case eResourceType::DSV:
		break;
	case eResourceType::DSS:
		break;
	case eResourceType::SS:
		break;
	case eResourceType::RS:
		break;
	case eResourceType::BS:
		break;
	case eResourceType::RT:
		break;
	case eResourceType::VP:
		break;
	default:
		break;
	}
}

inline void GraphicResourceManager::AddResource(BufferData* resource) { m_BufferList.push_back(resource); }


