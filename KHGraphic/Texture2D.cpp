#include "DirectDefine.h"
#include "Texture2D.h"

Texture2D::Texture2D(eResourceType resourceType, ID3D11Texture2D* tex2D)
	:ResourceBase(resourceType), m_Width_Ratio(1.0f), m_Height_Ratio(1.0f)
{
	tex2D->GetDesc(&m_TexDesc);
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

void Texture2D::SetResourceBind(Bind_Mask bind_type)
{
	m_BindResource |= bind_type;
}

Bind_Mask Texture2D::GetBindType()
{
	return m_BindResource;
}

D3D11_TEXTURE2D_DESC* Texture2D::GetTextureDesc()
{
	return &m_TexDesc;
}
