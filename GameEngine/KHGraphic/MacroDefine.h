#pragma once

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)									\
	{									\
		HRESULT hr = (x);						\
		if(FAILED(hr))							\
		{								\
			LPWSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |		\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
				NULL,						\
				hr,						\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,				\
				0,						\
				NULL);					        \
			MessageBox(NULL, output, L"Error", MB_OK);		\
		}								\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#define RESET_COM(x) { if(x != nullptr) {x.Reset();} }
#define RELEASE_COM(x) { if(x != nullptr){ x->Release(); x = nullptr; } }

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------

#define SAFE_RESET(x) { if(x != nullptr) {x->Reset();} }
#define SAFE_RELEASE(x) { if(x != nullptr){ x->Release(); delete x; x = nullptr; } }
#define SAFE_DELETE(x) { if(x != nullptr) {delete x; x = nullptr;} }

#if defined(_DEBUG) || defined(DEBUG)
inline void GRAPHIC_DEBUG_NAME(ID3D11Device * pObj, const CHAR * pstrName)
{
	if (pObj)
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
}
inline void GRAPHIC_DEBUG_NAME(ID3D11DeviceChild * pObj, const CHAR * pstrName)
{
	if (pObj)
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName);
}
#else
#define GRAPHIC_DEBUG_NAME( pObj, pstrName )
#endif