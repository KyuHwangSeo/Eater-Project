#pragma once
#include "HashBase.h"

#define BUFFER_DATA(ClassName) CREATE_HASH_CLASS(ClassName, eResourceType::BD)

///////////////////////////////////////////////////////////////////////////////////////////
// Global BufferData
///////////////////////////////////////////////////////////////////////////////////////////

BUFFER_DATA(BD_Quad)
BUFFER_DATA(BD_SSAO)

BUFFER_DATA(BD_Line_Quad)
BUFFER_DATA(BD_Line_Axis)
BUFFER_DATA(BD_Line_Circle)
BUFFER_DATA(BD_Line_Box)
