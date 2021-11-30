#include "DirectDefine.h"
#include "GraphicState.h"

DepthStencilState::DepthStencilState(ID3D11DepthStencilState** dss)
	:ResourceBase(eResourceType::DSS)
{
	if (dss) m_DSS = *dss;
}

DepthStencilState::~DepthStencilState()
{

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

}

ID3D11SamplerState* SamplerState::Get()
{

	return m_SS.Get();
}

ID3D11SamplerState** SamplerState::GetAddress()
{
	return m_SS.GetAddressOf();

}
