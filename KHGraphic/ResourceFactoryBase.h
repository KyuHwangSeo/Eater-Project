#pragma once
#include <string>
#include <vector>
#include <assert.h>
#include "ResourcesData.h"
#include "ParserData.h"
#include "HashBase.h"


// Template을 통해 들어오는 Class가 HashClass를 상속 받았는지 체크..
template<typename T>
using Enable_Check = typename std::enable_if<std::is_base_of<HashClass<T>, T>::value, bool>::type;

interface IShaderManager;
interface IGraphicResourceManager;

///
/// 2021/11/07 22:59
/// SeoKyuHwang
///
/// # GraphicResourceFactory Interface Class
///
/// - GraphicResource를 생성해주는 Factory Class
/// - 내부적으로 Resource Manager에서 Resource 관리
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

	template<typename T, Enable_Check<T> = NULL> void CreateDepthStencilView(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateBlendState(D3D11_BLEND_DESC* bsDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateViewPort(float topX, float topY, float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f);

public:
	template<typename T, Enable_Check<T> = NULL> void CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv);
	template<typename T, Enable_Check<T> = NULL> void CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);

	template<typename T, Enable_Check<T> = NULL> void CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav);
	template<typename T, Enable_Check<T> = NULL> void CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

private:
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

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateDepthStencilView(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::DSV);

	CreateDSV(T::GetHashCode(), tex2D, dsvDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::DSS);

	CreateDSS(T::GetHashCode(), dssDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::RS);

	CreateRS(T::GetHashCode(), rsDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateBlendState(D3D11_BLEND_DESC* bsDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::BS);

	CreateBS(T::GetHashCode(), bsDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::SS);

	CreateSS(T::GetHashCode(), ssDesc);
}

template<typename T, Enable_Check<T>>
void IGraphicResourceFactory::CreateViewPort(float topX, float topY, float width, float height, float width_ratio, float height_ratio)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::VP);

	CreateVP(T::GetHashCode(), topX, topY, width, height, width_ratio, height_ratio);
}

template<typename T, Enable_Check<T>>
void IGraphicResourceFactory::CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::RT);

	CreateBasicRT(T::GetHashCode(), rtv, srv);
}

template<typename T, Enable_Check<T>>
void IGraphicResourceFactory::CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::RT);

	CreateBasicRT(T::GetHashCode(), tex2D, rtvDesc, srvDesc);
}

template<typename T, Enable_Check<T>>
void IGraphicResourceFactory::CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::RT);

	CreateComputeRT(T::GetHashCode(), rtv, uav);
}

template<typename T, Enable_Check<T>>
void IGraphicResourceFactory::CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == eResourceType::RT);

	CreateComputeRT(T::GetHashCode(), tex2D, rtvDesc, uavDesc);
}
