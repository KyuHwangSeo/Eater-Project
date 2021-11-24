#pragma once
#include "HashBase.h"

#define ADD_RASTERIZER_STATE(ClassName) static bool rs_##ClassName = ShaderResourceHashTable::Get()->Push(eResourceType::RS, #ClassName, typeid(ClassName).hash_code());
#define RASTERIZER_STATE(ClassName) CREATE_EMPTY_CLASS(ClassName) ADD_RASTERIZER_STATE(ClassName)
