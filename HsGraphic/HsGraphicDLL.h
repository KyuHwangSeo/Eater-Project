#pragma once

#ifdef HSGRAPHIC_INTERFACE

#define HS_GRAPHICDLL __declspec(dllexport)
#else
#define HS_GRAPHICDLL __declspec(dllimport)

	#ifdef _DEBUG
		#pragma comment(lib,"HsGraphic_x64d")
	#else
		#pragma comment(lib,"HsGraphic_x64r")
	#endif

#endif