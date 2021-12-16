#pragma once
#include "SimpleMath.h"

struct MeshVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector2 Tex;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector3 Tangent;
};

struct SkinVertex
{
	UINT BoneIndex1[4];
	UINT BoneIndex2[4];
	float BoneWeight1[4];
	float BoneWeight2[4];

	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector2 Tex;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector3 Tangent;
};


struct TerrainVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector2 Tex;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector3 Tangent;
	DirectX::SimpleMath::Vector4 Mask;
};

struct PosMeshVertex
{
	DirectX::SimpleMath::Vector3 Pos;
};

struct PosSkinVertex
{
	UINT BoneIndex1[4];
	UINT BoneIndex2[4];
	float BoneWeight1[4];
	float BoneWeight2[4];

	DirectX::SimpleMath::Vector3 Pos;
};

struct PosTexVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector2 Tex;
};

struct PosNormalTexVertex
{
	DirectX::SimpleMath::Vector3 Pos;
	DirectX::SimpleMath::Vector2 Tex;
	DirectX::SimpleMath::Vector3 Normal;
};
