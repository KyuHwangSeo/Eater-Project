#pragma once
#include "ResourceBase.h"

typedef unsigned int Bind_Mask;

class Texture2D : public ResourceBase
{
public:
	Texture2D(eResourceType resourceType, ID3D11Texture2D* tex2D);
	virtual ~Texture2D() = default;

public:
	void OnResize(int width, int height);
	void SetRatio(float width_ratio, float height_ratio);

public:
	Bind_Mask GetBindType();
	D3D11_TEXTURE2D_DESC* GetTextureDesc();

protected:
	Bind_Mask m_BindResource = 0x00000000;

	D3D11_TEXTURE2D_DESC m_TexDesc;

	float m_Width_Ratio;
	float m_Height_Ratio;
};