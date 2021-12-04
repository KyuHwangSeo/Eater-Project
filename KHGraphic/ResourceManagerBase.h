#pragma once
#include <assert.h>
#include "RenderTargetTypes.h"
#include "ResourceBase.h"
#include "HashBase.h"

typedef size_t Hash_Code;

// Template을 통해 들어오는 Class가 HashClass인지 체크..
template<typename T>
using Enable_Check = typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type;

class DepthStencilView;
class DepthStencilState;
class BlendState;
class RasterizerState;
class BufferData;
class ViewPort;

///
/// 2021/11/07 23:02
/// SeoKyuHwang
///
/// # GraphicResourceManager Interface Class
///
/// - 모든 GraphicResource를 관리하는 Class
/// - GraphicResourceFactory에서 생성된 Resource 관리
interface IGraphicResourceManager
{
public:
	virtual void Initialize() abstract;
	virtual void OnResize(int width, int height) abstract;
	virtual void Release() abstract;

public:
	friend class OriginalRenderTarget;
	friend class GraphicResourceFactory;

public:
	// BackBuffer RenderTarget Get Function
	virtual BasicRenderTarget* GetMainRenderTarget() abstract;
	// BackBuffer RenderTarget Add Function
	virtual void AddMainRenderTarget(RenderTarget* rtv) abstract;

	// RenderTarget Get Function
	template<typename T, Enable_Check<T> = NULL> OriginalRenderTarget GetRenderTarget();
	// DepthStencilView Get Function
	template<typename T, Enable_Check<T> = NULL> DepthStencilView* GetDepthStencilView();
	// DepthStencilState Get Function
	template<typename T, Enable_Check<T> = NULL> DepthStencilState* GetDepthStencilState();
	// RasterizerState Get Function
	template<typename T, Enable_Check<T> = NULL> RasterizerState* GetRasterizerState();
	// BlendState Get Function
	template<typename T, Enable_Check<T> = NULL> BlendState* GetBlendState();
	// ViewPort Get Function
	template<typename T, Enable_Check<T> = NULL> ViewPort* GetViewPort();
	// BufferData Get Function
	template<typename T, Enable_Check<T> = NULL> BufferData* GetBuffer();

public:
	// Resource Add Function
	template<typename T, Enable_Check<T> = NULL> void AddResource(ResourceBase* resource);

private:
	virtual OriginalRenderTarget GetRenderTarget(Hash_Code hash_code) abstract;
	virtual DepthStencilView* GetDepthStencilView(Hash_Code hash_code) abstract;
	virtual BlendState* GetBlendState(Hash_Code hash_code) abstract;
	virtual RasterizerState* GetRasterizerState(Hash_Code hash_code) abstract;
	virtual DepthStencilState* GetDepthStencilState(Hash_Code hash_code) abstract;
	virtual ViewPort* GetViewPort(Hash_Code hash_code) abstract;
	virtual BufferData* GetBuffer(Hash_Code hash_code) abstract;

private:
	virtual void AddResource(Hash_Code hash_code, ResourceBase* resource) abstract;

private:
	virtual BasicRenderTarget* GetBasicRenderTarget(Hash_Code hash_code) abstract;
	virtual ComputeRenderTarget* GetComputeRenderTarget(Hash_Code hash_code) abstract;
};

template<typename T, Enable_Check<T>>
inline OriginalRenderTarget IGraphicResourceManager::GetRenderTarget()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::RT);
	
	return GetRenderTarget(T::GetHashCode());
}


template<typename T, Enable_Check<T>>
inline DepthStencilView* IGraphicResourceManager::GetDepthStencilView()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::DSV);
	
	return GetDepthStencilView(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline BlendState* IGraphicResourceManager::GetBlendState()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::BS);
	
	return GetBlendState(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline RasterizerState* IGraphicResourceManager::GetRasterizerState()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::RS);
	
	return GetRasterizerState(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline DepthStencilState* IGraphicResourceManager::GetDepthStencilState()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::DSS);
	
	return GetDepthStencilState(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline ViewPort* IGraphicResourceManager::GetViewPort()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::VP);
	
	return GetViewPort(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline BufferData* IGraphicResourceManager::GetBuffer()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::BD);
	
	return GetBuffer(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceManager::AddResource(ResourceBase* resource)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == resource->GetType());
	
	AddResource(T::GetHashCode(), resource);
}