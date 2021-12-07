#include "DirectDefine.h"
#include "Texture2D.h"
#include "DepthStencilView.h"

DepthStencilView::DepthStencilView(ID3D11DepthStencilView** dsv, ID3D11ShaderResourceView** srv)
	:Texture2D(eResourceType::DSV), m_DSV(*dsv)
{
	if (dsv) m_DSV = *dsv;
	if (srv) m_SRV = *srv;
}

DepthStencilView::~DepthStencilView()
{
	RESET_COM(m_DSV);
}

void DepthStencilView::Reset()
{
	RESET_COM(m_DSV);
}

ID3D11Texture2D* DepthStencilView::GetTexture2D()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_DSV->GetResource(&resource);

	return (ID3D11Texture2D*)resource;
}

D3D11_TEXTURE2D_DESC DepthStencilView::GetTextureDesc()
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_DSV->GetResource(&resource);

	// Texture 2D Description 추출..
	D3D11_TEXTURE2D_DESC texDesc;

	((ID3D11Texture2D*)resource)->GetDesc(&texDesc);

	return texDesc;
}

D3D11_TEXTURE2D_DESC DepthStencilView::GetTextureDesc(int width, int height)
{
	ID3D11Resource* resource = nullptr;

	// 현재 View의 Texture 2D Resource..
	m_DSV->GetResource(&resource);

	// Texture 2D Description 추출..
	D3D11_TEXTURE2D_DESC texDesc;

	((ID3D11Texture2D*)resource)->GetDesc(&texDesc);

	// 설정한 비율에 따른 Description 설정..
	texDesc.Width = (UINT)(width * m_Width_Ratio);
	texDesc.Height = (UINT)(height * m_Height_Ratio);

	return texDesc;
}

ID3D11DepthStencilView* DepthStencilView::GetDSV()
{
	return m_DSV.Get();
}

ID3D11DepthStencilView** DepthStencilView::GetAddressDSV()
{
	return m_DSV.GetAddressOf();
}

D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilView::GetDSVDesc()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	m_DSV->GetDesc(&dsvDesc);

	return dsvDesc;
}

ID3D11ShaderResourceView* DepthStencilView::GetSRV()
{
	return m_SRV.Get();
}

ID3D11ShaderResourceView** DepthStencilView::GetAddressSRV()
{
	return m_SRV.GetAddressOf();
}

D3D11_SHADER_RESOURCE_VIEW_DESC DepthStencilView::GetSRVDesc()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	m_SRV->GetDesc(&srvDesc);

	return srvDesc;
}

bool DepthStencilView::IsDSV()
{
	if (m_DSV) return true;

	return false;
}

bool DepthStencilView::IsSRV()
{
	if (m_SRV) return true;

	return false;
}
