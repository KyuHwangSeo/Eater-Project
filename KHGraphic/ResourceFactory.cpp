#include <vector>
#include "DirectDefine.h"
#include "D3D11GraphicBase.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "BufferData.h"
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

#include "VertexDefine.h"
#include "SamplerBufferDefine.h"
#include "BlendStateDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RasterizerStateDefine.h"
#include "BufferDataDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "ImageParser.h"

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
}

void GraphicResourceFactory::Release()
{
	RELEASE_COM(m_Graphic);

	SAFE_RELEASE(m_ShaderManager);
	SAFE_RELEASE(m_ResourceManager);
}

void GraphicResourceFactory::CreateDSS(Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss = nullptr;

	// DepthStencilState Resource 积己..
	m_Graphic->CreateDepthStencilState(dssDesc, dss.GetAddressOf());

	// DepthStencilState 积己..
	DepthStencilState* newResource = new DepthStencilState(dss.Get());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Reset Pointer..
	RESET_COM(dss);
}

void GraphicResourceFactory::CreateRS(Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rs = nullptr;

	// RasterizerState Resource 积己..
	m_Graphic->CreateRasterizerState(rsDesc, rs.GetAddressOf());

	// RasterizerState 积己..
	RasterizerState* newResource = new RasterizerState(rs.Get());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Reset Pointer..
	RESET_COM(rs);
}

void GraphicResourceFactory::CreateBS(Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11BlendState> bs = nullptr;

	// BlendState Resource 积己..
	m_Graphic->CreateBlendState(bsDesc, bs.GetAddressOf());

	// BlendState 积己..
	BlendState* newResource = new BlendState(bs.Get());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Reset Pointer..
	RESET_COM(bs);
}

void GraphicResourceFactory::CreateSS(Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11SamplerState> ss = nullptr;

	// SamplerState Resource 积己..
	m_Graphic->CreateSamplerState(ssDesc, ss.GetAddressOf());

	// SamplerState 积己..
	SamplerState* newResource = new SamplerState(ss.Get());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Reset Pointer..
	RESET_COM(ss);
}

void GraphicResourceFactory::CreateVP(Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height)
{
	// ViewPort 积己..
	ViewPort* newResource = new ViewPort(ratio_offsetX, ratio_offsetY, ratio_sizeX, ratio_sizeY, width, height);

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);
}

void GraphicResourceFactory::CreateDS(Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// Texture2D Resource 积己..
	m_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	// DepthStencilView Resource 积己..
	m_Graphic->CreateDepthStencilView(tex2D.Get(), dsvDesc, dsv.GetAddressOf());

	// ShaderResourceView Resource 积己..
	m_Graphic->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf());

	// Resource 积己 棺 殿废..
	DepthStencilView* newDSV = RegisterResource<DepthStencilView, ID3D11DepthStencilView>(hash_code, dsv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_code, srv.Get());

	// DepthStencil 积己..
	DepthStencil* newResource = new DepthStencil(tex2D.Get(), newDSV, newSRV);

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Reset Pointer..
	RESET_COM(tex2D);
	RESET_COM(dsv);
	RESET_COM(srv);
}

void GraphicResourceFactory::CreateRT(Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Texture2D Resource 积己..
	m_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	// RenderTargetView Resource 积己..
	m_Graphic->CreateRenderTargetView(tex2D.Get(), rtvDesc, rtv.GetAddressOf());

	// ShaderResourceView Resource 积己..
	m_Graphic->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf());

	// UnorderedAccessView Resource 积己..
	m_Graphic->CreateUnorderedAccessView(tex2D.Get(), uavDesc, uav.GetAddressOf());

	// Resource 积己 棺 殿废..
	RenderTargetView* newRTV = RegisterResource<RenderTargetView, ID3D11RenderTargetView>(hash_code, rtv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_code, srv.Get());
	UnorderedAccessView* newUAV = RegisterResource<UnorderedAccessView, ID3D11UnorderedAccessView>(hash_code, uav.Get());

	// RenderTarget 积己..
	RenderTarget* newResource = new RenderTarget(tex2D.Get(), newRTV, newSRV, newUAV);

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Reset Pointer..
	RESET_COM(tex2D);
	RESET_COM(rtv);
	RESET_COM(srv);
	RESET_COM(uav);
}

void GraphicResourceFactory::CreateSRV(Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// Texture2D Resource 积己..
	m_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	// ShaderResourceView Resource 积己..
	m_Graphic->CreateShaderResourceView(tex2D.Get(), srvDesc, srv.GetAddressOf());

	// ShaderResourceView 积己..
	ShaderResourceView* newResource = new ShaderResourceView(srv.Get());

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Reset Pointer..
	RESET_COM(tex2D);
	RESET_COM(srv);
}

void GraphicResourceFactory::CreateUAV(Hash_Code hash_code, D3D11_TEXTURE2D_DESC* texDesc, D3D11_SUBRESOURCE_DATA* subData, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	// Texture2D Resource 积己..
	m_Graphic->CreateTexture2D(texDesc, subData, tex2D.GetAddressOf());

	// UnorderedAccessView Resource 积己..
	m_Graphic->CreateUnorderedAccessView(tex2D.Get(), uavDesc, uav.GetAddressOf());

	// UnorderedAccessView 积己..
	UnorderedAccessView* newResource = new UnorderedAccessView(uav.Get());

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, newResource);

	// Reset Pointer..
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
	RenderTargetView* newRTV = RegisterResource<RenderTargetView, ID3D11RenderTargetView>(hash_Code, rtv.Get());
	ShaderResourceView* newSRV = RegisterResource<ShaderResourceView, ID3D11ShaderResourceView>(hash_Code, srv.Get());
	UnorderedAccessView* newUAV = RegisterResource<UnorderedAccessView, ID3D11UnorderedAccessView>(hash_Code, uav.Get());

	// Main RenderTarget 积己..
	RenderTarget* mainRenderTarget = new RenderTarget(tex2D.Get(), newRTV, newSRV, newUAV);

	// Resource 殿废..
	m_ResourceManager->AddMainRenderTarget(mainRenderTarget);

	// Reset Pointer..
	RESET_COM(tex2D);
	RESET_COM(rtv);
	RESET_COM(srv);
}


VertexBuffer* GraphicResourceFactory::CreateVertexBuffer(ParserData::Mesh* mesh)
{
	switch (mesh->m_MeshType)
	{
	case MESH_TYPE::STATIC_MESH:
		return CreateMeshVB<MeshVertex>(mesh);
	case MESH_TYPE::SKIN_MESH:
		return CreateMeshVB<SkinVertex>(mesh);
	default:
		return nullptr;
	}
}

VertexBuffer* GraphicResourceFactory::CreateTerrainVertexBuffer(ParserData::Mesh* mesh, std::string maskName)
{
	return CreateTerrainVB(mesh, maskName);
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


IndexBuffer* GraphicResourceFactory::CreateIndexBuffer(ParserData::Mesh* mesh)
{
	// 货肺款 IndexBufferData 积己..
	IndexBuffer* iBuffer = new IndexBuffer();

	ID3D11Buffer* IB = nullptr;

	// Face Count..
	UINT iCount = (UINT)mesh->m_IndexList.size();

	std::vector<UINT> indices(iCount * 3);
	for (UINT i = 0; i < iCount; ++i)
	{
		indices[i * 3 + 0] = mesh->m_IndexList[i]->m_Index[0];
		indices[i * 3 + 1] = mesh->m_IndexList[i]->m_Index[1];
		indices[i * 3 + 2] = mesh->m_IndexList[i]->m_Index[2];
	}

	// 货肺款 IndexBuffer 积己..
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * iCount * 3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&ibd, &iinitData, &IB);

	// 逞败拎具且 IndexBufferData 火涝..
	iBuffer->Count = iCount * 3;
	iBuffer->IndexBufferPointer = IB;

	return iBuffer;
}

TextureBuffer* GraphicResourceFactory::CreateTextureBuffer(std::string path)
{
	TextureBuffer* tBuffer = nullptr;

	ID3D11Resource* texResource = nullptr;
	ID3D11ShaderResourceView* newTex = nullptr;

	// Texture Buffer 积己..
	m_Graphic->CreateTextureBuffer(path, &texResource, &newTex);

	// Texture 积己 己傍矫 Texture Buffer 积己..
	if (newTex)
	{
		tBuffer = new TextureBuffer();
		tBuffer->TextureBufferPointer = newTex;

		texResource->Release();
	}

	return tBuffer;
}

template<>
VertexBuffer* GraphicResourceFactory::CreateMeshVB<MeshVertex>(ParserData::Mesh* mesh)
{
	// 货肺款 VertexBufferData 积己..
	VertexBuffer* vBuffer = new VertexBuffer();

	ID3D11Buffer* VB = nullptr;

	// Vertex Count..
	UINT vCount = (UINT)mesh->m_VertexList.size();

	std::vector<MeshVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos		= mesh->m_VertexList[i]->m_Pos;
		vertices[i].Tex		= mesh->m_VertexList[i]->m_UV;
		vertices[i].Normal	= mesh->m_VertexList[i]->m_Normal;
		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;
	}

	// 货肺款 VertexBuffer 积己..
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(MeshVertex) * vCount;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&ibd, &iinitData, &VB);

	// 逞败拎具且 VertexBufferData 火涝..
	vBuffer->Count = vCount;
	vBuffer->VertexBufferPointer = VB;
	vBuffer->VertexDataSize = sizeof(MeshVertex);
	
	return vBuffer;
}

template<>
VertexBuffer* GraphicResourceFactory::CreateMeshVB<SkinVertex>(ParserData::Mesh* mesh)
{
	// 货肺款 VertexBufferData 积己..
	VertexBuffer* vBuffer = new VertexBuffer();

	ID3D11Buffer* VB = nullptr;

	// Vertex Count..
	UINT vCount = (UINT)mesh->m_VertexList.size();
	UINT bCount = 0;
	std::vector<SkinVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos		= mesh->m_VertexList[i]->m_Pos;
		vertices[i].Tex		= mesh->m_VertexList[i]->m_UV;
		vertices[i].Normal	= mesh->m_VertexList[i]->m_Normal;
		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;

		// Bone Weights, Bone Index..
		bCount = (UINT)mesh->m_VertexList[i]->m_BoneIndices.size();
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

	// 货肺款 VertexBuffer 积己..
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(SkinVertex) * vCount;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&ibd, &iinitData, &VB);

	// 逞败拎具且 VertexBufferData 火涝..
	vBuffer->Count = vCount;
	vBuffer->VertexBufferPointer = VB;
	vBuffer->VertexDataSize = sizeof(SkinVertex);

	return vBuffer;
}

VertexBuffer* GraphicResourceFactory::CreateTerrainVB(ParserData::Mesh* mesh, std::string maskName)
{
	// 货肺款 VertexBufferData 积己..
	VertexBuffer* vBuffer = new VertexBuffer();

	ID3D11Buffer* VB = nullptr;

	// Vertex Count..
	UINT vCount = (UINT)mesh->m_VertexList.size();

	// Mask Pixel Data Parsing..
	ParserData::ImageData maskImage = m_Parser->LoadImagePixel(maskName.c_str(), 4);

	std::vector<TerrainVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;

		if (i != 0)
		{
			vertices[i].Tex.x = vertices[i].Pos.x;
			vertices[i].Tex.y = vertices[i].Pos.z;
		}

		vertices[i].Normal	= mesh->m_VertexList[i]->m_Normal;
		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;

		// 秦寸 Pixel Mask Color..
		vertices[i].Mask = m_Parser->GetPixelColor(maskImage, abs(vertices[i].Pos.x), abs(vertices[i].Pos.z));
	}

	// 货肺款 VertexBuffer 积己..
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(TerrainVertex) * vCount;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &vertices[0];

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&ibd, &iinitData, &VB);

	// 逞败拎具且 VertexBufferData 火涝..
	vBuffer->Count = vCount;
	vBuffer->VertexBufferPointer = VB;
	vBuffer->VertexDataSize = sizeof(TerrainVertex);

	return vBuffer;
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

	// NoDepth DepthStencilState 积己..
	CreateDepthStencilState<DSS_NoDepth>(&depthStencilDesc);
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
	CreateRasterizerState<RS_CullNone>(&rasterizerDesc);

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
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
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
	blendDesc.AlphaToCoverageEnable = TRUE;
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
}

void GraphicResourceFactory::CreateDepthStencilViews(int width, int height)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;

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
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Defalt DepthStencilView 积己..
	CreateDepthStencil<DS_Defalt>(&texDesc, nullptr, &descDSV);

	RESET_COM(tex2D);
}

void GraphicResourceFactory::CreateViewPorts(int width, int height)
{
	// Defalt ViewPort 积己..
	CreateViewPort<VP_FullScreen>(0.0f, 0.0f, 1.0f, 1.0f, (float)width, (float)height);
}

void GraphicResourceFactory::CreateQuadBuffer()
{
	BufferData* newBuf = new BufferData();

	PosTexVertex v[4];
	v[0].Pos = Vector3(-1.0f, -1.0f, 0.0f);
	v[1].Pos = Vector3(-1.0f, +1.0f, 0.0f);
	v[2].Pos = Vector3(+1.0f, +1.0f, 0.0f);
	v[3].Pos = Vector3(+1.0f, -1.0f, 0.0f);

	v[0].Tex = Vector2(0.0f, 1.0f);
	v[1].Tex = Vector2(0.0f, 0.0f);
	v[2].Tex = Vector2(1.0f, 0.0f);
	v[3].Tex = Vector2(1.0f, 1.0f);

	UINT indices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	newBuf->Stride = sizeof(PosTexVertex);
	newBuf->IndexCount = 6;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PosTexVertex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&vbd, &vinitData, &newBuf->VB);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&ibd, &iinitData, &newBuf->IB);

	// Resource 殿废..
	m_ResourceManager->AddResource<BD_Quad>(newBuf);
}

void GraphicResourceFactory::CreateSSAOQuadBuffer()
{
	BufferData* newBuf = new BufferData();

	PosNormalTexVertex v[4];

	v[0].Pos = Vector3(-1.0f, -1.0f, 0.0f);
	v[1].Pos = Vector3(-1.0f, +1.0f, 0.0f);
	v[2].Pos = Vector3(+1.0f, +1.0f, 0.0f);
	v[3].Pos = Vector3(+1.0f, -1.0f, 0.0f);

	// Store far plane frustum corner indices in Normal.x slot.
	v[0].Normal = Vector3(0.0f, 0.0f, 0.0f);
	v[1].Normal = Vector3(1.0f, 0.0f, 0.0f);
	v[2].Normal = Vector3(2.0f, 0.0f, 0.0f);
	v[3].Normal = Vector3(3.0f, 0.0f, 0.0f);

	v[0].Tex = Vector2(0.0f, 1.0f);
	v[1].Tex = Vector2(0.0f, 0.0f);
	v[2].Tex = Vector2(1.0f, 0.0f);
	v[3].Tex = Vector2(1.0f, 1.0f);

	USHORT indices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	newBuf->Stride = sizeof(PosNormalTexVertex);
	newBuf->IndexCount = 6;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(PosNormalTexVertex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = v;

	// Vertex Buffer 积己..
	m_Graphic->CreateBuffer(&vbd, &vinitData, &newBuf->VB);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// Index Buffer 积己..
	m_Graphic->CreateBuffer(&ibd, &iinitData, &newBuf->IB);

	// Resource 殿废..
	m_ResourceManager->AddResource<BD_SSAO>(newBuf);
}
