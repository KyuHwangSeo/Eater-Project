#pragma once
#include "HashBase.h"

#define DEPTH_STENCIL_VIEW(ClassName) CREATE_HASH_CLASS(ClassName, RESOURCE_TYPE::DS)

///////////////////////////////////////////////////////////////////////////////////////////
// Global Depth Stencil
///////////////////////////////////////////////////////////////////////////////////////////

DEPTH_STENCIL_VIEW(DS_Defalt)
DEPTH_STENCIL_VIEW(DS_Light)
DEPTH_STENCIL_VIEW(DS_Shadow)