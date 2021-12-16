//***************************************************************************************
// LightHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper classes for lighting.
//***************************************************************************************

#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <Windows.h>

//#include <xnamath.h>
//#include <DirectXMath.h>
//#include "SimpleMath.h"

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

struct DirectionalLightData
{
	DirectionalLightData() = default;

	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Specular;
	DirectX::SimpleMath::Vector3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct PointLightData
{
	PointLightData() = default;
	
	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Specular;

	// Packed into 4D vector: (Position, Range)
	DirectX::SimpleMath::Vector3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	DirectX::SimpleMath::Vector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct SpotLightData
{
	SpotLightData() = default;
	
	DirectX::SimpleMath::Vector4 Ambient;
	DirectX::SimpleMath::Vector4 Diffuse;
	DirectX::SimpleMath::Vector4 Specular;

	// Packed into 4D vector: (Position, Range)
	DirectX::SimpleMath::Vector3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	DirectX::SimpleMath::Vector3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	DirectX::SimpleMath::Vector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

struct LightData
{
	DirectionalLightData* DirLights[3];
	PointLightData* PointLights[5];
	SpotLightData* SpotLights[5];

	UINT gDirLightCount;
	UINT gPointLightCount;
	UINT gSpotLightCount;
};

struct MaterialData
{
	MaterialData() = default;

	DirectX::SimpleMath::Vector4 Ambient	= DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::SimpleMath::Vector4 Diffuse	= DirectX::SimpleMath::Vector4(0.8f, 0.8f, 0.8f, 1.0f);
	DirectX::SimpleMath::Vector4 Specular	= DirectX::SimpleMath::Vector4(0.4f, 0.4f, 0.4f, 1.0f); // w = SpecPower
	DirectX::SimpleMath::Vector4 Reflect	= DirectX::SimpleMath::Vector4(0.4f, 0.4f, 0.4f, 1.0f);

	bool operator==(const MaterialData& mat)
	{
		return (Ambient == mat.Ambient && Diffuse == mat.Diffuse && Specular == mat.Specular && Reflect == mat.Reflect);
	}
};
#endif // LIGHTHELPER_H