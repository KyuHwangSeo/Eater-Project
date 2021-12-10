#pragma once

// DirectX 11 DepthStencil Class
class DepthStencil : public Texture2D
{
public:
	DepthStencil(ID3D11Texture2D* tex2D, ID3D11DepthStencilView* dsv, ID3D11ShaderResourceView* srv);
	~DepthStencil();

public:
	void Reset() override;

public:
	ID3D11DepthStencilView* GetDSV();
	ID3D11DepthStencilView** GetAddressDSV(bool is_release = false);
	void GetDSVDesc(D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc);

	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView** GetAddressSRV(bool is_release = false);
	void GetSRVDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
};