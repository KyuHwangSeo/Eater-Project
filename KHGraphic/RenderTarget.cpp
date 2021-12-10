#include "DirectDefine.h"
#include "Texture2D.h"
#include "RenderTarget.h"
#include <assert.h>

RenderTarget::RenderTarget(ID3D11Texture2D* tex2D, ID3D11RenderTargetView* rtv, ID3D11ShaderResourceView* srv, ID3D11UnorderedAccessView* uav)
	:Texture2D(eResourceType::RT, tex2D), m_RTV(rtv), m_SRV(srv), m_UAV(uav)
{
	if (rtv) SetResourceBind(BIND_RTV);
	if (srv) SetResourceBind(BIND_SRV);
	if (uav) SetResourceBind(BIND_UAV);
}

RenderTarget::~RenderTarget()
{
	Reset();
}

void RenderTarget::Reset()
{
	RESET_COM(m_RTV);
	RESET_COM(m_SRV);
	RESET_COM(m_UAV);
}

ID3D11RenderTargetView* RenderTarget::GetRTV()
{
	return m_RTV.Get();
}

ID3D11RenderTargetView** RenderTarget::GetAddressRTV(bool is_release)
{
	if (is_release)
		return m_RTV.ReleaseAndGetAddressOf();
	else
		return m_RTV.GetAddressOf();
}

void RenderTarget::GetRTVDesc(D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc)
{
	assert(m_RTV);
	
	m_RTV->GetDesc(rtvDesc);
}

ID3D11ShaderResourceView* RenderTarget::GetSRV()
{
	return m_SRV.Get();
}

ID3D11ShaderResourceView** RenderTarget::GetAddressSRV(bool is_release)
{
	if (is_release)
		return m_SRV.ReleaseAndGetAddressOf();
	else
		return m_SRV.GetAddressOf();
}

void RenderTarget::GetSRVDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	assert(m_SRV);
	
	m_SRV->GetDesc(srvDesc);
}

ID3D11UnorderedAccessView* RenderTarget::GetUAV()
{
	return m_UAV.Get();
}

ID3D11UnorderedAccessView** RenderTarget::GetAddressUAV(bool is_release)
{
	if (is_release)
		return m_UAV.ReleaseAndGetAddressOf();
	else
		return m_UAV.GetAddressOf();
}

void RenderTarget::GetUAVDesc(D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	assert(m_UAV);
	
	m_UAV->GetDesc(uavDesc);
}
