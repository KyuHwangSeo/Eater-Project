#pragma once

//D3D 기본 라이브러리 
////////////////////////////////////////////////

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler") //쉐이더
#pragma comment(lib, "dinput8")
////////////////////////////////////////////////

//텍스쳐 불러올떄
////////////////////////////////////////////////
#ifdef _DEBUG
	#pragma comment(lib,"DirectXTK_x64d")
#else
	#pragma comment(lib,"DirectXTK_x64r")	
#endif
////////////////////////////////////////////////
#include "DDSTextureLoader.h"


#include <d3dcompiler.h>
#include <winerror.h>
#include <assert.h>
#include <d3d11.h>
#include <string>
#include "atlstr.h"