#pragma once
#include "HashBase.h"

#define BUFFER_DATA(ClassName) CREATE_HASH_CLASS(ClassName, eResourceType::BD)

///////////////////////////////////////////////////////////////////////////////////////////
// Global BufferData
///////////////////////////////////////////////////////////////////////////////////////////

BUFFER_DATA(BD_Quad)
BUFFER_DATA(BD_SSAO)
