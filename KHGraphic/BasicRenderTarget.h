#pragma once
class BasicRenderTarget : public RenderTarget
{
public:
	BasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv);
	~BasicRenderTarget();

public:
	void Reset() override;

public:
	ID3D11Texture2D* GetTexture2D() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc() override;
	D3D11_TEXTURE2D_DESC GetTextureDesc(int width, int height) override;

public:
	D3D11_SHADER_RESOURCE_VIEW_DESC GetSRVDesc();
	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView** GetAddressSRV();

public:
	bool IsSRV();

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};

