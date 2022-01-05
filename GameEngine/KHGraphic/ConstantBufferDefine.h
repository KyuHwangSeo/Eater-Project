#pragma once
#include "SimpleMath.h"
#include "HashBase.h"
#include "LightHelper.h"
#include "EngineData.h"

#define SHADER_CONSTANT_BUFFER(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::CB) RESOURCE_PUSH(ClassName, RESOURCE_TYPE::CB)

/// <summary>
/// ConstantBuffer Resource Struct
/// </summary>
/// 
/// - Shader 내부의 ConstantBuffer Resource와 1:1 대응하는 Struct
/// - Shader에서 설정한 레지스터 번호와 버퍼가 일치하는 한개의 Constant Buffer
/// - D3DCompiler에서 현재 Shader에서 ConstantBuffer을 선언해 두어도 사용하지 않으면
///   Register Slot에 올리지 않는다..
/// 
/// - 주의점
///  1) 현재 struct의 이름과 변수 순서는 Shader 내부의 ConstantBuffer와 일치해야한다 (Struct Name을 통해 Constant Buffer를 찾기 때문)

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred Constant Buffer
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbMeshObject)
struct CB_MeshObject : public cbMeshObject
{
	DirectX::SimpleMath::Matrix gWorld;
	DirectX::SimpleMath::Matrix gWorldView;
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gTexTransform;
	DirectX::SimpleMath::Matrix gShadowTransform;
};

SHADER_CONSTANT_BUFFER(cbShadowMeshObject)
struct CB_ShadowMeshObject : public cbShadowMeshObject
{
	DirectX::SimpleMath::Matrix gWorldViewProj;
};

SHADER_CONSTANT_BUFFER(cbSkinObject)
struct CB_SkinObject : public cbSkinObject
{
	DirectX::SimpleMath::Matrix gWorld;
	DirectX::SimpleMath::Matrix gWorldView;
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gTexTransform;
	DirectX::SimpleMath::Matrix gShadowTransform;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

SHADER_CONSTANT_BUFFER(cbShadowSkinObject)
struct CB_ShadowSkinObject : public cbShadowSkinObject
{
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

SHADER_CONSTANT_BUFFER(cbLightSub)
struct CB_LightSub : public cbLightSub
{
	DirectX::SimpleMath::Matrix gViewProjTex;
	DirectX::SimpleMath::Vector3 gEyePosW;
}; 

SHADER_CONSTANT_BUFFER(cbLight)
struct CB_Light : public cbLight
{
	DirectionalLightData gDirLights;
	PointLightData gPointLights[5];
	SpotLightData gSpotLights[5];
	MaterialOption gMaterials[5];

	UINT gPointLightCount;
	UINT gSpotLightCount;
};

SHADER_CONSTANT_BUFFER(cbMaterial)
struct CB_Material : public cbMaterial
{
	DirectX::SimpleMath::Vector4 gColor = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	
	UINT gMatID = 0;
	UINT gTexID = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// SSAO Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbBlurOption)
struct CB_BlurOption : public cbBlurOption
{
	DirectX::SimpleMath::Vector2 gTexelSize;
	bool gHorizon;
};

SHADER_CONSTANT_BUFFER(cbSsaoObject)
struct CB_SsaoObject : public cbSsaoObject
{
	DirectX::SimpleMath::Matrix  gViewToTexSpace;
};

SHADER_CONSTANT_BUFFER(cbSsaoFrustum)
struct CB_SsaoFrustum : public cbSsaoFrustum
{
	DirectX::SimpleMath::Vector4 gFrustumCorners[4];
};

SHADER_CONSTANT_BUFFER(cbSsaoOption)
struct CB_SsaoOption : public cbSsaoOption
{
	DirectX::SimpleMath::Vector4 gOffsetVectors[14];

	float    gOcclusionRadius = 0.1f;
	float    gOcclusionFadeStart = 0.01f;
	float    gOcclusionFadeEnd = 1.0f;
	float    gSurfaceEpsilon = 0.000001f;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// Particle Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbParticleObject)
struct CB_ParticleObject : public cbParticleObject
{
	DirectX::SimpleMath::Matrix  gWorld;
	DirectX::SimpleMath::Matrix  gWorldView;
	DirectX::SimpleMath::Matrix  gWorldViewProj;
	DirectX::SimpleMath::Matrix  gTexTransform;
};

SHADER_CONSTANT_BUFFER(cbParticleOption)
struct CB_ParticleOption : public cbParticleOption
{
	DirectX::SimpleMath::Vector4 gColor;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// OIT Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////

SHADER_CONSTANT_BUFFER(cbOitFrame)
struct CB_OitFrame : public cbOitFrame
{
	UINT gFrameWidth;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// Debug Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////
SHADER_CONSTANT_BUFFER(cbDebugObject)
struct CB_DebugObject : public cbDebugObject
{
	DirectX::SimpleMath::Matrix  gWorldViewProj;
};


/// Resource Hash 재등록 방지 Define
RESOURCE_DEFINE(DEFINE_CB)