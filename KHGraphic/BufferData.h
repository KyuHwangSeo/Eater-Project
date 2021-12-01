#pragma once
#include "ResourceBase.h"

class BufferData : public ResourceBase
{
public:
	BufferData();
	~BufferData();

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB;		// Vertex Buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB;		// Index Buffer

	UINT IndexCount;		// Index Count
	UINT Stride;			// Vertex Struct Size
	UINT Offset;			// Offset
};
