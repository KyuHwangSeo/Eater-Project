#pragma once
#include "RenderTargetTypes.h"
#include "HashBase.h"

///
/// 2021/11/07 23:02
/// SeoKyuHwang
///
/// GraphicResourceManager Interface Class
///
/// - ��� GraphicResource�� �����ϴ� Class
/// - GraphicResourceFactory���� ������ Resource ����

typedef size_t Hash_Code;
//template<typename T>
//using TypeCheck = std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value;

class ResourceBase;
class BufferData;
class DepthStencilView;

interface IGraphicResourceManager
{
public:
	virtual void Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain) abstract;
	virtual void OnResize(int width, int height) abstract;
	virtual void Release() abstract;

public:
	friend class OriginalRenderTarget;
	friend class GraphicResourceFactory;

public:
	virtual BasicRenderTarget* GetMainRenderTarget() abstract;
	virtual void AddMainRenderTarget(RenderTarget* rtv) abstract;

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	OriginalRenderTarget GetRenderTarget();

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	DepthStencilView* GetDepthStencilView();

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	ID3D11BlendState* GetBlendState();

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	ID3D11RasterizerState* GetRasterizerState();

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	ID3D11DepthStencilState* GetDepthStencilState();

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	D3D11_VIEWPORT* GetViewPort();

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	BufferData* GetBuffer();

public:
	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void AddResource(ResourceBase* resource);

protected:
	virtual OriginalRenderTarget GetRenderTarget(Hash_Code hash_code) abstract;
	virtual DepthStencilView* GetDepthStencilView(Hash_Code hash_code) abstract;
	virtual ID3D11BlendState* GetBlendState(Hash_Code hash_code) abstract;
	virtual ID3D11RasterizerState* GetRasterizerState(Hash_Code hash_code) abstract;
	virtual ID3D11DepthStencilState* GetDepthStencilState(Hash_Code hash_code) abstract;
	virtual D3D11_VIEWPORT* GetViewPort(Hash_Code hash_code) abstract;
	virtual BufferData* GetBuffer(Hash_Code hash_code) abstract;
	virtual void AddResource(Hash_Code hash_code, ResourceBase* resource) abstract;

private:
	virtual BasicRenderTarget* GetBasicRenderTarget(Hash_Code hash_code) abstract;
	virtual ComputeRenderTarget* GetComputeRenderTarget(Hash_Code hash_code) abstract;
};

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline OriginalRenderTarget IGraphicResourceManager::GetRenderTarget()
{
	return GetRenderTarget(T::GetHashCode());
}


template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline DepthStencilView* IGraphicResourceManager::GetDepthStencilView()
{
	return GetDepthStencilView(T::GetHashCode());
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline ID3D11BlendState* IGraphicResourceManager::GetBlendState()
{
	return GetBlendState(T::GetHashCode());
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline ID3D11RasterizerState* IGraphicResourceManager::GetRasterizerState()
{
	return GetRasterizerState(T::GetHashCode());
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline ID3D11DepthStencilState* IGraphicResourceManager::GetDepthStencilState()
{
	return GetDepthStencilState(T::GetHashCode());
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline D3D11_VIEWPORT* IGraphicResourceManager::GetViewPort()
{
	return GetViewPort(T::GetHashCode());
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline BufferData* IGraphicResourceManager::GetBuffer()
{
	return GetBuffer(T::GetHashCode());
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline void IGraphicResourceManager::AddResource(ResourceBase* resource)
{
	AddResource(T::GetHashCode(), resource);
}