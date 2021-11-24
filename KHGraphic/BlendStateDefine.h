#pragma once
#include "HashBase.h"

#define ADD_BLEND_STATE(ClassName) static bool bs_##ClassName = ShaderResourceHashTable::Get()->Push(eResourceType::BS, #ClassName, typeid(ClassName).hash_code());
#define BLEND_STATE(ClassName) CREATE_EMPTY_CLASS(ClassName) ADD_BLEND_STATE(ClassName)
