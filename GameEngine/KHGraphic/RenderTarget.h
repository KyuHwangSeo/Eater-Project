#pragma once

/// 2022/01/04 18:30
/// SeoKyuHwang
///
/// # RenderTarget Class
///
/// - Rendering�� �ʿ��� GraphicView Resource Class
/// - Texture2D ������� RenderTarget Bind Resoruce ����

class RenderTarget : public ResourceBase
{
public:
	RenderTarget(Texture2D* tex2D, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav);
	virtual ~RenderTarget();

public:
	void Reset() override;
	void Release() override;

	void OnResize(int width, int height);
	void SetRatio(float width_ratio, float height_ratio);

public:
	Texture2D* GetTex2D();

	RenderTargetView* GetRTV();
	ShaderResourceView* GetSRV();
	UnorderedAccessView* GetUAV();

private:
	Texture2D* m_Tex2D;

	RenderTargetView* m_RTV;
	ShaderResourceView* m_SRV;
	UnorderedAccessView* m_UAV;
};

