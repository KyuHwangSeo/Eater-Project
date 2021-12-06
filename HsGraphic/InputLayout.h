#pragma once

#include <SimpleMath.h>
#include <DirectXMath.h>
struct Deferred32
{
	//위치
	DirectX::SimpleMath::Vector3 Pos;
	//노말
	DirectX::SimpleMath::Vector2 Tex;
	//텍스쳐
	DirectX::SimpleMath::Vector3 Nomal;
	//탄젠트
	DirectX::SimpleMath::Vector3 Tangent;
};

struct Skinning32
{
	UINT BoneIndex01[4];
	UINT BoneIndex02[4];

	float BoneWeights01[4];
	float BoneWeights02[4];
	
	//위치
	DirectX::SimpleMath::Vector3 Pos;
	//텍스쳐
	DirectX::SimpleMath::Vector2 Tex;
	//노말
	DirectX::SimpleMath::Vector3 Nomal;
	//탄젠트
	DirectX::SimpleMath::Vector3 Tangent;


};

struct Debug32
{
	//위치
	DirectX::XMFLOAT3 Pos;
	//컬러
	DirectX::XMFLOAT3 Color;
};