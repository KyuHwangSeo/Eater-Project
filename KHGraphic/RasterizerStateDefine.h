#pragma once
#include "HashBase.h"

#define ADD_RASTERIZER_STATE(ClassName) static bool rs_##ClassName = ShaderResourceHashTable::Get()->Push(eResourceType::RS, #ClassName, typeid(ClassName).hash_code());
#define RASTERIZER_STATE(ClassName) CREATE_EMPTY_CLASS(ClassName, RS_Resource) ADD_RASTERIZER_STATE(ClassName)

///////////////////////////////////////////////////////////////////////////////////////////
// Global Rasterizer State
///////////////////////////////////////////////////////////////////////////////////////////

RASTERIZER_STATE(RS_Solid)
RASTERIZER_STATE(RS_WireFrame)
RASTERIZER_STATE(RS_CullNone)
RASTERIZER_STATE(RS_Depth)

/// Resource Hash 재등록 방지 Define
RESOURCE_DEFINE(DEFINE_RS)