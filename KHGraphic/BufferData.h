#pragma once
#include "ResourceBase.h"

/// 2021/12/05 21:17
/// SeoKyuHwang
///
/// # BufferData Class
///
/// - Rendering에 필요한 기본 Buffer Class
/// 
class BufferData : public ResourceBase
{
public:
	BufferData();
	~BufferData();

public:
	void Reset() override;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB;		// Vertex Buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB;		// Index Buffer

	UINT IndexCount;		// Index Count
	UINT Stride;			// Vertex Struct Size
	UINT Offset;			// Offset
};
