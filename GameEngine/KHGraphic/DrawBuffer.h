#pragma once
#include "ResourceBase.h"

/// 2021/12/05 21:17
/// SeoKyuHwang
///
/// # DrawBuffer Class
///
/// - Rendering에 필요한 기본 Vertex, Index Buffer Class
///

class DrawBuffer : public ResourceBase
{
public:
	DrawBuffer(Buffer* vb, Buffer* ib);
	~DrawBuffer();

public:
	void Reset() override;
	void Release() override;

public:
	Buffer* VB;		// Vertex Buffer
	Buffer* IB;		// Index Buffer

	UINT IndexCount;		// Index Count
	UINT Stride;			// Vertex Struct Size
	UINT Offset;			// Offset
};
