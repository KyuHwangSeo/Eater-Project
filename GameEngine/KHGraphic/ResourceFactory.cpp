#include <vector>
#include "DirectDefine.h"
#include "D3D11GraphicBase.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "Buffer.h"
#include "RenderBuffer.h"
#include "DrawBuffer.h"
#include "Texture2D.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "ShaderTypes.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "ResourceFactory.h"

#include "EngineData.h"
#include "VertexDefine.h"
#include "SamplerStateDefine.h"
#include "BlendStateDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RasterizerStateDefine.h"
#include "DrawBufferDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "ImageParser.h"
#include "ShaderResourceHashTable.h"

using namespace DirectX::SimpleMath;

GraphicResourceFactory::GraphicResourceFactory(ID3D11Graphic* graphic, IGraphicResourceManager* resource)
	:m_Graphic(graphic), m_ResourceManager(resource)
{
	// Parser 积己 棺 檬扁拳..
	m_Parser = ImageParser::Create(IMAGE_TYPE::FLOAT_IMAGE);
	m_Parser->Initialize();
}

GraphicResourceFactory::~GraphicResourceFactory()
{

}

void GraphicResourceFactory::Initialize(int width, int height)
{
	// Back Buffer 积己..
	CreateMainRenderTarget(RT_BackBuffer::GetHashCode(), width, height);

	/// Global Resource 积己..
	CreateDepthStencilStates();
	CreateRasterizerStates();
	CreateSamplerStates();
	CreateBlendStates();

	CreateDepthStencilViews(width, height);
	CreateViewPorts(width, height);

	// FullScreen Buffer..
	CreateQuadBuffer();
	CreateSSAOQuadBuffer();

	// Debug Buffer..
	CreateLineQuadBuffer();
	CreateLineAxisBuffer();
	CreateLineCircleBuffer();
	CreateLineBoxBuffer();
}

void GraphicResourceFactory::Release()
{
	RELEASE_COM(m_Graphic);

	SAFE_RELEASE(m_ShaderManager);
	SAFE_RELEASE(m_ResourceManager);
}

void GraphicResourceFactory::CreateDS(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// Texture2D Resource 积己..
	m_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());
	
	// Bind Resource 积己..
	UINT bindFlag = texDesc->BindFlags;

	if (bindFlag & D3D11_BIND_DEPTH_STENCIL)
	{
		// DepthStencilView Resource 积己..
		m_Graphic->CreateDepthStencilView(tex2D.Get(), dsvDesc, dsv.GetAddressOf());
	}
	if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		// ShaderResourceView Resource 积己..
		m_Graphic->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf());
	}

	// Resource 积己 棺 殿废..
	Texture2D* newTex2D = new Texture2D(tex2D.Get());
	DepthStencilView* newDSV = RegisterResource<DepthStencilView, ID3D11DepthStencilView>(hash_code, dsv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_code, srv.Get());

	// DepthStencil 积己..
	DepthStencil* newResource = new DepthStencil(newTex2D, newDSV, newSRV);

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(dsv.Get(), ((std::string)name + "_DSV").c_str());
	GRAPHIC_DEBUG_NAME(srv.Get(), ((std::string)name + "_SRV").c_str());

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(dsv);
	RESET_COM(srv);
}

void GraphicResourceFactory::CreateRT(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Texture2D Resource 积己..
	m_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	// Bind Resource 积己..
	UINT bindFlag = texDesc->BindFlags;

	if (bindFlag & D3D11_BIND_RENDER_TARGET)
	{
		// RenderTargetView Resource 积己..
		m_Graphic->CreateRenderTargetView(tex2D.Get(), rtvDesc, rtv.GetAddressOf());
	}
	if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		// ShaderResourceView Resource 积己..
		m_Graphic->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf());
	}
	if (bindFlag & D3D11_BIND_UNORDERED_ACCESS)
	{
		// UnorderedAccessView Resource 积己..
		m_Graphic->CreateUnorderedAccessView(tex2D.Get(), uavDesc, uav.GetAddressOf());
	}

	// Resource 积己 棺 殿废..
	Texture2D* newTex2D = new Texture2D(tex2D.Get());
	RenderTargetView* newRTV = RegisterResource<RenderTargetView, ID3D11RenderTargetView>(hash_code, rtv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_code, srv.Get());
	UnorderedAccessView* newUAV = RegisterResource<UnorderedAccessView, ID3D11UnorderedAccessView>(hash_code, uav.Get());

	// RenderTarget 积己..
	RenderTarget* newResource = new RenderTarget(newTex2D, newRTV, newSRV, newUAV);

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(rtv.Get(), (name + "_RTV").c_str());
	GRAPHIC_DEBUG_NAME(srv.Get(), (name + "_SRV").c_str());
	GRAPHIC_DEBUG_NAME(uav.Get(), (name + "_UAV").c_str());

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(rtv);
	RESET_COM(srv);
	RESET_COM(uav);
}

void GraphicResourceFactory::CreateRB(std::string name, Hash_Code hash_code, D3D11_BUFFER_DESC* bufferDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Buffer Resource 积己..
	m_Graphic->CreateBuffer(bufferDesc, subData, buffer.GetAddressOf());

	// Bind Resource 积己..
	UINT bindFlag = bufferDesc->BindFlags;

	if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		// ShaderResourceView Resource 积己..
		m_Graphic->CreateShaderResourceView(buffer.Get(), srvDesc, srv.GetAddressOf());
	}
	if (bindFlag & D3D11_BIND_UNORDERED_ACCESS)
	{
		// UnorderedAccessView Resource 积己..
		m_Graphic->CreateUnorderedAccessView(buffer.Get(), uavDesc, uav.GetAddressOf());
	}

	// Resource 积己 棺 殿废..
	Buffer* newBuffer = new Buffer(buffer.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_code, srv.Get());
	UnorderedAccessView* newUAV = RegisterResource<UnorderedAccessView, ID3D11UnorderedAccessView>(hash_code, uav.Get());

	// RenderTarget 积己..
	RenderBuffer* newResource = new RenderBuffer(newBuffer, newSRV, newUAV);

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(buffer.Get(), (name + "_Buffer").c_str());
	GRAPHIC_DEBUG_NAME(srv.Get(), (name + "_SRV").c_str());
	GRAPHIC_DEBUG_NAME(uav.Get(), (name + "_UAV").c_str());

	// Reset Resource..
	RESET_COM(buffer);
	RESET_COM(srv);
	RESET_COM(uav);
}

void GraphicResourceFactory::CreateDSS(std::string name, Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss = nullptr;

	// DepthStencilState Resource 积己..
	m_Graphic->CreateDepthStencilState(dssDesc, dss.GetAddressOf());

	// Debug Name..
	GRAPHIC_DEBUG_NAME(dss.Get(), name.c_str());

	// DepthStencilState 积己..
	DepthStencilState* newResource = new DepthStencilState(dss.Get());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Reset Resource..
	RESET_COM(dss);
}

void GraphicResourceFactory::CreateRS(std::string name, Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rs = nullptr;

	// RasterizerState Resource 积己..
	m_Graphic->CreateRasterizerState(rsDesc, rs.GetAddressOf());

	// RasterizerState 积己..
	RasterizerState* newResource = new RasterizerState(rs.Get());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(rs.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(rs);
}

void GraphicResourceFactory::CreateBS(std::string name, Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11BlendState> bs = nullptr;

	// BlendState Resource 积己..
	m_Graphic->CreateBlendState(bsDesc, bs.GetAddressOf());

	// BlendState 积己..
	BlendState* newResource = new BlendState(bs.Get());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(bs.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(bs);
}

void GraphicResourceFactory::CreateSS(std::string name, Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11SamplerState> ss = nullptr;

	// SamplerState Resource 积己..
	m_Graphic->CreateSamplerState(ssDesc, ss.GetAddressOf());

	// SamplerState 积己..
	SamplerState* newResource = new SamplerState(ss.Get());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(ss.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(ss);
}

void GraphicResourceFactory::CreateVP(std::string name, Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height)
{
	// ViewPort 积己..
	ViewPort* newResource = new ViewPort(ratio_offsetX, ratio_offsetY, ratio_sizeX, ratio_sizeY, width, height);

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);
}

void GraphicResourceFactory::CreateSRV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// Texture2D Resource 积己..
	m_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	// Bind Resource 积己..
	UINT bindFlag = texDesc->BindFlags;

	if (bindFlag & D3D11_BIND_SHADER_RESOURCE)
	{
		// ShaderResourceView Resource 积己..
		m_Graphic->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf());
	}

	// ShaderResourceView 积己..
	ShaderResourceView* newResource = new ShaderResourceView(srv.Get());

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(srv.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(srv);
}

void GraphicResourceFactory::CreateUAV(std::string name, Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Texture2D Resource 积己..
	m_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	// Bind Resource 积己..
	UINT bindFlag = texDesc->BindFlags;

	if (bindFlag & D3D11_BIND_UNORDERED_ACCESS)
	{
		// UnorderedAccessView Resource 积己..
		m_Graphic->CreateUnorderedAccessView(tex2D.Get(), uavDesc, uav.GetAddressOf());
	}

	// UnorderedAccessView 积己..
	UnorderedAccessView* newResource = new UnorderedAccessView(uav.Get());

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(uav.Get(), name.c_str());

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(uav);
}

void GraphicResourceFactory::CreateMainRenderTarget(Hash_Code hash_Code, UINT width, UINT height)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Swap Chain, Render Target View Resize
	m_Graphic->CreateBackBuffer(width, height, tex2D.GetAddressOf(), rtv.GetAddressOf(), srv.GetAddressOf());

	// Resource 积己 棺 殿废..
	Texture2D* newTex2D = new Texture2D(tex2D.Get());
	RenderTargetView* newRTV = RegisterResource<RenderTargetView, ID3D11RenderTargetView>(hash_Code, rtv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_Code, srv.Get());
	UnorderedAccessView* newUAV = RegisterResource<UnorderedAccessView, ID3D11UnorderedAccessView>(hash_Code, uav.Get());

	// Main RenderTarget 积己..
	RenderTarget* mainRenderTarget = new RenderTarget(newTex2D, newRTV, newSRV, newUAV);

	// Resource 殿废..
	m_ResourceManager->AddMainRenderTarget(mainRenderTarget);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(rtv.Get(), "Main_RTV");
	GRAPHIC_DEBUG_NAME(srv.Get(), "Main_SRV");
	GRAPHIC_DEBUG_NAME(uav.Get(), "Main_UAV");

	// Reset Resource..
	RESET_COM(tex2D);
	RESET_COM(rtv);
	RESET_COM(srv);
}

template<>
DepthStencilView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11DepthStencilView* resource)
{
	if (resource == nullptr) return nullptr;

	// Resource 积己..
	DepthStencilView* newResource = new DepthStencilView(resource);

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	return newResource;
}

template<>
RenderTargetView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11RenderTargetView* resource)
{
	if (resource == nullptr) return nullptr;

	// Resource 积己..
	RenderTargetView* newResource = new RenderTargetView(resource);

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	return newResource;
}

template<>
ShaderResourceView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11ShaderResourceView* resource)
{
	if (resource == nullptr) return nullptr;

	// Resource 积己..
	ShaderResourceView* newResource = new ShaderResourceView(resource);

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	return newResource;
}

template<>
UnorderedAccessView* GraphicResourceFactory::RegisterResource(Hash_Code hash_code, ID3D11UnorderedAccessView* resource)
{
	if (resource == nullptr) return nullptr;

	// Resource 积己..
	UnorderedAccessView* newResource = new UnorderedAccessView(resource);

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	return newResource;
}

void GraphicResourceFactory::CreateMeshBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData)
{
	switch (meshData->MeshType)
	{
	case MESH_TYPE::STATIC_MESH:
		CreateLoadBuffer<MeshVertex>(mesh, meshData);
		break;
	case MESH_TYPE::SKIN_MESH:
		CreateLoadBuffer<SkinVertex>(mesh, meshData);
		break;
	case MESH_TYPE::TERRAIN_MESH:
		CreateLoadBuffer<TerrainVertex>(mesh, meshData);
		break;
	case MESH_TYPE::QUAD_MESH:
		CreateLoadBuffer<QuadVertex>(mesh, meshData);
		break;
	default:
		break;
	}
}

TextureBuffer* GraphicResourceFactory::CreateTextureBuffer(std::string path)
{
	TextureBuffer* texBuf = nullptr;

	ID3D11Resource* texResource = nullptr;
	ID3D11ShaderResourceView* newTex = nullptr;

	// Texture Buffer 积己..
	m_Graphic->CreateTextureBuffer(path, &texResource, &newTex);

	// Texture 积己 己傍矫 Texture Buffer 火涝..
	if (newTex)
	{
		size_t indexSlash = path.rfind("/") + 1;
		size_t indexDot = path.rfind(".");

		std::string texName = path.substr(indexSlash, path.size() - indexSlash);

		texBuf = new TextureBuffer();
		texBuf->TextureBufferPointer = newTex;

		// Debug Name..
		GRAPHIC_DEBUG_NAME(newTex, texName.c_str());

		// Reset Resource..
		texResource->Release();
	}

	return texBuf;
}

template<>
void GraphicResourceFactory::CreateLoadBuffer<MeshVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData)
{
	if (mesh->m_VertexList.empty()) return;
	

	// 货肺款 Buffer 积己..
	meshData->VB = new VertexBuffer();
	meshData->IB = new IndexBuffer();

	UINT vCount = (UINT)mesh->m_VertexList.size();
	UINT iCount = (UINT)mesh->m_IndexList.size();
	UINT vByteSize = sizeof(MeshVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount * 3;

	std::vector<MeshVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;
		vertices[i].Tex = mesh->m_VertexList[i]->m_UV;
		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;
		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;
	}

	std::vector<UINT> indices(iCount * 3);
	for (UINT i = 0; i < iCount; ++i)
	{
		indices[i * 3 + 0] = mesh->m_IndexList[i]->m_Index[0];
		indices[i * 3 + 1] = mesh->m_IndexList[i]->m_Index[1];
		indices[i * 3 + 2] = mesh->m_IndexList[i]->m_Index[2];
	}

	/// Buffer 积己..
	ID3D11Buffer* vb = nullptr;
	ID3D11Buffer* ib = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, &vb);

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, &ib);

	// 逞败拎具且 VertexBufferData 火涝..
	meshData->VB->VertexBufferPointer = vb;
	meshData->VB->VertexDataSize = sizeof(MeshVertex);

	// 逞败拎具且 IndexBufferData 火涝..
	meshData->IB->Count = iCount * 3;
	meshData->IB->IndexBufferPointer = ib;

	// Debug Name..
	GRAPHIC_DEBUG_NAME(vb, (mesh->m_NodeName + "_VB").c_str());
	GRAPHIC_DEBUG_NAME(ib, (mesh->m_NodeName + "_IB").c_str());
}

template<>
void GraphicResourceFactory::CreateLoadBuffer<SkinVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData)
{
	if (mesh->m_VertexList.empty()) return;
	
	// 货肺款 Buffer 积己..
	meshData->VB = new VertexBuffer();
	meshData->IB = new IndexBuffer();

	UINT vCount = (UINT)mesh->m_VertexList.size();
	UINT iCount = (UINT)mesh->m_IndexList.size();
	UINT vByteSize = sizeof(SkinVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount * 3;

	std::vector<SkinVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;
		vertices[i].Tex = mesh->m_VertexList[i]->m_UV;
		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;
		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;

		// Bone Weights, Bone Index..
		UINT bCount = (UINT)mesh->m_VertexList[i]->m_BoneIndices.size();
		for (UINT j = 0; j < bCount; j++)
		{
			if (j < 4)
			{
				vertices[i].BoneIndex1[j] = mesh->m_VertexList[i]->m_BoneIndices[j];
				vertices[i].BoneWeight1[j] = mesh->m_VertexList[i]->m_BoneWeights[j];
			}
			else if (j < 8)
			{
				vertices[i].BoneIndex2[j - 4] = mesh->m_VertexList[i]->m_BoneIndices[j];
				vertices[i].BoneWeight2[j - 4] = mesh->m_VertexList[i]->m_BoneWeights[j];
			}
		}
	}

	std::vector<UINT> indices(iCount * 3);
	for (UINT i = 0; i < iCount; ++i)
	{
		indices[i * 3 + 0] = mesh->m_IndexList[i]->m_Index[0];
		indices[i * 3 + 1] = mesh->m_IndexList[i]->m_Index[1];
		indices[i * 3 + 2] = mesh->m_IndexList[i]->m_Index[2];
	}

	/// Buffer 积己..
	ID3D11Buffer* vb = nullptr;
	ID3D11Buffer* ib = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, &vb);

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, &ib);

	// 逞败拎具且 VertexBufferData 火涝..
	meshData->VB->VertexBufferPointer = vb;
	meshData->VB->VertexDataSize = sizeof(SkinVertex);

	// 逞败拎具且 IndexBufferData 火涝..
	meshData->IB->Count = iCount * 3;
	meshData->IB->IndexBufferPointer = ib;

	// Debug Name..
	GRAPHIC_DEBUG_NAME(vb, (mesh->m_NodeName + "_VB").c_str());
	GRAPHIC_DEBUG_NAME(ib, (mesh->m_NodeName + "_IB").c_str());
}

template<>
void GraphicResourceFactory::CreateLoadBuffer<TerrainVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData)
{
	// 货肺款 Buffer 积己..
	meshData->VB = new VertexBuffer();
	meshData->IB = new IndexBuffer();

	UINT vCount = (UINT)mesh->m_VertexList.size();
	UINT iCount = (UINT)mesh->m_IndexList.size();
	UINT vByteSize = sizeof(TerrainVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount * 3;

	// Mask Pixel Data Parsing..
	ParserData::ImageData maskImage = m_Parser->LoadImagePixel(meshData->Mask_Name.c_str(), 4);

	std::vector<TerrainVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;

		vertices[i].Tex.x = vertices[i].Pos.x;
		vertices[i].Tex.y = vertices[i].Pos.z;

		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;
		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;
		 
		// 秦寸 Pixel Mask Color..
		vertices[i].Mask = m_Parser->GetPixelColor(maskImage, abs(vertices[i].Pos.x), abs(vertices[i].Pos.z));
	}

	std::vector<UINT> indices(iCount * 3);
	for (UINT i = 0; i < iCount; ++i)
	{
		indices[i * 3 + 0] = mesh->m_IndexList[i]->m_Index[0];
		indices[i * 3 + 1] = mesh->m_IndexList[i]->m_Index[1];
		indices[i * 3 + 2] = mesh->m_IndexList[i]->m_Index[2];
	}

	/// Buffer 积己..
	ID3D11Buffer* vb = nullptr;
	ID3D11Buffer* ib = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, &vb);

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, &ib);

	// 逞败拎具且 VertexBufferData 火涝..
	meshData->VB->VertexBufferPointer = vb;
	meshData->VB->VertexDataSize = sizeof(TerrainVertex);

	// 逞败拎具且 IndexBufferData 火涝..
	meshData->IB->Count = iCount * 3;
	meshData->IB->IndexBufferPointer = ib;

	// Debug Name..
	GRAPHIC_DEBUG_NAME(vb, (mesh->m_NodeName + "_VB").c_str());
	GRAPHIC_DEBUG_NAME(ib, (mesh->m_NodeName + "_IB").c_str());
}

template<>
void GraphicResourceFactory::CreateLoadBuffer<QuadVertex>(ParserData::Mesh* mesh, LoadMeshData* meshData)
{
	// 货肺款 Buffer 积己..
	meshData->VB = new VertexBuffer();
	meshData->IB = new IndexBuffer();

	// Quad Buffer..
	DrawBuffer* quadBuf = m_ResourceManager->GetDrawBuffer<DB_Quad>();

	// 逞败拎具且 VertexBufferData 火涝..
	meshData->VB->VertexBufferPointer = quadBuf->VB->Get();
	meshData->VB->VertexDataSize = quadBuf->Stride;

	// 逞败拎具且 IndexBufferData 火涝..
	meshData->IB->Count = quadBuf->IndexCount;
	meshData->IB->IndexBufferPointer = quadBuf->IB->Get();
}

IShaderManager* GraphicResourceFactory::GetShaderManager()
{
	return m_ShaderManager;
}

IGraphicResourceManager* GraphicResourceFactory::GetResourceManager()
{
	return m_ResourceManager;
}

void GraphicResourceFactory::CreateDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Defalt DepthStencilState 积己..
	CreateDepthStencilState<DSS_Defalt>(&depthStencilDesc);

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// NoDepth DepthStencilState 积己..
	CreateDepthStencilState<DSS_NoDepth>(&depthStencilDesc);

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	CreateDepthStencilState<DSS_NoDepthStencil>(&depthStencilDesc);
}

void GraphicResourceFactory::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.MultisampleEnable = true;
	rasterizerDesc.ScissorEnable = false;

	// Solid RasterizerState 积己..
	CreateRasterizerState<RS_Solid>(&rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.AntialiasedLineEnable = true;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;

	// WireFrame RasterizerState 积己..
	CreateRasterizerState<RS_WireFrame>(&rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	// NoCull RasterizerState 积己..
	CreateRasterizerState<RS_NoCull>(&rasterizerDesc);

	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.DepthBias = 100000;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.005f;

	// Depth RasterizerState 积己..
	CreateRasterizerState<RS_Depth>(&rasterizerDesc);
}

void GraphicResourceFactory::CreateSamplerStates()
{
	D3D11_SAMPLER_DESC samplerDesc;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samWrapMinLinear SamplerState 积己..
	CreateSamplerState<gSamWrapLinear>(&samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samWrapLinerPoint SamplerState 积己..
	CreateSamplerState<gSamWrapLinerPoint>(&samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samWrapAnisotropic SamplerState 积己..
	CreateSamplerState<gSamWrapAnisotropic>(&samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samClampMinLinear SamplerState 积己..
	CreateSamplerState<gSamClampLinear>(&samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samClampMinLinearPoint SamplerState 积己..
	CreateSamplerState<gSamClampLinearPoint>(&samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1e5f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samBorderLinerPoint SamplerState 积己..
	CreateSamplerState<gSamBorderLinearPoint>(&samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// gShadowSam SamplerState 积己..
	CreateSamplerState<gSamBorderComparisonLinearPoint>(&samplerDesc);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// samMirrorMinLinear SamplerState 积己..
	CreateSamplerState<gSamMirrorLinear>(&samplerDesc);
}

void GraphicResourceFactory::CreateBlendStates()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[2].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[3].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[4].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	// Blending First RenderTarget BlendState 积己..
	CreateBlendState<BS_AlphaBlend>(&blendDesc);

	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Blending First RenderTarget BlendState 积己..
	CreateBlendState<BS_Defalt>(&blendDesc);
}

void GraphicResourceFactory::CreateDepthStencilViews(int width, int height)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	//texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Flags = 0;
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Defalt DepthStencilView 积己..
	CreateDepthStencil<DS_Defalt>(&texDesc, nullptr, &descDSV);
	CreateDepthStencil<DS_Light>(&texDesc, nullptr, &descDSV);
}

void GraphicResourceFactory::CreateViewPorts(int width, int height)
{
	// Defalt ViewPort 积己..
	CreateViewPort<VP_FullScreen>(0.0f, 0.0f, 1.0f, 1.0f, (float)width, (float)height);
}

void GraphicResourceFactory::CreateQuadBuffer()
{
	UINT vCount = 4;
	UINT iCount = 6;
	UINT vByteSize = sizeof(QuadVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<QuadVertex> vertices(vCount);
	vertices[0].Pos = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1].Pos = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[2].Pos = Vector3(+1.0f, +1.0f, 0.0f);
	vertices[3].Pos = Vector3(+1.0f, -1.0f, 0.0f);

	vertices[0].Tex = Vector2(0.0f, 1.0f);
	vertices[1].Tex = Vector2(0.0f, 0.0f);
	vertices[2].Tex = Vector2(1.0f, 0.0f);
	vertices[3].Tex = Vector2(1.0f, 1.0f);

	std::vector<UINT> indices(iCount);
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	/// Buffer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, VB.GetAddressOf());

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, IB.GetAddressOf());

	// Resource Buffer 积己..
	Buffer* newVB = new Buffer(VB.Get());
	Buffer* newIB = new Buffer(IB.Get());

	// Resource Draw Buffer 积己..
	DrawBuffer* newBuf = new DrawBuffer(newVB, newIB);

	// Buffer Data 火涝..
	newBuf->Stride = sizeof(QuadVertex);
	newBuf->IndexCount = iCount;

	// Resource 殿废..
	m_ResourceManager->AddResource<DB_Quad>(newBuf);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(newBuf->VB->Get(), "ScreenQuad_VB");
	GRAPHIC_DEBUG_NAME(newBuf->IB->Get(), "ScreenQuad_IB");

	RESET_COM(VB);
	RESET_COM(IB);
}

void GraphicResourceFactory::CreateSSAOQuadBuffer()
{
	UINT vCount = 4;
	UINT iCount = 6;
	UINT vByteSize = sizeof(PosNormalTexVertex) * vCount;
	UINT iByteSize = sizeof(USHORT) * iCount;

	std::vector<PosNormalTexVertex> vertices(vCount);
	vertices[0].Pos = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1].Pos = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[2].Pos = Vector3(+1.0f, +1.0f, 0.0f);
	vertices[3].Pos = Vector3(+1.0f, -1.0f, 0.0f);

	// Store far plane frustum corner indices in Normal.x slot.
	vertices[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
	vertices[1].Normal = Vector3(1.0f, 0.0f, 0.0f);
	vertices[2].Normal = Vector3(2.0f, 0.0f, 0.0f);
	vertices[3].Normal = Vector3(3.0f, 0.0f, 0.0f);

	vertices[0].Tex = Vector2(0.0f, 1.0f);
	vertices[1].Tex = Vector2(0.0f, 0.0f);
	vertices[2].Tex = Vector2(1.0f, 0.0f);
	vertices[3].Tex = Vector2(1.0f, 1.0f);

	std::vector<USHORT> indices(iCount);
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	/// Buffer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, VB.GetAddressOf());

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, IB.GetAddressOf());

	// Resource Buffer 积己..
	Buffer* newVB = new Buffer(VB.Get());
	Buffer* newIB = new Buffer(IB.Get());

	// Resource Draw Buffer 积己..
	DrawBuffer* newBuf = new DrawBuffer(newVB, newIB);

	// Buffer Data 火涝..
	newBuf->Stride = sizeof(PosNormalTexVertex);
	newBuf->IndexCount = iCount;

	// Resource 殿废..
	m_ResourceManager->AddResource<DB_SSAO>(newBuf);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(newBuf->VB->Get(), "SSAOQuad_VB");
	GRAPHIC_DEBUG_NAME(newBuf->IB->Get(), "SSAOQuad_IB");

	RESET_COM(VB);
	RESET_COM(IB);
}

void GraphicResourceFactory::CreateLineQuadBuffer()
{
	UINT vCount = 4;
	UINT iCount = 10;
	UINT vByteSize = sizeof(PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<PosColorVertex> vertices(vCount);
	vertices[0].Pos = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1].Pos = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[2].Pos = Vector3(+1.0f, +1.0f, 0.0f);
	vertices[3].Pos = Vector3(+1.0f, -1.0f, 0.0f);
	vertices[0].Color = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
	vertices[1].Color = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
	vertices[2].Color = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
	vertices[3].Color = Vector4(0.2f, 0.2f, 0.2f, 1.0f);

	std::vector<UINT> indices(iCount);
	indices[0] = 0; indices[1] = 1;
	indices[2] = 1; indices[3] = 2;
	indices[4] = 2; indices[5] = 0;
	indices[6] = 0; indices[7] = 3;
	indices[8] = 3; indices[9] = 2;

	/// Buffer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, VB.GetAddressOf());

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, IB.GetAddressOf());

	// Resource Buffer 积己..
	Buffer* newVB = new Buffer(VB.Get());
	Buffer* newIB = new Buffer(IB.Get());

	// Resource Draw Buffer 积己..
	DrawBuffer* newBuf = new DrawBuffer(newVB, newIB);

	// Buffer Data 火涝..
	newBuf->Stride = sizeof(PosColorVertex);
	newBuf->IndexCount = iCount;

	// Resource 殿废..
	m_ResourceManager->AddResource<DB_Line_Quad>(newBuf);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(newBuf->VB->Get(), "DebugQuad_VB");
	GRAPHIC_DEBUG_NAME(newBuf->IB->Get(), "DebugQuad_IB");

	RESET_COM(VB);
	RESET_COM(IB);
}

void GraphicResourceFactory::CreateLineAxisBuffer()
{
	UINT vCount = 6;
	UINT iCount = 6;
	UINT vByteSize = sizeof(PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	float extents = 0.1f;

	std::vector<PosColorVertex> vertices(vCount);
	vertices[0].Pos = Vector3(0, 0, 0);
	vertices[1].Pos = Vector3(0, 0, 0);
	vertices[2].Pos = Vector3(0, 0, 0);
	vertices[3].Pos = Vector3(extents, 0, 0);
	vertices[4].Pos = Vector3(0, extents, 0);
	vertices[5].Pos = Vector3(0, 0, extents);
	vertices[0].Color = Vector4(1, 0, 0, 1);
	vertices[1].Color = Vector4(0, 1, 0, 1);
	vertices[2].Color = Vector4(0, 0, 1, 1);
	vertices[3].Color = Vector4(1, 0, 0, 1);
	vertices[4].Color = Vector4(0, 1, 0, 1);
	vertices[5].Color = Vector4(0, 0, 1, 1);

	std::vector<UINT> indices(iCount);
	indices[0] = 0; indices[1] = 3;
	indices[2] = 1; indices[3] = 4;
	indices[4] = 2; indices[5] = 5;

	/// Buffer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, VB.GetAddressOf());

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, IB.GetAddressOf());

	// Resource Buffer 积己..
	Buffer* newVB = new Buffer(VB.Get());
	Buffer* newIB = new Buffer(IB.Get());

	// Resource Draw Buffer 积己..
	DrawBuffer* newBuf = new DrawBuffer(newVB, newIB);

	// Buffer Data 火涝..
	newBuf->Stride = sizeof(PosColorVertex);
	newBuf->IndexCount = iCount;

	// Resource 殿废..
	m_ResourceManager->AddResource<DB_Line_Axis>(newBuf);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(newBuf->VB->Get(), "DebugAxis_VB");
	GRAPHIC_DEBUG_NAME(newBuf->IB->Get(), "DebugAxis_IB");

	RESET_COM(VB);
	RESET_COM(IB);
}

void GraphicResourceFactory::CreateLineBoxBuffer()
{
	UINT vCount = 8;
	UINT iCount = 24;
	UINT vByteSize = sizeof(PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	float extents = 1.0f;

	std::vector<PosColorVertex> vertices(vCount);
	vertices[0].Pos = Vector3(-extents, -extents, -extents);
	vertices[1].Pos = Vector3(-extents, -extents, +extents);
	vertices[2].Pos = Vector3(+extents, -extents, +extents);
	vertices[3].Pos = Vector3(+extents, -extents, -extents);
	vertices[4].Pos = Vector3(-extents, +extents, -extents);
	vertices[5].Pos = Vector3(-extents, +extents, +extents);
	vertices[6].Pos = Vector3(+extents, +extents, +extents);
	vertices[7].Pos = Vector3(+extents, +extents, -extents);
	vertices[0].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[1].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[2].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[3].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[4].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[5].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[6].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	vertices[7].Color = DirectX::SimpleMath::Vector4(0, 1, 0, 1);
	
	std::vector<UINT> indices(iCount);
	indices[0]  = 0; indices[1]  = 1; indices[2]  = 1; indices[3]  = 2;
	indices[4]  = 2; indices[5]  = 3; indices[6]  = 3; indices[7]  = 0;
	indices[8]  = 4; indices[9]  = 5; indices[10] = 5; indices[11] = 6;
	indices[12] = 6; indices[13] = 7; indices[14] = 7; indices[15] = 4;
	indices[16] = 4; indices[17] = 0; indices[18] = 5; indices[19] = 1;
	indices[20] = 6; indices[21] = 2; indices[22] = 7; indices[23] = 3;

	/// Buffer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, VB.GetAddressOf());

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, IB.GetAddressOf());

	// Resource Buffer 积己..
	Buffer* newVB = new Buffer(VB.Get());
	Buffer* newIB = new Buffer(IB.Get());

	// Resource Draw Buffer 积己..
	DrawBuffer* newBuf = new DrawBuffer(newVB, newIB);

	// Buffer Data 火涝..
	newBuf->Stride = sizeof(PosColorVertex);
	newBuf->IndexCount = iCount;

	// Resource 殿废..
	m_ResourceManager->AddResource<DB_Line_Box>(newBuf);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(newBuf->VB->Get(), "DebugBox_VB");
	GRAPHIC_DEBUG_NAME(newBuf->IB->Get(), "DebugBox_IB");

	RESET_COM(VB);
	RESET_COM(IB);
}

void GraphicResourceFactory::CreateLineCircleBuffer()
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> vb = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ib = nullptr;
	
	UINT vCount = 540;
	UINT iCount = 1080;
	UINT vByteSize = sizeof(PosColorVertex) * vCount;
	UINT iByteSize = sizeof(UINT) * iCount;

	std::vector<PosColorVertex> vertices(vCount);
	for (int i = 0; i < 180; i++)
	{
		float angle = (float)(i * 2);
		float x = cosf(angle * 3.14f / 180.0f);
		float z = -sinf(angle * 3.14f / 180.0f);

		vertices[i].Pos = Vector3(x, 0, z);
		vertices[i].Color = Vector4(0, 1, 0, 1);
	}

	for (int i = 0; i < 180; i++)
	{
		float angle = (float)(i * 2);
		float x = cosf(angle * 3.14f / 180.0f);
		float y = -sinf(angle * 3.14f / 180.0f);

		vertices[180 + i].Pos = Vector3(x, y, 0);
		vertices[180 + i].Color = Vector4(0, 0, 1, 1);
	}

	for (int i = 0; i < 180; i++)
	{
		float angle = (float)(i * 2);
		float y = cosf(angle * 3.14f / 180.0f);
		float z = -sinf(angle * 3.14f / 180.0f);

		vertices[360 + i].Pos = Vector3(0, y, z);
		vertices[360 + i].Color = Vector4(1, 0, 0, 1);
	}

	std::vector<UINT> indices(iCount);
	int index = 0;
	for (int i = 0; i < 540; i++)
	{
		indices[index] = i;
		indices[index + 1] = i+1;
		index += 2;
	}
	indices[359] = 0;
	indices[719] = 180;
	indices[1079] = 360;

	/// Buffer 积己..
	Microsoft::WRL::ComPtr<ID3D11Buffer> VB = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IB = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA initData;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = vByteSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, VB.GetAddressOf());

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = iByteSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	initData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&bufferDesc, &initData, IB.GetAddressOf());

	// Resource Buffer 积己..
	Buffer* newVB = new Buffer(VB.Get());
	Buffer* newIB = new Buffer(IB.Get());

	// Resource Draw Buffer 积己..
	DrawBuffer* newBuf = new DrawBuffer(newVB, newIB);

	// Buffer Data 火涝..
	newBuf->Stride = sizeof(PosColorVertex);
	newBuf->IndexCount = iCount;

	// Resource 殿废..
	m_ResourceManager->AddResource<DB_Line_Circle>(newBuf);

	// Debug Name..
	GRAPHIC_DEBUG_NAME(newBuf->VB->Get(), "DebugCircle_VB");
	GRAPHIC_DEBUG_NAME(newBuf->IB->Get(), "DebugCircle_IB");

	RESET_COM(VB);
	RESET_COM(IB);
}
