#pragma once

// DirectX 11 DepthStencilView Class
class DepthStencilView : public Texture2D
{
public:
	DepthStencilView(ID3D11DepthStencilView** dsv);
	~DepthStencilView();

public:
	void Reset();

public:
	ID3D11Texture2D* GetTexture2D() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc(int width, int height) override;

public:
	ID3D11DepthStencilView* Get();
	ID3D11DepthStencilView** GetAddress();
	D3D11_DEPTH_STENCIL_VIEW_DESC GetDesc();

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
};

