#pragma once
#include "HashBase.h"

#define ADD_RANDER_TARGER(ClassName) static bool rs_##ClassName = ShaderResourceHashTable::Get()->Push(eResourceType::RS, #ClassName, typeid(ClassName).hash_code());
#define RANDER_TARGER(ClassName) CREATE_EMPTY_CLASS(ClassName, RT_Resource) ADD_RANDER_TARGER(ClassName)
