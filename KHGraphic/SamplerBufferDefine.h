#pragma once
#include "ClassType.h"

#define ADD_SAMPLER_STATE(ClassName) static bool sampler_##ClassName = ShaderResourceHashTable::GetInstance()->Push<ClassName>(ShaderResourceHashTable::BufferType::SAMPLER, #ClassName, ClassName::GetHashCode());
#define SAMPLER_STATE(ClassName) CREATE_EMPTY_CLASS(ClassName) ADD_SAMPLER_STATE(ClassName)

/// <summary>
/// SamplerState Resource Struct
/// </summary>
/// 
/// - Shader ������ SamplerState Resource�� 1:1 �����ϴ� Struct
/// - Shader���� ������ �������� ��ȣ�� ���۰� ��ġ�ϴ� �Ѱ��� SamplerState
/// - D3DCompiler���� ���� Shader���� SamplerState�� ������ �ξ ������� ������
///   Register Slot�� �ø��� �ʴ´�..
/// 
/// - ������
///  1) ���� struct�� �̸��� ���� ������ Shader ������ SamplerState�� ��ġ�ؾ��Ѵ� (Struct Name�� ���� SamplerState�� ã�� ����)

///////////////////////////////////////////////////////////////////////////////////////////
// Global SamplerState Resource
///////////////////////////////////////////////////////////////////////////////////////////

SAMPLER_STATE(gShadowSam)
SAMPLER_STATE(samWrapMinLinear)
SAMPLER_STATE(samWrapAnisotropic)
SAMPLER_STATE(samClampMinLinear)
SAMPLER_STATE(samWrapLinerPoint)
SAMPLER_STATE(samBorderLinerPoint)
SAMPLER_STATE(samClampMinLinearPoint)
SAMPLER_STATE(samMirrorMinLinear)