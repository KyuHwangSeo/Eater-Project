#pragma once

// DirectX 11 DepthStencil Class
class DepthStencil : public Texture2D
{
public:
	DepthStencil(ID3D11Texture2D* tex2D, DepthStencilView* dsv, ShaderResourceView* srv);
	~DepthStencil();

public:
	void Reset() override;
	void Release() override;

public:
	DepthStencilView* GetDSV();
	ShaderResourceView* GetSRV();

private:
	DepthStencilView* m_DSV;
	ShaderResourceView* m_SRV;
};