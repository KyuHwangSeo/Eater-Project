#include "DirectDefine.h"
#include "GraphicView.h"

DepthStencilView::DepthStencilView(ID3D11DepthStencilView* dsv)
	:ResourceBase(RESOURCE_TYPE::DSV), m_DSV(dsv)
{

}

DepthStencilView::~DepthStencilView()
{
	Release();
}

void DepthStencilView::Reset()
{
	RESET_COM(m_DSV);
}

void DepthStencilView::Release()
{
	RELEASE_COM(m_DSV);
}

ID3D11DepthStencilView* DepthStencilView::Get()
{
	return m_DSV.Get();
}

ID3D11DepthStencilView** DepthStencilView::GetAddress()
{
	return m_DSV.GetAddressOf();
}

ID3D11DepthStencilView** DepthStencilView::ReleaseGetAddress()
{
	return m_DSV.ReleaseAndGetAddressOf();
}

void DepthStencilView::GetDesc(D3D11_DEPTH_STENCIL_VIEW_DESC* desc)
{
	assert(m_DSV);

	m_DSV->GetDesc(desc);
}

RenderTargetView::RenderTargetView(ID3D11RenderTargetView* rtv)
	:ResourceBase(RESOURCE_TYPE::RTV), m_RTV(rtv)
{

}

RenderTargetView::~RenderTargetView()
{
	Release();
}

void RenderTargetView::Reset()
{
	RESET_COM(m_RTV);
}

void RenderTargetView::Release()
{
	RELEASE_COM(m_RTV);
}

ID3D11RenderTargetView* RenderTargetView::Get()
{
	return m_RTV.Get();
}

ID3D11RenderTargetView** RenderTargetView::GetAddress()
{
	return m_RTV.GetAddressOf();
}

ID3D11RenderTargetView** RenderTargetView::ReleaseGetAddress()
{
	return m_RTV.ReleaseAndGetAddressOf();
}

void RenderTargetView::GetDesc(D3D11_RENDER_TARGET_VIEW_DESC* desc)
{
	assert(m_RTV);

	m_RTV->GetDesc(desc);
}

ShaderResourceView::ShaderResourceView(ID3D11ShaderResourceView* srv)
	:ResourceBase(RESOURCE_TYPE::SRV), m_SRV(srv)
{

}

ShaderResourceView::~ShaderResourceView()
{
	Release();
}

void ShaderResourceView::Reset()
{
	RESET_COM(m_SRV);
}

void ShaderResourceView::Release()
{
	RELEASE_COM(m_SRV);
}

ID3D11ShaderResourceView* ShaderResourceView::Get()
{
	return m_SRV.Get();
}

ID3D11ShaderResourceView** ShaderResourceView::GetAddress()
{
	return m_SRV.GetAddressOf();
}

ID3D11ShaderResourceView** ShaderResourceView::ReleaseGetAddress()
{
	return m_SRV.ReleaseAndGetAddressOf();
}

void ShaderResourceView::GetDesc(D3D11_SHADER_RESOURCE_VIEW_DESC* desc)
{
	assert(m_SRV);

	m_SRV->GetDesc(desc);
}

UnorderedAccessView::UnorderedAccessView(ID3D11UnorderedAccessView* uav)
	:ResourceBase(RESOURCE_TYPE::UAV), m_UAV(uav)
{

}

UnorderedAccessView::~UnorderedAccessView()
{
	Release();
}

void UnorderedAccessView::Reset()
{
	RESET_COM(m_UAV);
}

void UnorderedAccessView::Release()
{
	RELEASE_COM(m_UAV);
}

ID3D11UnorderedAccessView* UnorderedAccessView::Get()
{
	return m_UAV.Get();
}

ID3D11UnorderedAccessView** UnorderedAccessView::GetAddress()
{
	return m_UAV.GetAddressOf();
}

ID3D11UnorderedAccessView** UnorderedAccessView::ReleaseGetAddress()
{
	return m_UAV.ReleaseAndGetAddressOf();
}

void UnorderedAccessView::GetDesc(D3D11_UNORDERED_ACCESS_VIEW_DESC* desc)
{
	assert(m_UAV);

	m_UAV->GetDesc(desc);
}
