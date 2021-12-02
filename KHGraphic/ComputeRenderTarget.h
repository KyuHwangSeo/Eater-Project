#pragma once

class ComputeRenderTarget : public RenderTarget
{
public:
	ComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav);
	~ComputeRenderTarget();

public:
	void Reset() override;

public:
	ID3D11Texture2D* GetTexture2D() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc(int width, int height) override;

public:
	D3D11_UNORDERED_ACCESS_VIEW_DESC GetUAVDesc();
	ID3D11UnorderedAccessView* GetUAV();
	ID3D11UnorderedAccessView** GetAddressUAV();

public:
	bool IsUAV();

private:
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;
};

