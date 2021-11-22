#pragma once
#ifdef MULTIRENDERENGINE_EXPORTS
#define MULTIENGINE_DLL __declspec(dllexport)
#else
#define MULTIENGINE_DLL __declspec(dllimport)

#ifdef _DEBUG
#pragma comment(lib,"MultiRenderEngine_x64d")
#else
#pragma comment(lib,"MultiRenderEngine_x64r")
#endif

#endif

#include <windows.h>
#include <map>
#include <queue>
#include <string>