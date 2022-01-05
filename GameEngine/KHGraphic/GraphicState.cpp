#include "DirectDefine.h"
#include "GraphicState.h"

DepthStencilState::DepthStencilState(ID3D11DepthStencilState* dss)
	:ResourceBase(RESOURCE_TYPE::DSS), m_DSS(dss)
{
}

DepthStencilState::~DepthStencilState()
{
	Release();
}

void DepthStencilState::Reset()
{
	RESET_COM(m_DSS);
}

void DepthStencilState::Release()
{
	RELEASE_COM(m_DSS);
}

ID3D11DepthStencilState* DepthStencilState::Get()
{
	return m_DSS.Get();
}

ID3D11DepthStencilState** DepthStencilState::GetAddress()
{
	return m_DSS.GetAddressOf();
}

RasterizerState::RasterizerState(ID3D11RasterizerState* rs)
	:ResourceBase(RESOURCE_TYPE::RS), m_RS(rs)
{
}

RasterizerState::~RasterizerState()
{
	Release();
}

void RasterizerState::Reset()
{
	RESET_COM(m_RS);
}

void RasterizerState::Release()
{
	RELEASE_COM(m_RS);
}

ID3D11RasterizerState* RasterizerState::Get()
{
	return m_RS.Get();
}

ID3D11RasterizerState** RasterizerState::GetAddress()
{
	return m_RS.GetAddressOf();
}

BlendState::BlendState(ID3D11BlendState* bs)
	:ResourceBase(RESOURCE_TYPE::BS), m_BS(bs)
{
}

BlendState::~BlendState()
{
	Release();
}

void BlendState::Reset()
{
	RESET_COM(m_BS);
}

void BlendState::Release()
{
	RELEASE_COM(m_BS);
}

ID3D11BlendState* BlendState::Get()
{
	return m_BS.Get();
}

ID3D11BlendState** BlendState::GetAddress()
{
	return m_BS.GetAddressOf();
}

SamplerState::SamplerState(ID3D11SamplerState* ss)
	:ResourceBase(RESOURCE_TYPE::SS), m_SS(ss)
{
}

SamplerState::~SamplerState()
{
	Release();
}

void SamplerState::Reset()
{
	RESET_COM(m_SS);
}

void SamplerState::Release()
{
	RELEASE_COM(m_SS);
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
	:ResourceBase(RESOURCE_TYPE::VP)
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
	Release();
}

void ViewPort::Reset()
{
	SAFE_DELETE(m_ViewPort);
}

void ViewPort::Release()
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
