#pragma once
#include "HashBase.h"

#define ADD_DEPTH_STENCIL_STATE(ClassName) static bool dss_##ClassName = ShaderResourceHashTable::Get()->Push(eResourceType::DSS, #ClassName, typeid(ClassName).hash_code());
#define DEPTH_STENCIL_STATE(ClassName) CREATE_EMPTY_CLASS(ClassName, DSS_Resource) ADD_DEPTH_STENCIL_STATE(ClassName)

///////////////////////////////////////////////////////////////////////////////////////////
// Global Depth Stencil State
///////////////////////////////////////////////////////////////////////////////////////////

DEPTH_STENCIL_STATE(DSS_Defalt)
DEPTH_STENCIL_STATE(DSS_NoDepth)


/// Resource Hash 재등록 방지 Define
RESOURCE_DEFINE(DEFINE_DSS)