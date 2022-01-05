#include "DirectDefine.h"
#include "Texture2D.h"
#include "GraphicView.h"
#include "DepthStencil.h"

DepthStencil::DepthStencil(Texture2D* tex2D, DepthStencilView* dsv, ShaderResourceView* srv)
	:ResourceBase(RESOURCE_TYPE::DS), m_Tex2D(tex2D), m_DSV(dsv), m_SRV(srv)
{

}

DepthStencil::~DepthStencil()
{
	Release();
}

void DepthStencil::Reset()
{
	m_Tex2D->Reset();
	m_DSV->Reset();
	m_SRV->Reset();
}

void DepthStencil::Release()
{
	RELEASE_COM(m_Tex2D);
	RELEASE_COM(m_DSV);
	RELEASE_COM(m_SRV);
}

void DepthStencil::OnResize(int width, int height)
{
	m_Tex2D->OnResize(width, height);
}

void DepthStencil::SetRatio(float width_ratio, float height_ratio)
{
	m_Tex2D->SetRatio(width_ratio, height_ratio);
}

Texture2D* DepthStencil::GetTex2D()
{
	return m_Tex2D;
}

DepthStencilView* DepthStencil::GetDSV()
{
	return m_DSV;
}

ShaderResourceView* DepthStencil::GetSRV()
{
	return m_SRV;
}