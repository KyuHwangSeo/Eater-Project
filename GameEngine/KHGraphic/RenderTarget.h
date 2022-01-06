#pragma once

/// 2022/01/06 18:03
/// SeoKyuHwang
///
/// # RenderTarget Class
///
/// - Rendering에 필요한 GraphicView Resource Class

class RenderTarget : public ResourceBase
{
public:
	RenderTarget(RESOURCE_TYPE type, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav);
	virtual ~RenderTarget();

public:
	void Reset() override;
	void Release() override;

public:
	void SetResize();
	bool GetResize();

public:
	RenderTargetView* GetRTV();
	ShaderResourceView* GetSRV();
	UnorderedAccessView* GetUAV();

protected:
	bool m_IsResize;

	RenderTargetView* m_RTV;
	ShaderResourceView* m_SRV;
	UnorderedAccessView* m_UAV;
};

/// 2022/01/04 18:30
/// SeoKyuHwang
///
/// # RenderTarget Class
///
/// - Rendering에 필요한 GraphicView Resource Class
/// - Texture2D 기반으로 RenderTarget Bind Resoruce 생성

class RenderTexture : public RenderTarget
{
public:
	RenderTexture(Texture2D* tex2D, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav);
	virtual ~RenderTexture();

public:
	void Reset() override;
	void Release() override;

	void OnResize(int width, int height);
	void SetRatio(float width_ratio, float height_ratio);

public:
	Texture2D* GetTex2D();

private:
	Texture2D* m_Tex2D;
};

/// 2022/01/04 18:30
/// SeoKyuHwang
///
/// # ResourceBuffer Class
///
/// - Rendering에 필요한 GraphicView Resource Class
/// - Buffer 기반으로 Buffer Bind Resoruce 생성

class RenderBuffer : public RenderTarget
{
public:
	RenderBuffer(Buffer* buffer, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav);
	virtual ~RenderBuffer();

public:
	void Reset() override;
	void Release() override;

	void OnResize(UINT byteWidth, UINT numElements);

public:
	Buffer* GetBuffer();

private:
	Buffer* m_Buffer;
};

