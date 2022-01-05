#pragma once
#include <string>
#include <vector>
#include <assert.h>
#include "ResourcesData.h"
#include "ParserData.h"
#include "HashBase.h"

interface IShaderManager;
interface IGraphicResourceManager;

class LoadMeshData;
class TextureBuffer;

///
/// 2021/11/07 22:59
/// SeoKyuHwang
///
/// # GraphicResourceFactory Interface Class
///
/// - GraphicResource를 생성해주는 Factory Class
/// - 내부적으로 Resource Manager에서 Resource 관리
/// 
interface IGraphicResourceFactory
{
public:
	virtual void Initialize(int width, int height) abstract;
	virtual void Release() abstract;

public:
	// Create Output Graphic Resource..
	virtual void CreateMeshBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) abstract;
	virtual TextureBuffer* CreateTextureBuffer(std::string path) abstract;

public:
	// Create Graphic Struct Resource..
	template<typename T, Enable_Check<T> = NULL> void CreateDepthStencil(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc = nullptr, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr);
	template<typename T, Enable_Check<T> = NULL> void CreateRenderTarget(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc = nullptr, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr);
	template<typename T, Enable_Check<T> = NULL> void CreateRenderBuffer(D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr);
	
public:
	// Create Graphic View Resource..
	template<typename T, Enable_Check<T> = NULL> void CreateShaderResourceView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr);
	template<typename T, Enable_Check<T> = NULL> void CreateUnorderedAccessView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc = nullptr);

public:
	// Create Graphic State Resource..
	template<typename T, Enable_Check<T> = NULL> void CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateBlendState(D3D11_BLEND_DESC* bsDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc);
	template<typename T, Enable_Check<T> = NULL> void CreateViewPort(float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height);

private:
	virtual void CreateDS(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateRT(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;
	virtual void CreateRB(std::string name, Hash_Code hash_code, D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;

	virtual void CreateDSS(std::string name, Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc) abstract;
	virtual void CreateRS(std::string name, Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc) abstract;
	virtual void CreateBS(std::string name, Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc) abstract;
	virtual void CreateSS(std::string name, Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc) abstract;
	virtual void CreateVP(std::string name, Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height) abstract;

	virtual void CreateSRV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) abstract;
	virtual void CreateUAV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) abstract;

public:
	virtual IShaderManager* GetShaderManager() abstract;
	virtual IGraphicResourceManager* GetResourceManager() abstract;
};

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateDepthStencil(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::DS);

	CreateDS(T::GetName(), T::GetHashCode(), texDesc, subData, dsvDesc, srvDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateRenderTarget(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RT);

	CreateRT(T::GetName(), T::GetHashCode(), texDesc, subData, rtvDesc, srvDesc, uavDesc);
}

template<typename T, Enable_Check<T>>
void IGraphicResourceFactory::CreateRenderBuffer(D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc /*= nullptr*/, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc /*= nullptr*/)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RB);

	CreateRB(T::GetName(), T::GetHashCode(), bufferDesc, subData, srvDesc, uavDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateShaderResourceView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::SRV);

	CreateSRV(T::GetName(), T::GetHashCode(), texDesc, subData, srvDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateUnorderedAccessView(D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::UAV);

	CreateUAV(T::GetName(), T::GetHashCode(), texDesc, subData, uavDesc);
}


template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::DSS);
	
	CreateDSS(T::GetName(), T::GetHashCode(), dssDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateRasterizerState(D3D11_RASTERIZER_DESC* rsDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::RS);

	CreateRS(T::GetName(), T::GetHashCode(), rsDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateBlendState(D3D11_BLEND_DESC* bsDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::BS);

	CreateBS(T::GetName(), T::GetHashCode(), bsDesc);
}

template<typename T, Enable_Check<T>>
inline void IGraphicResourceFactory::CreateSamplerState(D3D11_SAMPLER_DESC* ssDesc)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::SS);

	CreateSS(T::GetName(), T::GetHashCode(), ssDesc);
}

template<typename T, Enable_Check<T>>
void IGraphicResourceFactory::CreateViewPort(float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height)
{
	// Template Struct Resource Type Check..
	assert(T::GetType() == RESOURCE_TYPE::VP);

	CreateVP(T::GetName(), T::GetHashCode(), ratio_offsetX, ratio_offsetY, ratio_sizeX, ratio_sizeY, width, height);
}