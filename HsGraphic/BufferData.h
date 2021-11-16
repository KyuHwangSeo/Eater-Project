#pragma once

#include "DirectXMath.h"

//카메라 버퍼 
struct CameraBuffer
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
};

//오브젝트 버퍼
struct ObjectBuffer
{
	DirectX::XMMATRIX world;
};

//스키닝 버퍼
struct SkinningBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX BoneOffset[96];
};