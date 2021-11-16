#pragma once

#include <SimpleMath.h>
#include <DirectXMath.h>
struct Deferred32
{
	//위치
	DirectX::SimpleMath::Vector3 Pos;
	//텍스쳐
	DirectX::SimpleMath::Vector2 Tex;
	//노말
	DirectX::SimpleMath::Vector3 Nomal;
	//탄젠트
	DirectX::SimpleMath::Vector3 Tangent;
};

struct Skinning32
{
	//위치
	DirectX::SimpleMath::Vector3 Pos;
	//텍스쳐
	DirectX::SimpleMath::Vector2 Tex;
	//노말
	DirectX::SimpleMath::Vector3 Nomal;
	//탄젠트
	DirectX::SimpleMath::Vector3 Tangent;


	UINT BoneIndex[4];
	float BoneWeights[4];
};