#include "DirectDefine.h"
#include "GraphicView.h"
#include "Texture2D.h"
#include "RenderTarget.h"
#include <assert.h>

RenderTarget::RenderTarget(ID3D11Texture2D* tex2D, RenderTargetView* rtv, ShaderResourceView* srv, UnorderedAccessView* uav)
	:Texture2D(eResourceType::RT, tex2D), m_RTV(rtv), m_SRV(srv), m_UAV(uav)
{

}

RenderTarget::~RenderTarget()
{
	Release();
}

void RenderTarget::Reset()
{
	m_RTV->Reset();
	m_SRV->Reset();
	m_UAV->Reset();
}

void RenderTarget::Release()
{
	RELEASE_COM(m_RTV);
	RELEASE_COM(m_SRV);
	RELEASE_COM(m_UAV);
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