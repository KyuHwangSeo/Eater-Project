#include "DirectDefine.h"
#include "Texture2D.h"

Texture2D::Texture2D(eResourceType resourceType, ID3D11Texture2D* tex2D)
	:ResourceBase(resourceType), m_Width_Ratio(1.0f), m_Height_Ratio(1.0f)
{
	// Texture 2D Description 저장..
	tex2D->GetDesc(&m_TexDesc);

	// Texture 2D BindFlag 저장..
	m_BindResource = m_TexDesc.BindFlags;
}

void Texture2D::OnResize(int width, int height)
{
	m_TexDesc.Width = (UINT)(width * m_Width_Ratio);
	m_TexDesc.Height = (UINT)(height * m_Height_Ratio);
}

void Texture2D::SetRatio(float width_ratio, float height_ratio)
{
	m_Width_Ratio = width_ratio;
	m_Height_Ratio = height_ratio;
}

Bind_Mask Texture2D::GetBindType()
{
	return m_BindResource;
}

D3D11_TEXTURE2D_DESC* Texture2D::GetTextureDesc()
{
	return &m_TexDesc;
}
