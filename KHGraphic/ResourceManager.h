#pragma once
#include <unordered_map>
#include "ResourceManagerBase.h"

class D3D11Graphic;
class SamplerState;

class GraphicResourceManager : public IGraphicResourceManager
{
public:
	GraphicResourceManager(D3D11Graphic* graphic, IShaderManager* shaderManager);
	~GraphicResourceManager();

public:
	void Initialize() override;
	void OnResize(int width, int height) override;
	void Release() override;

public:
	BasicRenderTarget* GetMainRenderTarget() override;
	void AddMainRenderTarget(RenderTarget* rtv) override;

public:
	OriginalRenderTarget GetRenderTarget(Hash_Code hash_code) override;
	DepthStencilView* GetDepthStencilView(Hash_Code hash_code) override;
	BlendState* GetBlendState(Hash_Code hash_code) override;
	RasterizerState* GetRasterizerState(Hash_Code hash_code) override;
	DepthStencilState* GetDepthStencilState(Hash_Code hash_code) override;
	ViewPort* GetViewPort(Hash_Code hash_code) override;
	BufferData* GetBuffer(Hash_Code hash_code) override;

public:
	void AddResource(Hash_Code hash_code, ResourceBase* resource) override;

private:
	BasicRenderTarget* GetBasicRenderTarget(Hash_Code hash_code) override;
	ComputeRenderTarget* GetComputeRenderTarget(Hash_Code hash_code) override;

private:
	IShaderManager* m_ShaderManager;

	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	/////////////////////////////////////////////////////////////////////////////////////////
	// RenderTarget Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	RenderTarget* m_BackBuffer;
	std::unordered_map<Hash_Code, RenderTarget*> m_RenderTargetList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// View Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::unordered_map<Hash_Code, DepthStencilView*> m_DepthStencilViewList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// State Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	std::unordered_map<Hash_Code, DepthStencilState*> m_DepthStencilStateList;
	std::unordered_map<Hash_Code, RasterizerState*> m_RasterizerStateList;
	std::unordered_map<Hash_Code, BlendState*> m_BlendStateList;
	std::unordered_map<Hash_Code, SamplerState*> m_SamplerStateList;

	std::unordered_map<Hash_Code, ViewPort*> m_ViewPortList;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Buffer Resource List
	/////////////////////////////////////////////////////////////////////////////////////////
	std::unordered_map<Hash_Code, BufferData*> m_BufferList;
};