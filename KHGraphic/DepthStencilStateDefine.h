#pragma once
#include "HashBase.h"

#define ADD_DEPTH_STENCIL_STATE(ClassName) static bool dss_##ClassName = ShaderResourceHashTable::Get()->Push(eResourceType::DSS, #ClassName, typeid(ClassName).hash_code());
#define DEPTH_STENCIL_STATE(ClassName) CREATE_EMPTY_CLASS(ClassName) ADD_DEPTH_STENCIL_STATE(ClassName)
