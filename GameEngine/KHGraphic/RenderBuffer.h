#pragma once

/// 2022/01/04 18:30
/// SeoKyuHwang
///
/// # ResourceBuffer Class
///
/// - Rendering�� �ʿ��� GraphicView Resource Class
/// - Buffer ������� Buffer Bind Resoruce ����

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

