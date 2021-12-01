#include <vector>
#include "EnumDefine.h"
#include "DirectDefine.h"
#include "Texture2D.h"
#include "BufferData.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "ComputeRenderTarget.h"
#include "ResourceManagerBase.h"
#include "RenderTargetTypes.h"

OriginalRenderTarget::operator BasicRenderTarget* ()
{
	return pThis->GetBasicRenderTarget(hash_code);
}

OriginalRenderTarget::operator ComputeRenderTarget* ()
{
	return pThis->GetComputeRenderTarget(hash_code);
}
