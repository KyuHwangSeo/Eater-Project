#include "DirectDefine.h"
#include "Texture2D.h"

Texture2D::Texture2D(ID3D11Texture2D* tex2D)
	:m_Tex2D(tex2D), m_Width_Ratio(1.0f), m_Height_Ratio(1.0f)
{

}

Texture2D::~Texture2D()
{
	Release();
}

void Texture2D::OnResize(int width, int height)
{
	m_Width = (UINT)(width * m_Width_Ratio);
	m_Height = (UINT)(height * m_Height_Ratio);
}

void Texture2D::SetRatio(float width_ratio, float height_ratio)
{
	m_Width_Ratio = width_ratio;
	m_Height_Ratio = height_ratio;
}

void Texture2D::Reset()
{
	m_Tex2D.Reset();
}

void Texture2D::Release()
{
	RELEASE_COM(m_Tex2D);
}

ID3D11Texture2D* Texture2D::Get()
{
	return m_Tex2D.Get();
}

ID3D11Texture2D** Texture2D::GetAddress()
{
	return m_Tex2D.GetAddressOf();
}

ID3D11Texture2D** Texture2D::ReleaseGetAddress()
{
	return m_Tex2D.ReleaseAndGetAddressOf();
}

void Texture2D::GetDesc(D3D11_TEXTURE2D_DESC* desc)
{
	assert(m_Tex2D);

	m_Tex2D->GetDesc(desc);

	desc->Width = m_Width;
	desc->Height = m_Height;
}