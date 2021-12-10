#pragma once

// DirectX 11 RenderTargetView Class
class RenderTarget : public Texture2D
{
public:
	RenderTarget(ID3D11Texture2D* tex2D, ID3D11RenderTargetView* rtv, ID3D11ShaderResourceView* srv, ID3D11UnorderedAccessView* uav);
	virtual ~RenderTarget();

public:
	void Reset() override;

public:
	ID3D11RenderTargetView* GetRTV();
	ID3D11RenderTargetView** GetAddressRTV(bool is_release = false);
	void GetRTVDesc(D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc);

public:
	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView** GetAddressSRV(bool is_release = false);
	void GetSRVDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);

public:
	ID3D11UnorderedAccessView* GetUAV();
	ID3D11UnorderedAccessView** GetAddressUAV(bool is_release = false);
	void GetUAVDesc(D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;
};

