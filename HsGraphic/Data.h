#pragma once
#include "d3d11.h"

//쉐이더들을 보관할 데이터
struct ShaderData
{
	ID3D11VertexShader* VertexShader	= nullptr;
	ID3D11PixelShader* PixelShader		= nullptr;
	ID3D11InputLayout* Layout			= nullptr;
};

struct BufferData
{
	ID3D11Buffer* IB;
	ID3D11Buffer* VB;

	int IndexCount;
	int VertexCount;

	int VertexDataSize;
};


