#pragma once
#include "HashBase.h"

#define RANDER_TARGET(ClassName) CREATE_HASH_CLASS(ClassName, eResourceType::RT)

///////////////////////////////////////////////////////////////////////////////////////////
// Global RenderTarget
///////////////////////////////////////////////////////////////////////////////////////////

RANDER_TARGET(RT_Shadow)