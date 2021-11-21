#pragma once
#include "ClassType.h"

#define ADD_UNORDERED_ACCESS_VIEW(ClassName) static bool uav_##ClassName = ShaderResourceHashTable::GetInstance()->Push<ClassName>(ShaderResourceHashTable::BufferType::UAV, #ClassName, ClassName::GetHashCode());
#define UNORDERED_ACCESS_VIEW(ClassName) CREATE_EMPTY_CLASS(ClassName) ADD_UNORDERED_ACCESS_VIEW(ClassName)

/// <summary>
/// UnorderedAccessView Resource Struct
/// </summary>
/// 
/// - Shader ������ UnorderedAccessView Resource�� 1:1 �����ϴ� Struct
/// - Shader���� ������ �������� ��ȣ�� ���۰� ��ġ�ϴ� �Ѱ��� UnorderedAccessView
/// - D3DCompiler���� ���� Shader���� UnorderedAccessView�� ������ �ξ ������� ������
///   Register Slot�� �ø��� �ʴ´�..
/// 
/// - ������
///  1) ���� struct�� �̸��� ���� ������ Shader ������ UnorderedAccessView�� ��ġ�ؾ��Ѵ� (Struct Name�� ���� UnorderedAccessView�� ã�� ����)

///////////////////////////////////////////////////////////////////////////////////////////
// Blur UnorderedAccessView Resource
///////////////////////////////////////////////////////////////////////////////////////////

UNORDERED_ACCESS_VIEW(gOutput)