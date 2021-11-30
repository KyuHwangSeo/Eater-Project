#pragma once
#include "HashBase.h"

#define ADD_BLEND_STATE(ClassName) static bool bs_##ClassName = ShaderResourceHashTable::Get()->Push(eResourceType::BS, #ClassName, typeid(ClassName).hash_code());
#define BLEND_STATE(ClassName) CREATE_EMPTY_CLASS(ClassName, BS_Resource) ADD_BLEND_STATE(ClassName)

///////////////////////////////////////////////////////////////////////////////////////////
// Global Blend State
///////////////////////////////////////////////////////////////////////////////////////////

BLEND_STATE(BS_Defalt)
BLEND_STATE(BS_AlphaBlend)


/// Resource Hash 재등록 방지 Define
RESOURCE_DEFINE(DEFINE_BS)