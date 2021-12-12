#pragma once
#include <assert.h>
#include "ResourceBase.h"
#include "HashBase.h"

typedef size_t Hash_Code;

class RenderTarget;
class DepthStencil;

class RenderTargetView;
class DepthStencilView;
class ShaderResourceView;
class UnorderedAccessView;

class DepthStencilState;
class BlendState;
class RasterizerState;
class ViewPort;
class BufferData;

///
/// 2021/11/07 23:02
/// SeoKyuHwang
///
/// # GraphicResourceManager Interface Class
///
/// - 葛电 GraphicResource甫 包府窍绰 Class
/// - GraphicResourceFactory俊辑 积己等 Resource 包府
/// 
interface IGraphicResourceManager
{
public:
	virtual void Initialize() abstract;
	virtual void OnResize(int width, int height) abstract;
	virtual void Release() abstract;

public:
	friend class GraphicResourceFactory;

public:
	// BackBuffer RenderTarget Get Function
	virtual RenderTarget* GetMainRenderTarget() abstract;
	// BackBuffer RenderTarget Add Function
	virtual void AddMainRenderTarget(RenderTarget* rtv) abstract;

public:
	// RenderTarget Get Function
	template<typename T, Enable_Check<T> = NULL> RenderTarget* GetRenderTarget();
	// DepthStencil Get Function
	template<typename T, Enable_Check<T> = NULL> DepthStencil* GetDepthStencil();

public:
	// RenderTargetView Get Function
	template<typename T, Enable_Check<T> = NULL> RenderTargetView* GetRenderTargetView();
	// DepthStencilView Get Function
	template<typename T, Enable_Check<T> = NULL> DepthStencilView* GetDepthStencilView();
	// ShaderResourceView Get Function
	template<typename T, Enable_Check<T> = NULL> ShaderResourceView* GetShaderResourceView();
	// UnorderedAccessView Get Function
	template<typename T, Enable_Check<T> = NULL> UnorderedAccessView* GetUnorderedAccessView();

public:
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
	// Graphic View Resource Getter..
	virtual RenderTarget* GetRenderTarget(Hash_Code hash_code) abstract;
	virtual DepthStencil* GetDepthStencil(Hash_Code hash_code) abstract;
	virtual RenderTargetView* GetRenderTargetView(Hash_Code hash_code) abstract;
	virtual DepthStencilView* GetDepthStencilView(Hash_Code hash_code) abstract;
	virtual ShaderResourceView* GetShaderResourceView(Hash_Code hash_code) abstract;
	virtual UnorderedAccessView* GetUnorderedAccessView(Hash_Code hash_code) abstract;

private:
	// Graphic State Resource Getter..
	virtual BlendState* GetBlendState(Hash_Code hash_code) abstract;
	virtual RasterizerState* GetRasterizerState(Hash_Code hash_code) abstract;
	virtual DepthStencilState* GetDepthStencilState(Hash_Code hash_code) abstract;
	virtual ViewPort* GetViewPort(Hash_Code hash_code) abstract;
	virtual BufferData* GetBuffer(Hash_Code hash_code) abstract;

private:
	virtual void AddResource(Hash_Code hash_code, ResourceBase* resource) abstract;
};

template<typename T, Enable_Check<T>>
inline RenderTarget* IGraphicResourceManager::GetRenderTarget()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::RT);
	
	return GetRenderTarget(T::GetHashCode());
}


template<typename T, Enable_Check<T>>
inline DepthStencil* IGraphicResourceManager::GetDepthStencil()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::DS);
	
	return GetDepthStencil(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline DepthStencilView* IGraphicResourceManager::GetDepthStencilView()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::DSV || T::GetType() == eResourceType::DS);

	return GetDepthStencilView(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline RenderTargetView* IGraphicResourceManager::GetRenderTargetView()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::RTV || T::GetType() == eResourceType::RT);
	
	return GetRenderTargetView(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline ShaderResourceView* IGraphicResourceManager::GetShaderResourceView()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::SRV || T::GetType() == eResourceType::RT || T::GetType() == eResourceType::DS);

	return GetShaderResourceView(T::GetHashCode());
}

template<typename T, Enable_Check<T>>
inline UnorderedAccessView* IGraphicResourceManager::GetUnorderedAccessView()
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::UAV || T::GetType() == eResourceType::RT);

	return GetUnorderedAccessView(T::GetHashCode());
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