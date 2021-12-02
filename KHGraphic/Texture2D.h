#pragma once
#include "ResourceBase.h"

class Texture2D : public ResourceBase
{
public:
	Texture2D(eResourceType resourceType) : ResourceBase(resourceType), m_Width_Ratio(1.0f), m_Height_Ratio(1.0f) {}
	virtual ~Texture2D() = default;

public:
	virtual ID3D11Texture2D* GetTexture2D() abstract;
	virtual D3D11_TEXTURE2D_DESC GetTextureDesc() abstract;
	virtual D3D11_TEXTURE2D_DESC GetTextureDesc(int width, int height) abstract;

public:
	void SetRatio(float width_ratio, float height_ratio) { m_Width_Ratio = width_ratio; m_Height_Ratio = height_ratio; }
	float GetRatioWidth() { return m_Width_Ratio; }
	float GetRatioHeight() { return m_Height_Ratio; }

protected:
	float m_Width_Ratio;
	float m_Height_Ratio;
};