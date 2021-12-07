#pragma once
#include "ResourceFactoryBase.h"
#include "HashBase.h"

class D3D11Graphic;
class BasicRenderTarget;
class ComputeRenderTarget;

class GraphicResourceFactory : public IGraphicResourceFactory
{
public:
	GraphicResourceFactory(D3D11Graphic* graphic, IGraphicResourceManager* resource);
	~GraphicResourceFactory();

public:
	void Initialize(int width, int height) override;
	void Release() override;

	void CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, ID3D11Texture2D** tex2D) override;
	void CreateRenderTargetView(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv) override;
	void CreateShaderResourceView(ID3D11Texture2D* tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv) override;
	void CreateUnorderedAccessView(ID3D11Texture2D* tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav) override;

private:
	void CreateDSS(Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc) override;
	void CreateRS(Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc) override;
	void CreateBS(Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc) override;
	void CreateSS(Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc) override;
	void CreateVP(Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height) override;

	void CreateDSV(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;

	void CreateBasicRT(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc) override;
	void CreateComputeRT(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc) override;

public:
	Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) override;
	Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) override;
	TextureBuffer* CreateTextureBuffer(std::string path) override;

public:
	IShaderManager* GetShaderManager();
	IGraphicResourceManager* GetResourceManager();

private:
	template<typename T>
	Vertexbuffer* CreateMeshVertexBuffer(ParserData::Mesh* mesh);

	void CreateMainRenderTarget(UINT width, UINT height);

	void CreateDepthStencilStates();
	void CreateRasterizerStates();
	void CreateSamplerStates();
	void CreateBlendStates();

	void CreateDepthStencilViews(int width, int height);
	void CreateViewPorts(int width, int height);

	void CreateQuadBuffer();
	void CreateSSAOQuadBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	IShaderManager* m_ShaderManager;
	IGraphicResourceManager* m_ResourceManager;
};

struct MeshVertex;
struct SkinVertex;
struct TerrainVertex;

template<typename T>
inline Vertexbuffer* GraphicResourceFactory::CreateMeshVertexBuffer(ParserData::Mesh* mesh) { return nullptr; }

template<>
inline Vertexbuffer* GraphicResourceFactory::CreateMeshVertexBuffer<MeshVertex>(ParserData::Mesh* mesh);

template<>
inline Vertexbuffer* GraphicResourceFactory::CreateMeshVertexBuffer<SkinVertex>(ParserData::Mesh* mesh);

template<>
inline Vertexbuffer* GraphicResourceFactory::CreateMeshVertexBuffer<TerrainVertex>(ParserData::Mesh* mesh);