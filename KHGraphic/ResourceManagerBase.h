#pragma once
#include "RenderTargetTypes.h"
#include "HashBase.h"

///
/// 2021/11/07 23:02
/// SeoKyuHwang
///
/// GraphicResourceManager Interface Class
///
/// - 모든 GraphicResource를 관리하는 Class
/// - GraphicResourceFactory에서 생성된 Resource 관리

typedef size_t Hash_Code;

// Template을 통해 들어오는 Class가 HashClass인지 체크..
template<typename T>
using Enable_Check = std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>*;

class ResourceBase;
class DepthStencilView;
class DepthStencilState;
class BlendState;
class RasterizerState;
class BufferData;
class ViewPort;

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
	virtual BasicRenderTarget* GetMainRenderTarget() abstract;
	virtual void AddMainRenderTarget(RenderTarget* rtv) abstract;

	template<typename T, typename Enable_Check<T> = nullptr>
	OriginalRenderTarget GetRenderTarget();

	template<typename T, typename Enable_Check<T> = nullptr>
	DepthStencilView* GetDepthStencilView();

	template<typename T, typename Enable_Check<T> = nullptr>
	DepthStencilState* GetDepthStencilState();

	template<typename T, typename Enable_Check<T> = nullptr>
	RasterizerState* GetRasterizerState();

	template<typename T, typename Enable_Check<T> = nullptr>
	BlendState* GetBlendState();

	template<typename T, typename Enable_Check<T> = nullptr>
	ViewPort* GetViewPort();

	template<typename T, typename Enable_Check<T> = nullptr>
	BufferData* GetBuffer();

public:
	template<typename T, typename Enable_Check<T> = nullptr>
	void AddResource(ResourceBase* resource);

private:
	virtual OriginalRenderTarget GetRenderTarget(Hash_Code hash_code) abstract;
	virtual DepthStencilView* GetDepthStencilView(Hash_Code hash_code) abstract;
	virtual BlendState* GetBlendState(Hash_Code hash_code) abstract;
	virtual RasterizerState* GetRasterizerState(Hash_Code hash_code) abstract;
	virtual DepthStencilState* GetDepthStencilState(Hash_Code hash_code) abstract;
	virtual ViewPort* GetViewPort(Hash_Code hash_code) abstract;
	virtual BufferData* GetBuffer(Hash_Code hash_code) abstract;
	virtual void AddResource(Hash_Code hash_code, ResourceBase* resource) abstract;

private:
	virtual BasicRenderTarget* GetBasicRenderTarget(Hash_Code hash_code) abstract;
	virtual ComputeRenderTarget* GetComputeRenderTarget(Hash_Code hash_code) abstract;
};

template<typename T, typename Enable_Check<T>>
inline OriginalRenderTarget IGraphicResourceManager::GetRenderTarget()
{
	return GetRenderTarget(T::GetHashCode());
}


template<typename T, typename Enable_Check<T>>
inline DepthStencilView* IGraphicResourceManager::GetDepthStencilView()
{
	return GetDepthStencilView(T::GetHashCode());
}

template<typename T, typename Enable_Check<T>>
inline BlendState* IGraphicResourceManager::GetBlendState()
{
	return GetBlendState(T::GetHashCode());
}

template<typename T, typename Enable_Check<T>>
inline RasterizerState* IGraphicResourceManager::GetRasterizerState()
{
	return GetRasterizerState(T::GetHashCode());
}

template<typename T, typename Enable_Check<T>>
inline DepthStencilState* IGraphicResourceManager::GetDepthStencilState()
{
	return GetDepthStencilState(T::GetHashCode());
}

template<typename T, typename Enable_Check<T>>
inline ViewPort* IGraphicResourceManager::GetViewPort()
{
	return GetViewPort(T::GetHashCode());
}

template<typename T, typename Enable_Check<T>>
inline BufferData* IGraphicResourceManager::GetBuffer()
{
	return GetBuffer(T::GetHashCode());
}

template<typename T, typename Enable_Check<T>>
inline void IGraphicResourceManager::AddResource(ResourceBase* resource)
{
	AddResource(T::GetHashCode(), resource);
}