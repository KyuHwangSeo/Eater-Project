#pragma once
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
	template<typename T, typename U>
	void AddResource(U resource);

	template<typename T>
	void AddResource(ViewPort* resource);

	template<typename T>
	void AddResource(RenderTarget* resource);

	template<typename T>
	void AddResource(DepthStencilView* resource);

	template<typename T>
	void AddResource(Microsoft::WRL::ComPtr<ID3D11DepthStencilState> resource);

	template<typename T>
	void AddResource(Microsoft::WRL::ComPtr<ID3D11RasterizerState> resource);

	template<typename T>
	void AddResource(Microsoft::WRL::ComPtr<ID3D11BlendState> resource);

	template<typename T>
	void AddResource(Microsoft::WRL::ComPtr<ID3D11SamplerState> resource);

	template<typename T = int>
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


template<typename T, typename U>
inline void GraphicResourceManager::AddResource(U resource) {}

template<typename T>
inline void GraphicResourceManager::AddResource(ViewPort* resource) { m_ViewPortList.insert(std::make_pair(T::GetHashCode(), resource)); }

template<typename T>
inline void GraphicResourceManager::AddResource(RenderTarget* resource) { m_RenderTargetList.insert(std::make_pair(T::GetHashCode(), resource)); }

template<typename T>
inline void GraphicResourceManager::AddResource(DepthStencilView* resource) { m_DepthStencilViewList.insert(std::make_pair(T::GetHashCode(), resource)); }

template<typename T>
inline void GraphicResourceManager::AddResource(Microsoft::WRL::ComPtr<ID3D11DepthStencilState> resource) { m_DepthStencilStateList.insert(std::make_pair(T::GetHashCode(), resource)); }

template<typename T>
inline void GraphicResourceManager::AddResource(Microsoft::WRL::ComPtr<ID3D11RasterizerState> resource) { m_RasterizerStateList.insert(std::make_pair(T::GetHashCode(), resource)); }

template<typename T>
inline void GraphicResourceManager::AddResource(Microsoft::WRL::ComPtr<ID3D11BlendState> resource) { m_BlendStateList.insert(std::make_pair(T::GetHashCode(), resource)); }

template<typename T>
inline void GraphicResourceManager::AddResource(Microsoft::WRL::ComPtr<ID3D11SamplerState> resource) { m_SamplerStateList.insert(std::make_pair(T::GetHashCode(), resource)); }

template<typename T>
inline void GraphicResourceManager::AddResource(BufferData* resource) { m_BufferList.insert(std::make_pair(T::GetHashCode(), resource)); }