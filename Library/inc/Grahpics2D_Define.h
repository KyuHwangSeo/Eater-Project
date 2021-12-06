#pragma once

///윈도우 제공 D2D & D3D 기본 라이브러리 
#include <wincodec.h>
#include <d2d1.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <dxgi.h>

/// D2D용 라이브러리.
// WIC 사용을 위함.
#pragma comment(lib, "WindowsCodecs.lib")
/// DX11 SwapChain 용 라이브러리.
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite")


/// 기본 라이브러리
#include <stdio.h>
#include <iostream>
#include <string>
#include <limits>
#include <atlstr.h>
#include <map>



#ifdef Graphcis2D_Interface
#define Graphics2D_DLL __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#else
#define Graphics2D_DLL __declspec(dllexport)
#define EXPIMP_TEMPLATE
#endif
