#include "DirectDefine.h"
#include "Texture2D.h"
#include "GraphicView.h"
#include "DepthStencil.h"
#include <assert.h>

DepthStencil::DepthStencil(ID3D11Texture2D* tex2D, DepthStencilView* dsv, ShaderResourceView* srv)
	:Texture2D(eResourceType::DS, tex2D), m_DSV(dsv), m_SRV(srv)
{

}

DepthStencil::~DepthStencil()
{
	Release();
}

void DepthStencil::Reset()
{
	m_DSV->Reset();
	m_SRV->Reset();
}

void DepthStencil::Release()
{
	RELEASE_COM(m_DSV);
	RELEASE_COM(m_SRV);
}

DepthStencilView* DepthStencil::GetDSV()
{
	return m_DSV;
}

ShaderResourceView* DepthStencil::GetSRV()
{
	return m_SRV;
}