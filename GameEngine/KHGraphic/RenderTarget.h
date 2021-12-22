#pragma once

// DirectX 11 RenderTarget Class
class RenderTarget : public Texture2D
{
public:
	RenderTarget(ID3D11Texture2D* tex2D, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav);
	virtual ~RenderTarget();

public:
	void Reset() override;
	void Release() override;

public:
	RenderTargetView* GetRTV();
	ShaderResourceView* GetSRV();
	UnorderedAccessView* GetUAV();

private:
	RenderTargetView* m_RTV;
	ShaderResourceView* m_SRV;
	UnorderedAccessView* m_UAV;
};

