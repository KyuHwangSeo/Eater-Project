#pragma once
#include "D3D11GraphicBase.h"

class D3D11Graphic : public ID3D11Graphic
{
public:
	D3D11Graphic(HWND hwnd, int screenWidth, int screenHeight);
	~D3D11Graphic();

public:
	void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;
	void Release() override;

public:
	void CreateBuffer(D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subDesc, ID3D11Buffer** buffer) override;
	void CreateTextureBuffer(std::string filePath, ID3D11Resource** resource, ID3D11ShaderResourceView** srv) override;

public:
	void CreateBackBuffer(UINT width, UINT height, ID3D11Texture2D** tex2D, ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv) override;
	void CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, ID3D11Texture2D** tex2D) override;
	void CreateDepthStencilView(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, ID3D11DepthStencilView** dsv) override;
	void CreateRenderTargetView(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv) override;
	void CreateShaderResourceView(ID3D11Texture2D* tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv) override;
	void CreateUnorderedAccessView(ID3D11Texture2D* tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav) override;

public:
	void CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc, ID3D11DepthStencilState** dss) override;
	void CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc, ID3D11RasterizerState** rs) override;
	void CreateBlendState(D3D11_BLEND_DESC* bsDesc, ID3D11BlendState** bs) override;
	void CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc, ID3D11SamplerState** ss) override;

public:
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() override;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() override;
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
};