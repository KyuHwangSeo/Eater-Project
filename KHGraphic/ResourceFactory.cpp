#include <vector>
#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "GraphicState.h"
#include "BufferData.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "ComputeRenderTarget.h"
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
#include "ToolKitDefine.h"
#include "BlendStateDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RasterizerStateDefine.h"
#include "BufferDataDefine.h"
#include "ViewPortDefine.h"


using namespace DirectX::SimpleMath;

GraphicResourceFactory::GraphicResourceFactory(D3D11Graphic* graphic, IGraphicResourceManager* resource)
{
	// Graphic Resource & Shader Manager 积己..
	m_ResourceManager = resource;

	m_Device = graphic->GetDevice();
	m_Context = graphic->GetContext();
	m_SwapChain = graphic->GetSwapChain();
}

GraphicResourceFactory::~GraphicResourceFactory()
{

}

void GraphicResourceFactory::Initialize(int width, int height)
{
	// Back Buffer 积己..
	CreateMainRenderTarget(width, height);

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
	RESET_COM(m_Device);
	RESET_COM(m_Context);
	RESET_COM(m_SwapChain);
	
	SAFE_RELEASE(m_ShaderManager);
	SAFE_RELEASE(m_ResourceManager);
}

void GraphicResourceFactory::CreateTexture2D(D3D11_TEXTURE2D_DESC* texDesc, ID3D11Texture2D** tex2D)
{
	// Texture2D Resource 积己..
	HR(m_Device->CreateTexture2D(texDesc, 0, tex2D));
}

void GraphicResourceFactory::CreateRenderTargetView(ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, ID3D11RenderTargetView** rtv)
{
	// RenderTargetView Resource 积己..
	HR(m_Device->CreateRenderTargetView(tex2D, rtvDesc, rtv));
}

void GraphicResourceFactory::CreateShaderResourceView(ID3D11Texture2D* tex2D, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, ID3D11ShaderResourceView** srv)
{
	// ShaderResourceView Resource 积己..
	HR(m_Device->CreateShaderResourceView(tex2D, srvDesc, srv));
}

void GraphicResourceFactory::CreateUnorderedAccessView(ID3D11Texture2D* tex2D, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc, ID3D11UnorderedAccessView** uav)
{
	// UnorderedAccessView Resource 积己..
	HR(m_Device->CreateUnorderedAccessView(tex2D, uavDesc, uav));
}

void GraphicResourceFactory::CreateDSS(Hash_Code hash_code, D3D11_DEPTH_STENCIL_DESC* dssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> newResource = nullptr;

	// DepthStencilState Resource 积己..
	HR(m_Device->CreateDepthStencilState(dssDesc, newResource.GetAddressOf()));

	// DepthStencilState 积己..
	DepthStencilState* dss = new DepthStencilState(newResource.GetAddressOf());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, dss);

	// Reset Pointer..
	RESET_COM(newResource);
}

void GraphicResourceFactory::CreateRS(Hash_Code hash_code, D3D11_RASTERIZER_DESC* rsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> newResource = nullptr;

	// RasterizerState Resource 积己..
	HR(m_Device->CreateRasterizerState(rsDesc, newResource.GetAddressOf()));

	// RasterizerState 积己..
	RasterizerState* rs = new RasterizerState(newResource.GetAddressOf());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, rs);

	// Reset Pointer..
	RESET_COM(newResource);
}

void GraphicResourceFactory::CreateBS(Hash_Code hash_code, D3D11_BLEND_DESC* bsDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11BlendState> newResource = nullptr;

	// BlendState Resource 积己..
	HR(m_Device->CreateBlendState(bsDesc, newResource.GetAddressOf()));

	// BlendState 积己..
	BlendState* rs = new BlendState(newResource.GetAddressOf());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, rs);

	// Reset Pointer..
	RESET_COM(newResource);
}

void GraphicResourceFactory::CreateSS(Hash_Code hash_code, D3D11_SAMPLER_DESC* ssDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11SamplerState> newResource = nullptr;

	// SamplerState Resource 积己..
	HR(m_Device->CreateSamplerState(ssDesc, newResource.GetAddressOf()));

	// SamplerState 积己..
	SamplerState* ss = new SamplerState(newResource.GetAddressOf());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, ss);

	// Reset Pointer..
	RESET_COM(newResource);
}

void GraphicResourceFactory::CreateVP(Hash_Code hash_code, float ratio_offsetX, float ratio_offsetY, float ratio_sizeX, float ratio_sizeY, float width, float height)
{
	// ViewPort 积己..
	ViewPort* viewPort = new ViewPort(ratio_offsetX, ratio_offsetY, ratio_sizeX, ratio_sizeY, width, height);

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, viewPort);
}

void GraphicResourceFactory::CreateDSV(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	// 货肺款 Resource Pointer 积己..
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> newResource = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> newSubResource = nullptr;

	// DepthStencilView Resource 积己..
	HR(m_Device->CreateDepthStencilView(tex2D, dsvDesc, newResource.GetAddressOf()));

	// ShaderResourceView Resource 积己..
	if (srvDesc) HR(m_Device->CreateShaderResourceView(tex2D, srvDesc, newSubResource.GetAddressOf()));

	// DepthStencilView 积己..
	DepthStencilView* dsv = new DepthStencilView(newResource.GetAddressOf(), newSubResource.GetAddressOf());

	// Resoure 殿废..
	m_ResourceManager->AddResource(hash_code, dsv);

	// Reset Pointer..
	RESET_COM(newResource);
	RESET_COM(newSubResource);
}

void GraphicResourceFactory::CreateBasicRT(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	if (rtvDesc) CreateRenderTargetView(tex2D, rtvDesc, rtv.GetAddressOf());
	if (srvDesc) CreateShaderResourceView(tex2D, srvDesc, srv.GetAddressOf());

	// Basic RenderTarget 积己..
	BasicRenderTarget* basicRenderTarget = new BasicRenderTarget(rtv.GetAddressOf(), srv.GetAddressOf());

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, basicRenderTarget);

	// Reset Pointer..
	RESET_COM(rtv);
	RESET_COM(srv);
}

void GraphicResourceFactory::CreateComputeRT(Hash_Code hash_code, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> uav = nullptr;

	CreateRenderTargetView(tex2D, rtvDesc, rtv.GetAddressOf());
	CreateUnorderedAccessView(tex2D, uavDesc, uav.GetAddressOf());

	// Compute RenderTarget 积己..
	ComputeRenderTarget* computeRenderTarget = new ComputeRenderTarget(rtv.GetAddressOf(), uav.GetAddressOf());

	// Resource 殿废..
	m_ResourceManager->AddResource(hash_code, computeRenderTarget);

	// Reset Pointer..
	RESET_COM(rtv);
	RESET_COM(uav);
}

void GraphicResourceFactory::CreateMainRenderTarget(UINT width, UINT height)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	// Swap Chain, Render Target View Resize
	HR(m_SwapChain->ResizeBuffers(1, (UINT)width, (UINT)height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	// Get Swap Chain Back Buffer Pointer..
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(tex2D.GetAddressOf())));

	CreateRenderTargetView(tex2D.Get(), nullptr, rtv.GetAddressOf());
	CreateShaderResourceView(tex2D.Get(), nullptr, srv.GetAddressOf());

	// Main RenderTarget 积己..
	BasicRenderTarget* mainRenderTarget = new BasicRenderTarget(rtv.GetAddressOf(), srv.GetAddressOf());

	// Resource 殿废..
	m_ResourceManager->AddMainRenderTarget(mainRenderTarget);

	// Reset Pointer..
	RESET_COM(tex2D);
	RESET_COM(rtv);
	RESET_COM(srv);
}

Vertexbuffer* GraphicResourceFactory::CreateVertexBuffer(ParserData::Mesh* mesh)
{
	if (mesh->m_IsSkinningObject)
	{
		return CreateMeshVertexBuffer<SkinVertex>(mesh);
	}
	else
	{
		return CreateMeshVertexBuffer<MeshVertex>(mesh);

		// Terrain Type
		//return CreateMeshVertexBuffer<TerrainVertex>(mesh);
	}
}

Indexbuffer* GraphicResourceFactory::CreateIndexBuffer(ParserData::Mesh* mesh)
{
	// 货肺款 IndexBufferData 积己..
	Indexbuffer* iBuffer = new Indexbuffer();

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
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &IB));

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
	
	std::wstring wPath(path.begin(), path.end());

	// 犬厘磊俊 蝶弗 咆胶贸 颇老 肺靛 规侥..
	if (path.rfind(".dds") != std::string::npos)
	{
		HR(DirectX::CreateDDSTextureFromFile(m_Device.Get(), wPath.c_str(), &texResource, &newTex));
	}
	else
	{
		HR(DirectX::CreateWICTextureFromFile(m_Device.Get(), wPath.c_str(), &texResource, &newTex));
	}

	// Texture 积己 己傍矫 Texture Buffer 积己..
	if (newTex)
	{
		tBuffer = new TextureBuffer();
		tBuffer->TextureBufferPointer = newTex;

		texResource->Release();
	}

	return tBuffer;
}

IShaderManager* GraphicResourceFactory::GetShaderManager()
{
	return m_ShaderManager;
}

IGraphicResourceManager* GraphicResourceFactory::GetResourceManager()
{
	return m_ResourceManager;
}

template<>
Vertexbuffer* GraphicResourceFactory::CreateMeshVertexBuffer<MeshVertex>(ParserData::Mesh* mesh)
{
	// 货肺款 VertexBufferData 积己..
	Vertexbuffer* vBuffer = new Vertexbuffer();

	ID3D11Buffer* VB = nullptr;

	// Vertex Count..
	UINT vCount = (UINT)mesh->m_VertexList.size();

	std::vector<MeshVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;

		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;

		vertices[i].Tex = mesh->m_VertexList[i]->m_UV;

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
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &VB));

	// 逞败拎具且 VertexBufferData 火涝..
	vBuffer->Count = vCount;
	vBuffer->VertexbufferPointer = VB;
	vBuffer->VertexDataSize = sizeof(MeshVertex);

	return vBuffer;
}

template<>
Vertexbuffer* GraphicResourceFactory::CreateMeshVertexBuffer<SkinVertex>(ParserData::Mesh* mesh)
{
	// 货肺款 VertexBufferData 积己..
	Vertexbuffer* vBuffer = new Vertexbuffer();

	ID3D11Buffer* VB = nullptr;

	// Vertex Count..
	UINT vCount = (UINT)mesh->m_VertexList.size();
	UINT bCount = 0;
	std::vector<SkinVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;

		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;

		vertices[i].Tex = mesh->m_VertexList[i]->m_UV;

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
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &VB));

	// 逞败拎具且 VertexBufferData 火涝..
	vBuffer->Count = vCount;
	vBuffer->VertexbufferPointer = VB;
	vBuffer->VertexDataSize = sizeof(SkinVertex);

	return vBuffer;
}

template<>
Vertexbuffer* GraphicResourceFactory::CreateMeshVertexBuffer<TerrainVertex>(ParserData::Mesh* mesh)
{
	// 货肺款 VertexBufferData 积己..
	Vertexbuffer* vBuffer = new Vertexbuffer();

	ID3D11Buffer* VB = nullptr;

	// Vertex Count..
	UINT vCount = (UINT)mesh->m_VertexList.size();

	std::vector<TerrainVertex> vertices(vCount);
	for (UINT i = 0; i < vCount; i++)
	{
		vertices[i].Pos = mesh->m_VertexList[i]->m_Pos;

		vertices[i].Normal = mesh->m_VertexList[i]->m_Normal;

		vertices[i].Tex = mesh->m_VertexList[i]->m_UV;

		vertices[i].Tangent = mesh->m_VertexList[i]->m_Tanget;

		// 秦寸 Pixel Mask Color..
		vertices[i].Mask1;
		vertices[i].Mask2;
	}

	// 货肺款 VertexBuffer 积己..
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(TerrainVertex) * vCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &VB));

	// 逞败拎具且 VertexBufferData 火涝..
	vBuffer->Count = vCount;
	vBuffer->VertexbufferPointer = VB;
	vBuffer->VertexDataSize = sizeof(TerrainVertex);

	return vBuffer;
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

	CreateTexture2D(&texDesc, tex2D.GetAddressOf());

	// Defalt DepthStencilView 积己..
	CreateDepthStencilView<DSV_Defalt>(tex2D.Get(), nullptr, nullptr);

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

	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	// Resource 殿废..
	m_ResourceManager->AddResource<BD_FullScreen>(newBuf);
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

	UINT indices[6] =
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

	HR(m_Device->CreateBuffer(&vbd, &vinitData, &newBuf->VB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	HR(m_Device->CreateBuffer(&ibd, &iinitData, &newBuf->IB));

	// Resource 殿废..
	m_ResourceManager->AddResource<BD_SSAOScreen>(newBuf);
}
