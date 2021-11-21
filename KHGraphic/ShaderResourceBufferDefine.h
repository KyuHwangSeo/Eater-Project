#pragma once
#include "ClassType.h"

#define ADD_SHADER_RESOURCE_VIEW(ClassName) static bool srv_##ClassName = ShaderResourceHashTable::GetInstance()->Push<ClassName>(ShaderResourceHashTable::BufferType::SRV, #ClassName, ClassName::GetHashCode());
#define SHADER_RESOURCE_VIEW(ClassName) CREATE_EMPTY_CLASS(ClassName) ADD_SHADER_RESOURCE_VIEW(ClassName)

/// <summary>
/// ShaderResourceView Resource Struct
/// </summary>
/// 
/// - Shader ������ ShaderResourceView Resource�� 1:1 �����ϴ� Struct
/// - Shader���� ������ �������� ��ȣ�� ���۰� ��ġ�ϴ� �Ѱ��� ShaderResourceView
/// - D3DCompiler���� ���� Shader���� ShaderResourceView�� ������ �ξ ������� ������
///   Register Slot�� �ø��� �ʴ´�..
/// 
/// - ������
///  1) ���� struct�� �̸��� ���� ������ Shader ������ UnorderedAccessView�� ��ġ�ؾ��Ѵ� (Struct Name�� ���� UnorderedAccessView�� ã�� ����)

///////////////////////////////////////////////////////////////////////////////////////////
// Global ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gDiffuseMap)
SHADER_RESOURCE_VIEW(gNormalMap)
SHADER_RESOURCE_VIEW(gCubeMap)
SHADER_RESOURCE_VIEW(gShadowMap)
SHADER_RESOURCE_VIEW(gSSAOMap)

///////////////////////////////////////////////////////////////////////////////////////////
// SSAO ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gNormalDepthMap)
SHADER_RESOURCE_VIEW(gRandomVecMap)
SHADER_RESOURCE_VIEW(gInputMap)

///////////////////////////////////////////////////////////////////////////////////////////
// Deferred ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(AlbedoSRV)
SHADER_RESOURCE_VIEW(NormalSRV)
SHADER_RESOURCE_VIEW(PositionSRV)
SHADER_RESOURCE_VIEW(ShadowSRV)
SHADER_RESOURCE_VIEW(SsaoSRV)

///////////////////////////////////////////////////////////////////////////////////////////
// Blur ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gInput)

///////////////////////////////////////////////////////////////////////////////////////////
// MotionBlur ShaderResourceView Resource
///////////////////////////////////////////////////////////////////////////////////////////

SHADER_RESOURCE_VIEW(gDepthMap)
SHADER_RESOURCE_VIEW(gOrigin)
SHADER_RESOURCE_VIEW(gVelocity)