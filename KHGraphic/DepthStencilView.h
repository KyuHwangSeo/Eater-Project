#pragma once

// DirectX 11 DepthStencilView Class
class DepthStencilView : public Texture2D
{
public:
	DepthStencilView(ID3D11DepthStencilView** dsv, ID3D11ShaderResourceView** srv);
	~DepthStencilView();

public:
	void Reset();

public:
	ID3D11Texture2D* GetTexture2D() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc(int width, int height) override;

public:
	ID3D11DepthStencilView* GetDSV();
	ID3D11DepthStencilView** GetAddressDSV();
	D3D11_DEPTH_STENCIL_VIEW_DESC GetDSVDesc();

	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView** GetAddressSRV();
	D3D11_SHADER_RESOURCE_VIEW_DESC GetSRVDesc();

public:
	bool IsDSV();
	bool IsSRV();

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};

