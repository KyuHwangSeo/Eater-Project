#pragma once
#include "ResourceFactoryBase.h"

class D3D11Graphic;
class ShaderManager;
class GraphicResourceManager;
class BasicRenderTarget;
class ComputeRenderTarget;
class GraphicResourceFactory : public IGraphicResourceFactory
{
public:
	GraphicResourceFactory(D3D11Graphic* graphic);
	GraphicResourceFactory(ID3D11Device** device, ID3D11DeviceContext** context);
	~GraphicResourceFactory();

public:
	void Initialize(int width, int height) override;
	void Release() override;

	void CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, ID3D11Texture2D** tex2D);
	void CreateRTV(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv);
	void CreateSRV(ID3D11Texture2D* tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv);
	void CreateUAV(ID3D11Texture2D* tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav);
	template<typename T> void CreateDSV(ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, ID3D11DepthStencilView** dsv);

	template<typename T> Microsoft::WRL::ComPtr<ID3D11DepthStencilState> CreateDSS(D3D11_DEPTH_STENCIL_DESC* dssDesc);
	template<typename T> Microsoft::WRL::ComPtr<ID3D11RasterizerState> CreateRS(D3D11_RASTERIZER_DESC* rsDesc);
	template<typename T> Microsoft::WRL::ComPtr<ID3D11BlendState> CreateBS(D3D11_BLEND_DESC* bsDesc);
	template<typename T> Microsoft::WRL::ComPtr<ID3D11SamplerState> CreateSS(D3D11_SAMPLER_DESC* ssDesc);

	D3D11_VIEWPORT* CreateViewPort(float topX, float topY, float width, float height, float width_ratio = 1.0f, float height_ratio = 1.0f);

public:
	BasicRenderTarget* CreateMainRenderTarget(UINT width, UINT height);

	BasicRenderTarget* CreateBasicRenderTarget(ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv);
	BasicRenderTarget* CreateBasicRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);

	ComputeRenderTarget* CreateComputeRenderTarget(ID3D11RenderTargetView** rtv, ID3D11UnorderedAccessView** uav);
	ComputeRenderTarget* CreateComputeRenderTarget(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

	Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mesh) override;
	Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mesh) override;
	TextureBuffer* CreateTextureBuffer(std::string path) override;

public:
	ShaderManager* GetShaderManager();
	GraphicResourceManager* GetResourceManager();

private:
	template<typename T>
	Vertexbuffer* CreateMeshVertexBuffer(ParserData::Mesh* mesh);

	void CreateDepthStencilState();
	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();

	void CreateDepthStencilView(int width, int height);
	void CreateViewPort(int width, int height);

	void CreateQuadBuffer();
	void CreateSSAOQuadBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

	ShaderManager* m_ShaderManager;
	GraphicResourceManager* m_ResourceManager;
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