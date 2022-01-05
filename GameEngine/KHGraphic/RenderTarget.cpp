#include "DirectDefine.h"
#include "GraphicView.h"
#include "Texture2D.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(Texture2D* tex2D, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav)
	:ResourceBase(RESOURCE_TYPE::RT), m_Tex2D(tex2D), m_RTV(rtv), m_SRV(srv), m_UAV(uav)
{

}

RenderTarget::~RenderTarget()
{
	Release();
}

void RenderTarget::Reset()
{
	m_Tex2D->Reset();
	m_RTV->Reset();
	m_SRV->Reset();
	m_UAV->Reset();
}

void RenderTarget::Release()
{
	RELEASE_COM(m_Tex2D);
	RELEASE_COM(m_RTV);
	RELEASE_COM(m_SRV);
	RELEASE_COM(m_UAV);
}

void RenderTarget::OnResize(int width, int height)
{
	m_Tex2D->OnResize(width, height);
}

void RenderTarget::SetRatio(float width_ratio, float height_ratio)
{
	m_Tex2D->SetRatio(width_ratio, height_ratio);
}

Texture2D* RenderTarget::GetTex2D()
{
	return m_Tex2D;
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