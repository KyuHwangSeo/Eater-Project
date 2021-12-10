#include "DirectDefine.h"
#include "GraphicView.h"

ShaderResourceView::ShaderResourceView(ID3D11ShaderResourceView* srv)
	:ResourceBase(eResourceType::SRV), m_SRV(srv)
{

}

ShaderResourceView::~ShaderResourceView()
{
	Reset();
}

void ShaderResourceView::Reset()
{
	RESET_COM(m_SRV);
}

ID3D11ShaderResourceView* ShaderResourceView::Get()
{
	return m_SRV.Get();
}

ID3D11ShaderResourceView** ShaderResourceView::GetAddress()
{
	return m_SRV.GetAddressOf();
}

UnorderedAccessView::UnorderedAccessView(ID3D11UnorderedAccessView* uav)
	:ResourceBase(eResourceType::UAV), m_UAV(uav)
{

}

UnorderedAccessView::~UnorderedAccessView()
{
	Reset();
}

void UnorderedAccessView::Reset()
{
	RESET_COM(m_UAV);
}

ID3D11UnorderedAccessView* UnorderedAccessView::Get()
{
	return m_UAV.Get();
}

ID3D11UnorderedAccessView** UnorderedAccessView::GetAddress()
{
	return m_UAV.GetAddressOf();
}
