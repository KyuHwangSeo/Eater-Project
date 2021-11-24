#pragma once
#include "HashBase.h"

#define ADD_DEPTH_STENCIL_VIEW(ClassName) static bool dsv_##ClassName = ShaderResourceHashTable::Get()->Push(eResourceType::DSV, #ClassName, typeid(ClassName).hash_code());
#define DEPTH_STENCIL_VIEW(ClassName) CREATE_EMPTY_CLASS(ClassName) ADD_DEPTH_STENCIL_VIEW(ClassName)
