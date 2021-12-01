#pragma once
#include "ResourceBase.h"

///
/// 2021/11/08 2:34
/// SeoKyuHwang
///
/// ViewPort Class
///
/// - ViewPort Data Class
/// - Resource 관리를 위해 추가

class ViewPort : public ResourceBase
{
public:
	ViewPort(float topX, float topY, float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f);
	~ViewPort();

public:
	void OnResize(int width, int height);

	D3D11_VIEWPORT* Get();

private:
	D3D11_VIEWPORT* m_ViewPort;

	float m_Width_Ratio;
	float m_Height_Ratio;
};

