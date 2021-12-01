#pragma once
#include "HashBase.h"

#define VIEW_PORT(ClassName) CREATE_EMPTY_CLASS(ClassName, VP_Resource)

///////////////////////////////////////////////////////////////////////////////////////////
// Global ViewPort
///////////////////////////////////////////////////////////////////////////////////////////

VIEW_PORT(VP_FullScreen)
VIEW_PORT(VP_Shadow)