#pragma once

#include "SimpleMath.h"
struct MeshVertex
{
	//À§Ä¡
	DirectX::SimpleMath::Vector3 Pos;
	//uv
	DirectX::SimpleMath::Vector2 Tex;
	//³ë¸»
	DirectX::SimpleMath::Vector3 Normal;
	//ÅºÁ¨Æ®
	DirectX::SimpleMath::Vector3 Tangent;
};

struct SkinVertex
{
	//º» ÀÎµ¦½º
	UINT BoneIndex1[4];
	UINT BoneIndex2[4];

	//º» ¿þÀÌÆÃ
	float BoneWeight1[4];
	float BoneWeight2[4];

	//À§Ä¡
	DirectX::SimpleMath::Vector3 Pos;
	//uv
	DirectX::SimpleMath::Vector2 Tex;
	//³ë¸»
	DirectX::SimpleMath::Vector3 Normal;
	//ÅºÁ¨Æ®
	DirectX::SimpleMath::Vector3 Tangent;
};