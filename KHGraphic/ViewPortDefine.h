#pragma once
#include "HashBase.h"

#define ADD_VIEW_PORT(ClassName) static bool rs_##ClassName = ShaderResourceHashTable::Get()->Push(eResourceType::RS, #ClassName, typeid(ClassName).hash_code());
#define VIEW_PORT(ClassName) CREATE_EMPTY_CLASS(ClassName, VP_Resource) ADD_VIEW_PORT(ClassName)
