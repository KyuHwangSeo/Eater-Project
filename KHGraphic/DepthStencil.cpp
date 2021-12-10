#include "DirectDefine.h"
#include "Texture2D.h"
#include "DepthStencil.h"
#include <assert.h>

DepthStencil::DepthStencil(ID3D11Texture2D* tex2D, ID3D11DepthStencilView* dsv, ID3D11ShaderResourceView* srv)
	:Texture2D(eResourceType::DS, tex2D), m_DSV(dsv), m_SRV(srv)
{
	if (dsv) SetResourceBind(BIND_DSV);
	if (srv) SetResourceBind(BIND_SRV);
}

DepthStencil::~DepthStencil()
{
	Reset();
}

void DepthStencil::Reset()
{
	RESET_COM(m_DSV);
	RESET_COM(m_SRV);
}

ID3D11DepthStencilView* DepthStencil::GetDSV()
{
	return m_DSV.Get();
}

ID3D11DepthStencilView** DepthStencil::GetAddressDSV(bool is_release)
{
	if (is_release)
		return m_DSV.ReleaseAndGetAddressOf();
	else
		return m_DSV.GetAddressOf();
}

void DepthStencil::GetDSVDesc(D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc)
{
	assert(m_DSV);

	m_DSV->GetDesc(dsvDesc);
}

ID3D11ShaderResourceView* DepthStencil::GetSRV()
{
	return m_SRV.Get();
}

ID3D11ShaderResourceView** DepthStencil::GetAddressSRV(bool is_release)
{
	if (is_release)
		return m_SRV.ReleaseAndGetAddressOf();
	else
		return m_SRV.GetAddressOf();
}

void DepthStencil::GetSRVDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	assert(m_SRV);
	
	m_SRV->GetDesc(srvDesc);
}
