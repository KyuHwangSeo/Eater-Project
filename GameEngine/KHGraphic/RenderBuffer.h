#pragma once

/// 2022/01/04 18:30
/// SeoKyuHwang
///
/// # ResourceBuffer Class
///
/// - Rendering에 필요한 GraphicView Resource Class
/// - Buffer 기반으로 Buffer Bind Resoruce 생성

class RenderBuffer : public ResourceBase
{
public:
	RenderBuffer(Buffer* buffer, ShaderResourceView* srv, UnorderedAccessView* uav);
	virtual ~RenderBuffer();

public:
	void Reset() override;
	void Release() override;

	void OnResize(UINT byteWidth);
	void SetResize();

public:
	Buffer* GetBuffer();

	ShaderResourceView* GetSRV();
	UnorderedAccessView* GetUAV();

private:
	Buffer* m_Buffer;

	ShaderResourceView* m_SRV;
	UnorderedAccessView* m_UAV;
};

