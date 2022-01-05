#pragma once

/// 2021/11/04 12:20
/// SeoKyuHwang
///
/// # Texture2D Class
///
/// - Rendering에 필요한 Resource Texture2D Class
/// 

class Texture2D
{
public:
	Texture2D(ID3D11Texture2D* tex2D);
	~Texture2D();

public:
	void OnResize(int width, int height);
	void SetRatio(float width_ratio, float height_ratio);

	void Reset();
	void Release();

public:
	ID3D11Texture2D* Get();
	ID3D11Texture2D** GetAddress();
	ID3D11Texture2D** ReleaseGetAddress();

	void GetDesc(D3D11_TEXTURE2D_DESC* desc);

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Tex2D;

	UINT m_Width;
	UINT m_Height;

	float m_Width_Ratio;
	float m_Height_Ratio;
};

