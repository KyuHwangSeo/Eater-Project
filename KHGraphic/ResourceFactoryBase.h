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

// Template을 통해 들어오는 Class가 HashClass를 상속 받았는지 체크..
template<typename T>
using Enable_Check = std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>*;

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
	virtual void CreateRenderTargetView(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv) abstract;
	virtual void CreateShaderResourceView(ID3D11Texture2D* tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv) abstract;
	virtual void CreateUnorderedAccessView(ID3D11Texture2D* tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav) abstract;

	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateDepthStencilView(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc);

	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc);
	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc);
	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateBlendState(D3D11_BLEND_DESC* bsDesc);
	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc);

	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateViewPort(float topX, float topY, float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f);

public:
	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv);
	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);

	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav);
	template<typename T, typename Enable_Check<T> = nullptr>
	void CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

protected:
	virtual void CreateDSV(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc) abstract;
	virtual void CreateDSS(Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
	virtual void CreateRS(Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc) abstract;
	virtual void CreateBS(Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc) abstract;
	virtual void CreateSS(Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc) abstract;
	virtual void CreateVP(Hash_Code hash_code, float topX, float topY, float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f) abstract;

	virtual void CreateBasicRT(Hash_Code hash_code, ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv) abstract;
	virtual void CreateBasicRT(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateComputeRT(Hash_Code hash_code, ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav) abstract;
	virtual void CreateComputeRT(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;

public:
	virtual IShaderManager* GetShaderManager() abstract;
	virtual IGraphicResourceManager* GetResourceManager() abstract;
};

template<typename T, typename Enable_Check<T>>
inline void IGraphicResourceFactory::CreateDepthStencilView(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc)
{
	CreateDSV(T::GetHashCode(), tex2D, dsvDesc);
}

template<typename T, typename Enable_Check<T>>
inline void IGraphicResourceFactory::CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	CreateDSS(T::GetHashCode(), dssDesc);
}

template<typename T, typename Enable_Check<T>>
inline void IGraphicResourceFactory::CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc)
{
	CreateRS(T::GetHashCode(), rsDesc);
}

template<typename T, typename Enable_Check<T>>
inline void IGraphicResourceFactory::CreateBlendState(D3D11_BLEND_DESC* bsDesc)
{
	CreateBS(T::GetHashCode(), bsDesc);
}

template<typename T, typename Enable_Check<T>>
inline void IGraphicResourceFactory::CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc)
{
	CreateSS(T::GetHashCode(), ssDesc);
}

template<typename T, typename Enable_Check<T>>
void IGraphicResourceFactory::CreateViewPort(float topX, float topY, float width, float height, float width_ratio, float height_ratio)
{
	CreateVP(T::GetHashCode(), topX, topY, width, height, width_ratio, height_ratio);
}

template<typename T, typename Enable_Check<T>>
void IGraphicResourceFactory::CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv)
{
	CreateBasicRT(T::GetHashCode(), rtv, srv);
}

template<typename T, typename Enable_Check<T>>
void IGraphicResourceFactory::CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	CreateBasicRT(T::GetHashCode(), tex2D, rtvDesc, srvDesc);
}

template<typename T, typename Enable_Check<T>>
void IGraphicResourceFactory::CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav)
{
	CreateComputeRT(T::GetHashCode(), rtv, uav);
}

template<typename T, typename Enable_Check<T>>
void IGraphicResourceFactory::CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	CreateComputeRT(T::GetHashCode(), tex2D, rtvDesc, uavDesc);
}
