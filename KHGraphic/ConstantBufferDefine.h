#pragma once
#include "SimpleMath.h"
#include "LightHelper.h"
#include "HashBase.h"

#define CONSTANT_BUFFER(ClassName) CREATE_HASH_CLASS(ClassName, eResourceType::CB) RESOURCE_PUSH(ClassName, eResourceType::CB)

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

CONSTANT_BUFFER(cbMeshObject)
struct CB_MeshObject : public cbMeshObject
{
	DirectX::SimpleMath::Matrix gWorld;
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gTexTransform;
	DirectX::SimpleMath::Matrix gShadowTransform;
};

CONSTANT_BUFFER(cbShadowMeshObject)
struct CB_ShadowMeshObject : public cbShadowMeshObject
{
	DirectX::SimpleMath::Matrix gWorldViewProj;
};

CONSTANT_BUFFER(cbSkinObject)
struct CB_SkinObject : public cbSkinObject
{
	DirectX::SimpleMath::Matrix gWorld;
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gTexTransform;
	DirectX::SimpleMath::Matrix gShadowTransform;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

CONSTANT_BUFFER(cbShadowSkinObject)
struct CB_ShadowSkinObject : public cbShadowSkinObject
{
	DirectX::SimpleMath::Matrix gWorldViewProj;
	DirectX::SimpleMath::Matrix gBoneTransforms[96];
};

CONSTANT_BUFFER(cbLightSub)
struct CB_LightSub : public cbLightSub
{
	DirectX::SimpleMath::Vector3 gEyePosW;
	DirectX::SimpleMath::Matrix gViewProjTex;
};

CONSTANT_BUFFER(cbLight)
struct CB_Light : public cbLight
{
	DirectionalLightData gDirLights;
	PointLightData gPointLights[5];
	SpotLightData gSpotLights[5];
	MaterialData gMaterials[5];

	UINT gPointLightCount;
	UINT gSpotLightCount;
};

CONSTANT_BUFFER(cbMaterial)
struct CB_Material : public cbMaterial
{
	int gMatID = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//// SSAO Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////
//
//CONSTANT_BUFFER(cbTexel)
//{
//	float gTexelSize;
//};
//
//CONSTANT_BUFFER(cbSsaoFrame)
//{
//	DirectX::SimpleMath::Matrix  gViewToTexSpace;
//	DirectX::SimpleMath::Vector4 gOffsetVectors[14];
//	DirectX::SimpleMath::Vector4 gFrustumCorners[4];
//
//	float    gOcclusionRadius = 0.5f;
//	float    gOcclusionFadeStart = 0.2f;
//	float    gOcclusionFadeEnd = 2.0f;
//	float    gSurfaceEpsilon = 0.05f;
//};
//
/////////////////////////////////////////////////////////////////////////////////////////////
//// Full Screen Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////
//
//CONSTANT_BUFFER(cbFullScreen)
//{
//	DirectX::SimpleMath::Matrix gNowViewProj;
//	DirectX::SimpleMath::Matrix gPrevViewProj;
//	float gDeltaTime;
//};
//
/////////////////////////////////////////////////////////////////////////////////////////////
//// UI Constant Buffer
/////////////////////////////////////////////////////////////////////////////////////////////
//
//CONSTANT_BUFFER(cbPerUI)
//{
//	DirectX::SimpleMath::Matrix gWorldViewProj;
//};
//
//CONSTANT_BUFFER(cbAlpha)
//{
//	float alpha = 0.0f;
//};



/// Resource Hash 재등록 방지 Define
RESOURCE_DEFINE(DEFINE_CB)