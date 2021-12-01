#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ViewPort.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "MathDefine.h"
#include "EngineData.h"
#include "ShadowPass.h"

#include "VertexDefine.h"
#include "ConstantBufferDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ShaderResourceBufferDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "RasterizerStateDefine.h"

ShadowPass::ShadowPass()
	:m_ShadowDSV(nullptr), m_ShadowSRV(nullptr)
{
}

ShadowPass::~ShadowPass()
{

}

void ShadowPass::Create(int width, int height)
{
	// ViewPort 설정..
	g_Factory->CreateViewPort<VP_Shadow>(0.0f, 0.0f, (float)width, (float)height, 4.0f, 4.0f);

	// DepthStencilView 설정..
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width * 4;
	texDesc.Height = height * 4;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	//texDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// Texture 2D 생성..
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D = nullptr;
	g_Factory->CreateTexture2D(&texDesc, tex2D.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	// Shadow DepthStencilView 생성..
	g_Factory->CreateDSV<DSV_Shadow>(tex2D.Get(), &dsvDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	// ShaderResourceView 생성..
	g_Factory->CreateSRV(tex2D.Get(), &srvDesc, &m_ShadowSRV);

	// RenderTarget 생성..
	g_Factory->CreateBasicRenderTarget<RT_Shadow>(nullptr, &m_ShadowSRV);

	// Texture2D Resource Reset..
	RESET_COM(tex2D);
}

void ShadowPass::Start()
{
	// Shader 설정..
	m_MeshShadowVS = g_Shader->GetShader("ShadowMeshVS");
	m_SkinShadowVS = g_Shader->GetShader("ShadowSkinVS");
	m_ForwardPS = g_Shader->GetShader("ForwardPS");
	
	m_ShadowRT = g_Resource->GetRenderTarget<RT_Shadow>();
	m_ShadowRT->SetRatio(4.0f, 4.0f);
	m_ShadowViewport = g_Resource->GetViewPort<VP_Shadow>();
	m_RasterizerState = g_Resource->GetRasterizerState<RS_Depth>();

	m_ShadowDepthStencilView = g_Resource->GetDepthStencilView<DSV_Shadow>();
	m_ShadowDepthStencilView->SetRatio(4.0f, 4.0f);

	// Shadow DepthStencilView 설정..
	m_ShadowDSV = m_ShadowDepthStencilView->Get();

	// Shadow Map 등록..
	m_ForwardPS->SetShaderResourceView<gShadowMap>(&m_ShadowSRV);
}

void ShadowPass::OnResize(int width, int height)
{
	// Shadow DepthStencilView 재설정..
	m_ShadowDSV = m_ShadowDepthStencilView->Get();

	// Shadow ShaderResourceView 재설정..
	m_ShadowSRV = m_ShadowRT->GetSRV();

	// Shadow Map 등록..
	m_ForwardPS->SetShaderResourceView<gShadowMap>(&m_ShadowSRV);
}

void ShadowPass::Release()
{

}

void ShadowPass::BeginRender()
{
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->RSSetViewports(1, m_ShadowViewport);

	// 그리기만 할 것이므로 null Render Target 설정..
	// 깊이 버퍼, null Rendering 대상을 설정하면 색상 쓰기가 비활성화 된다..
	g_Context->OMSetRenderTargets(0, nullptr, m_ShadowDSV);
	g_Context->ClearDepthStencilView(m_ShadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_Context->RSSetState(m_RasterizerState);
}

void ShadowPass::Update(MeshData* mesh, GlobalData* global)
{
	Matrix world = mesh->mWorld;
	Matrix view = *global->mLightViewMX;
	Matrix proj = *global->mLightProj;

	switch (mesh->ObjType)
	{
	case OBJECT_TYPE::BASE:
	{
		CB_ShadowObject shadowBuf;
		shadowBuf.gWorldViewProj = world * view * proj;

		m_MeshShadowVS->SetConstantBuffer(shadowBuf);

		m_MeshShadowVS->Update();
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		CB_ShadowObject shadowBuf;
		shadowBuf.gWorldViewProj = world * view * proj;

		CB_Skinned skinBuf;
		for (int i = 0; i < mesh->BoneOffsetTM.size(); i++)
		{
			skinBuf.gBoneTransforms[i] = mesh->BoneOffsetTM[i];
		}

		m_SkinShadowVS->SetConstantBuffer(shadowBuf);
		m_SkinShadowVS->SetConstantBuffer(skinBuf);

		m_SkinShadowVS->Update();
	}
	break;
	default:
		break;
	}
}

void ShadowPass::Render(MeshData* mesh)
{
	/// 실제 렌더링 추가
	ID3D11Buffer* iBuffer = reinterpret_cast<ID3D11Buffer*>(mesh->IB->IndexBufferPointer);
	ID3D11Buffer* vBuffer = reinterpret_cast<ID3D11Buffer*>(mesh->VB->VertexbufferPointer);

	UINT indexCount = mesh->IB->Count;
	UINT stride = mesh->VB->VertexDataSize;
	UINT offset = 0;

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	g_Context->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw..
	g_Context->DrawIndexed(indexCount, 0, 0);
}
