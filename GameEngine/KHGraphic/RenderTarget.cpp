#include "DirectDefine.h"
#include "GraphicView.h"
#include "Buffer.h"
#include "Texture2D.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(RESOURCE_TYPE type, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav)
	:ResourceBase(type), m_RTV(rtv), m_SRV(srv), m_UAV(uav)
{

}

RenderTarget::~RenderTarget()
{

}

void RenderTarget::Reset()
{
	SAFE_RESET(m_RTV);
	SAFE_RESET(m_SRV);
	SAFE_RESET(m_UAV);
}

void RenderTarget::Release()
{
	SAFE_RELEASE(m_RTV);
	SAFE_RELEASE(m_SRV);
	SAFE_RELEASE(m_UAV);
}

void RenderTarget::SetResize()
{
	m_IsResize = true;
}

bool RenderTarget::GetResize()
{
	return m_IsResize;
}

RenderTargetView* RenderTarget::GetRTV()
{
	return m_RTV;
}

ShaderResourceView* RenderTarget::GetSRV()
{
	return m_SRV;
}

UnorderedAccessView* RenderTarget::GetUAV()
{
	return m_UAV;
}

RenderTexture::RenderTexture(Texture2D* tex2D, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav)
	:RenderTarget(RESOURCE_TYPE::RT, rtv, srv, uav), m_Tex2D(tex2D)
{

}

RenderTexture::~RenderTexture()
{
	Release();
}

void RenderTexture::Reset()
{
	SAFE_RESET(m_Tex2D);

	RenderTarget::Reset();
}

void RenderTexture::Release()
{
	SAFE_RELEASE(m_Tex2D);

	RenderTarget::Release();
}

void RenderTexture::OnResize(int width, int height)
{
	// Texture Resize..
	m_Tex2D->OnResize(width, height);
}

void RenderTexture::SetRatio(float width_ratio, float height_ratio)
{
	m_Tex2D->SetRatio(width_ratio, height_ratio);
}

Texture2D* RenderTexture::GetTex2D()
{
	return m_Tex2D;
}

RenderBuffer::RenderBuffer(Buffer* buffer, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav)
	:RenderTarget(RESOURCE_TYPE::RB, rtv, srv, uav), m_Buffer(buffer)
{

}

RenderBuffer::~RenderBuffer()
{
	Release();
}

void RenderBuffer::Reset()
{
	SAFE_RESET(m_Buffer);

	RenderTarget::Reset();
}

void RenderBuffer::Release()
{
	SAFE_RELEASE(m_Buffer);

	RenderTarget::Release();
}

void RenderBuffer::OnResize(UINT byteWidth, UINT numElements)
{
	// Buffer & Bind Resource Resize..
	m_Buffer->OnResize(byteWidth);

	if (m_RTV) m_RTV->OnResize(numElements);
	if (m_SRV) m_SRV->OnResize(numElements);
	if (m_UAV) m_UAV->OnResize(numElements);
}

Buffer* RenderBuffer::GetBuffer()
{
	return m_Buffer;
}