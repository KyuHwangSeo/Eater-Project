#pragma once
#include "HashBase.h"

#define RANDER_TARGET(ClassName) CREATE_HASH_CLASS(ClassName, eResourceType::RT)

///////////////////////////////////////////////////////////////////////////////////////////
// Global RenderTarget
///////////////////////////////////////////////////////////////////////////////////////////

RANDER_TARGET(RT_Deffered_Albedo)
RANDER_TARGET(RT_Deffered_Normal)
RANDER_TARGET(RT_Deffered_Position)
RANDER_TARGET(RT_Deffered_Shadow)
RANDER_TARGET(RT_Deffered_Depth)

RANDER_TARGET(RT_SSAO_Main)
RANDER_TARGET(RT_SSAO_Blur)
