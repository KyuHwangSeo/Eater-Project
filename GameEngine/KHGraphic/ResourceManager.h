#pragma once
#include <unordered_map>
#include "ResourceManagerBase.h"

interface ID3D11Graphic;
class SamplerState;

class GraphicResourceManager : public IGraphicResourceManager
{
public:
	GraphicResourceManager(ID3D11Graphic* graphic, IShaderManager* shaderManager);
	~GraphicResourceManager();

public:
	void Initialize() override;
	void OnResize(int width, int height) override;
	void Release() override;

public:
	RenderTarget* GetMainRenderTarget() override;
	void AddMainRenderTarget(RenderTarget* rtv) override;

public:
	RenderTarget* GetRenderTarget(Hash_Code hash_code) override;
	DepthStencil* GetDepthStencil(Hash_Code hash_code) override;
	RenderTargetView* GetRenderTargetView(Hash_Code hash_code) override;
	DepthStencilView* GetDepthStencilView(Hash_Code hash_code) override;
	ShaderResourceView* GetShaderResourceView(Hash_Code hash_code) override;
	UnorderedAccessView* GetUnorderedAccessView(Hash_Code hash_code) override;

public:
	BlendState* GetBlendState(Hash_Code hash_code) override;
	RasterizerState* GetRasterizerState(Hash_Code hash_code) override;
	DepthStencilState* GetDepthStencilState(Hash_Code hash_code) override;
	ViewPort* GetViewPort(Hash_Code hash_code) override;
	BufferData* GetBuffer(Hash_Code hash_code) override;

public:
	void AddResource(Hash_Code hash_code, ResourceBase* resource) override;

private:
	void SetShaderSampler();

private:
	ID3D11Graphic* m_Graphic;
	IShaderManager* m_ShaderManager;

	/////////////////////////////////////////////////////////////////////////////////////////
	// Graphic View Resource List
	/////////////////////////////////////////////////////////////////////////////////////////

	RenderTarget* m_BackBuffer;
	std::unordered_map<Hash_Code, RenderTarget*> m_RenderTargetList;
	std::unordered_map<Hash_Code, DepthStencil*> m_DepthStencilList;
	std::unordered_map<Hash_Code, RenderTargetView*> m_RenderTargetViewList;
	std::unordered_map<Hash_Code, DepthStencilView*> m_DepthStencilViewList;
	std::unordered_map<Hash_Code, ShaderResourceView*> m_ShaderResourceViewList;
	std::unordered_map<Hash_Code, UnorderedAccessView*> m_UnorderedAccessViewList;


	/////////////////////////////////////////////////////////////////////////////////////////
	// Graphic State Resource List
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