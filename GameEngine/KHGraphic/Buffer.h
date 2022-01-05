#pragma once
#include "EnumDefine.h"

/// 2022/01/04 19:20
/// SeoKyuHwang
///
/// # Buffer Class
///
/// - Rendering에 필요한 Resource Buffer Class
/// 

class Buffer
{
public:
	Buffer(ID3D11Buffer* buffer);
	~Buffer();

public:
	void OnResize(UINT byteWidth);
	void SetResize();

	void Reset();
	void Release();

public:
	ID3D11Buffer* Get();
	ID3D11Buffer** GetAddress();
	ID3D11Buffer** ReleaseGetAddress();

	void GetDesc(D3D11_BUFFER_DESC* desc);

	bool GetResize();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;

	UINT m_ByteWidth;

	bool m_IsResize;
};