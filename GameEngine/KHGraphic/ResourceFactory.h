#pragma once
#include "ResourceFactoryBase.h"
#include "HashBase.h"

interface ID3D11Graphic;
class BasicRenderTarget;
class ComputeRenderTarget;
class ImageParser;

class GraphicResourceFactory : public IGraphicResourceFactory
{
public:
	GraphicResourceFactory(ID3D11Graphic* graphic, IGraphicResourceManager* resource);
	~GraphicResourceFactory();

public:
	void Initialize(int width, int height) override;
	void Release() override;

private:
	void CreateDS(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	void CreateRT(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;
	void CreateRB(std::string name, Hash_Code hash_code, D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;

	void CreateDSS(std::string name, Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc) override;
	void CreateRS(std::string name, Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc) override;
	void CreateBS(std::string name, Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc) override;
	void CreateSS(std::string name, Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc) override;
	void CreateVP(std::string name, Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height) override;

	void CreateSRV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	void CreateUAV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;

private:
	template<typename ResourceClass, typename Resource>
	ResourceClass* RegisterResource(Hash_Code hash_code, Resource* resource);

public:
	void CreateMeshBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) override;
	TextureBuffer* CreateTextureBuffer(std::string path) override;

public:
	IShaderManager* GetShaderManager();
	IGraphicResourceManager* GetResourceManager();

private:
	template<typename T>
	void CreateLoadBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData);

	void CreateMainRenderTarget(Hash_Code hash_Code, UINT width, UINT height);

	void CreateDepthStencilStates();
	void CreateRasterizerStates();
	void CreateSamplerStates();
	void CreateBlendStates();
	
	void CreateDepthStencilViews(int width, int height);
	void CreateViewPorts(int width, int height);

	void CreateQuadBuffer();
	void CreateSSAOQuadBuffer();

	void CreateLineQuadBuffer();
	void CreateLineAxisBuffer();
	void CreateLineBoxBuffer();
	void CreateLineCircleBuffer();

private:
	ID3D11Graphic* m_Graphic;
	ImageParser* m_Parser;
	IShaderManager* m_ShaderManager;
	IGraphicResourceManager* m_ResourceManager;
};

template<typename ResourceClass, typename Resource>
inline ResourceClass* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, Resource* resource) { return nullptr; }

template<>
inline DepthStencilView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11DepthStencilView* resource);

template<>
inline RenderTargetView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11RenderTargetView* resource);

template<>
inline ShaderResourceView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11ShaderResourceView* resource);

template<>
inline UnorderedAccessView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11UnorderedAccessView* resource);

struct MeshVertex;
struct SkinVertex;
struct TerrainVertex;
struct QuadVertex;

template<typename T>
void GraphicResourceFactory::CreateLoadBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) {}

template<>
void GraphicResourceFactory::CreateLoadBuffer<MeshVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData);

template<>
void GraphicResourceFactory::CreateLoadBuffer<SkinVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData);

template<>
void GraphicResourceFactory::CreateLoadBuffer<TerrainVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData);

template<>
void GraphicResourceFactory::CreateLoadBuffer<QuadVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData);
