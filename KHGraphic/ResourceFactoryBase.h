#pragma once
#include <string>
#include <vector>
#include "ResourcesData.h"
#include "ParserData.h"
#include "HashBase.h"

///
/// 2021/11/07 22:59
/// SeoKyuHwang
///
/// GraphicResourceFactory Interface Class
///
/// - GraphicResource를 생성해주는 Factory Class
/// - 내부적으로 Resource Manager에서 Resource 관리

interface IShaderManager;
interface IGraphicResourceManager;

interface IGraphicResourceFactory
{
public:
	virtual void Initialize(int width, int height) abstract;
	virtual void Release() abstract;

public:
	// Create Output IndexBuffer Graphic Resource..
	virtual Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) abstract;
	// Create Output VertexBuffer Graphic Resource..
	virtual Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) abstract;
	// Create Output TextureBuffer Graphic Resource..
	virtual TextureBuffer* CreateTextureBuffer(std::string path) abstract;

public:
	virtual void CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, ID3D11Texture2D** tex2D) abstract;
	virtual void CreateRTV(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv) abstract;
	virtual void CreateSRV(ID3D11Texture2D* tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv) abstract;
	virtual void CreateUAV(ID3D11Texture2D* tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav) abstract;

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateDSV(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc);

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc);
	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateRS(D3D11_RASTERIZER_DESC* rsDesc);
	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateBS(D3D11_BLEND_DESC* bsDesc);
	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateSS(D3D11_SAMPLER_DESC* ssDesc);

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateViewPort(float topX, float topY, float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f);

public:
	virtual void CreateMainRenderTarget(UINT width, UINT height) abstract;

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv);
	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);

	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav);
	template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type = std::is_base_of<HashClass<T>, T>::value>
	void CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

protected:
	virtual void CreateDSV(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc) abstract;
	virtual void CreateDSS(Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
	virtual void CreateRS(Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc) abstract;
	virtual void CreateBS(Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc) abstract;
	virtual void CreateSS(Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc) abstract;
	virtual void CreateViewPort(Hash_Code hash_code, float topX, float topY, float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f) abstract;

	virtual void CreateBasicRenderTarget(Hash_Code hash_code, ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv) abstract;
	virtual void CreateBasicRenderTarget(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateComputeRenderTarget(Hash_Code hash_code, ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav) abstract;
	virtual void CreateComputeRenderTarget(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;

public:
	virtual IShaderManager* GetShaderManager() abstract;
	virtual IGraphicResourceManager* GetResourceManager() abstract;
};

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline void IGraphicResourceFactory::CreateDSV(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc)
{
	CreateDSV(T::GetHashCode(), tex2D, dsvDesc);
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline void IGraphicResourceFactory::CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	CreateDSS(T::GetHashCode(), dssDesc);
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline void IGraphicResourceFactory::CreateRS(D3D11_RASTERIZER_DESC* rsDesc)
{
	CreateRS(T::GetHashCode(), rsDesc);
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline void IGraphicResourceFactory::CreateBS(D3D11_BLEND_DESC* bsDesc)
{
	CreateBS(T::GetHashCode(), bsDesc);
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
inline void IGraphicResourceFactory::CreateSS(D3D11_SAMPLER_DESC* ssDesc)
{
	CreateSS(T::GetHashCode(), ssDesc);
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
void IGraphicResourceFactory::CreateViewPort(float topX, float topY, float width, float height, float width_ratio, float height_ratio)
{
	CreateViewPort(T::GetHashCode(), topX, topY, width, height, width_ratio, height_ratio);
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
void IGraphicResourceFactory::CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv)
{
	CreateBasicRenderTarget(T::GetHashCode(), rtv, srv);
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
void IGraphicResourceFactory::CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	CreateBasicRenderTarget(T::GetHashCode(), tex2D, rtvDesc, srvDesc);
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
void IGraphicResourceFactory::CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav)
{
	CreateComputeRenderTarget(T::GetHashCode(), rtv, uav);
}

template<typename T, typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type>
void IGraphicResourceFactory::CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	CreateComputeRenderTarget(T::GetHashCode(), tex2D, rtvDesc, uavDesc);
}
