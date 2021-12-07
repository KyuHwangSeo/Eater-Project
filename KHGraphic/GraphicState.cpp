#include "DirectDefine.h"
#include "GraphicState.h"

DepthStencilState::DepthStencilState(ID3D11DepthStencilState** dss)
	:ResourceBase(eResourceType::DSS)
{
	if (dss) m_DSS = *dss;
}

DepthStencilState::~DepthStencilState()
{
	Reset();
}

void DepthStencilState::Reset()
{
	RESET_COM(m_DSS);
}

ID3D11DepthStencilState* DepthStencilState::Get()
{
	return m_DSS.Get();
}

ID3D11DepthStencilState** DepthStencilState::GetAddress()
{
	return m_DSS.GetAddressOf();
}

RasterizerState::RasterizerState(ID3D11RasterizerState** rs)
	:ResourceBase(eResourceType::RS)
{
	if (rs) m_RS = *rs;
}

RasterizerState::~RasterizerState()
{
	Reset();
}

void RasterizerState::Reset()
{
	RESET_COM(m_RS);
}

ID3D11RasterizerState* RasterizerState::Get()
{
	return m_RS.Get();
}

ID3D11RasterizerState** RasterizerState::GetAddress()
{
	return m_RS.GetAddressOf();
}

BlendState::BlendState(ID3D11BlendState** bs)
	:ResourceBase(eResourceType::BS)
{
	if (bs) m_BS = *bs;
}

BlendState::~BlendState()
{
	Reset();
}

void BlendState::Reset()
{
	RESET_COM(m_BS);
}

ID3D11BlendState* BlendState::Get()
{
	return m_BS.Get();
}

ID3D11BlendState** BlendState::GetAddress()
{
	return m_BS.GetAddressOf();
}

SamplerState::SamplerState(ID3D11SamplerState** ss)
	:ResourceBase(eResourceType::SS)
{
	if (ss) m_SS = *ss;
}

SamplerState::~SamplerState()
{
	Reset();
}

void SamplerState::Reset()
{
	RESET_COM(m_SS);
}

ID3D11SamplerState* SamplerState::Get()
{
	return m_SS.Get();
}

ID3D11SamplerState** SamplerState::GetAddress()
{
	return m_SS.GetAddressOf();

}

ViewPort::ViewPort(float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height)
	:ResourceBase(eResourceType::VP)
{
	m_OffsetX = ratio_offsetX;
	m_OffsetY = ratio_offsetY;
	m_SizeX = ratio_sizeX;
	m_SizeY = ratio_sizeY;

	m_ViewPort = new D3D11_VIEWPORT();
	m_ViewPort->TopLeftX = ratio_offsetX * width;
	m_ViewPort->TopLeftY = ratio_offsetY * height;
	m_ViewPort->Width = ratio_sizeX * width;
	m_ViewPort->Height = ratio_sizeY * height;
	m_ViewPort->MinDepth = 0.0f;
	m_ViewPort->MaxDepth = 1.0f;
}

ViewPort::~ViewPort()
{
	SAFE_DELETE(m_ViewPort);
}

void ViewPort::OnResize(int width, int height)
{
	m_ViewPort->TopLeftX = m_OffsetX * (float)width;
	m_ViewPort->TopLeftY = m_OffsetY * (float)height;
	m_ViewPort->Width	 = m_SizeX * (float)width;
	m_ViewPort->Height	 = m_SizeY * (float)height;
}

D3D11_VIEWPORT* ViewPort::Get()
{
	return m_ViewPort;
}
