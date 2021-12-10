#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "Texture2D.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "VertexDefine.h"
#include "EngineData.h"
#include "DeferredPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceBufferDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RasterizerStateDefine.h"
#include "BlendStateDefine.h"

#define ALBEDO_MAP 0x00000001
#define NORMAL_MAP 0x00000010

DeferredPass::DeferredPass()
{

}

DeferredPass::~DeferredPass()
{

}

void DeferredPass::Create(int width, int height)
{
	// Texture 2D
	D3D11_TEXTURE2D_DESC texDescDiffuse;
	ZeroMemory(&texDescDiffuse, sizeof(texDescDiffuse));
	texDescDiffuse.Width = width;
	texDescDiffuse.Height = height;
	texDescDiffuse.MipLevels = 1;
	texDescDiffuse.ArraySize = 1;
	texDescDiffuse.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDescDiffuse.SampleDesc.Count = 1;
	texDescDiffuse.SampleDesc.Quality = 0;
	texDescDiffuse.Usage = D3D11_USAGE_DEFAULT;
	texDescDiffuse.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDescDiffuse.CPUAccessFlags = 0;
	texDescDiffuse.MiscFlags = 0;

	D3D11_TEXTURE2D_DESC texDescPosNormal;
	ZeroMemory(&texDescPosNormal, sizeof(texDescPosNormal));
	texDescPosNormal.Width = width;
	texDescPosNormal.Height = height;
	texDescPosNormal.MipLevels = 1;
	texDescPosNormal.ArraySize = 1;
	texDescPosNormal.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDescPosNormal.SampleDesc.Count = 1;
	texDescPosNormal.SampleDesc.Quality = 0;
	texDescPosNormal.Usage = D3D11_USAGE_DEFAULT;
	texDescPosNormal.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDescPosNormal.CPUAccessFlags = 0;
	texDescPosNormal.MiscFlags = 0;

	// RenderTargetView 2D
	D3D11_RENDER_TARGET_VIEW_DESC rtvDescDiffuse;
	ZeroMemory(&rtvDescDiffuse, sizeof(rtvDescDiffuse));
	rtvDescDiffuse.Format = texDescDiffuse.Format;
	rtvDescDiffuse.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDescDiffuse.Texture2D.MipSlice = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDescPosNormal;
	ZeroMemory(&rtvDescPosNormal, sizeof(rtvDescPosNormal));
	rtvDescPosNormal.Format = texDescPosNormal.Format;
	rtvDescPosNormal.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDescPosNormal.Texture2D.MipSlice = 0;

	// ShaderResourceView 2D
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescDiffuse;
	ZeroMemory(&srvDescDiffuse, sizeof(srvDescDiffuse));
	srvDescDiffuse.Format = texDescDiffuse.Format;
	srvDescDiffuse.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDescDiffuse.Texture2D.MostDetailedMip = 0;
	srvDescDiffuse.Texture2D.MipLevels = 1;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDescPosNormal;
	ZeroMemory(&srvDescPosNormal, sizeof(srvDescPosNormal));
	srvDescPosNormal.Format = texDescPosNormal.Format;
	srvDescPosNormal.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDescPosNormal.Texture2D.MostDetailedMip = 0;
	srvDescPosNormal.Texture2D.MipLevels = 1;

	// RenderTarget 생성..
	g_Factory->CreateRenderTarget<RT_Deffered_Albedo>(&texDescDiffuse, &rtvDescDiffuse, &srvDescDiffuse);
	g_Factory->CreateRenderTarget<RT_Deffered_Normal>(&texDescPosNormal, &rtvDescPosNormal, &srvDescPosNormal);
	g_Factory->CreateRenderTarget<RT_Deffered_Position>(&texDescPosNormal, &rtvDescPosNormal, &srvDescPosNormal);
	g_Factory->CreateRenderTarget<RT_Deffered_Shadow>(&texDescPosNormal, &rtvDescPosNormal, &srvDescPosNormal);
	g_Factory->CreateRenderTarget<RT_Deffered_Depth>(&texDescPosNormal, &rtvDescPosNormal, &srvDescPosNormal);
}

void DeferredPass::Start()
{
	// Shader 설정..
	m_MeshVS = g_Shader->GetShader("MeshVS");
	m_SkinVS = g_Shader->GetShader("SkinVS");
	m_DeferredPS = g_Shader->GetShader("DeferredPS");

	// DepthStencilView 설정..
	m_DepthStencilView = g_Resource->GetDepthStencil<DS_Defalt>()->GetDSV();

	// Graphic State 설정..
	m_DepthStencilState = g_Resource->GetDepthStencilState<DSS_Defalt>()->Get();
	m_RasterizerState = g_Resource->GetRasterizerState<RS_Solid>()->Get();
	m_BlendState = g_Resource->GetBlendState<BS_AlphaBlend>()->Get();
	
	// ViewPort 설정..
	m_ScreenViewport = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	// RenderTarget 설정..
	m_AlbedoRT = g_Resource->GetRenderTarget<RT_Deffered_Albedo>();
	m_NormalRT = g_Resource->GetRenderTarget<RT_Deffered_Normal>();
	m_PositionRT = g_Resource->GetRenderTarget<RT_Deffered_Position>();
	m_ShadowRT = g_Resource->GetRenderTarget<RT_Deffered_Shadow>();
	m_DepthRT = g_Resource->GetRenderTarget<RT_Deffered_Depth>();

	// RenderTargetView 설정..
	m_RTVList.resize(5);
	m_RTVList[0] = m_AlbedoRT->GetRTV();
	m_RTVList[1] = m_NormalRT->GetRTV();
	m_RTVList[2] = m_PositionRT->GetRTV();
	m_RTVList[3] = m_ShadowRT->GetRTV();
	m_RTVList[4] = m_DepthRT->GetRTV();

	// ShaderResourceView 설정..
	m_SRVList.resize(5);
	m_SRVList[0] = m_AlbedoRT->GetSRV();
	m_SRVList[1] = m_NormalRT->GetSRV();
	m_SRVList[2] = m_PositionRT->GetSRV();
	m_SRVList[3] = m_ShadowRT->GetSRV();
	m_SRVList[4] = m_DepthRT->GetSRV();
}

void DeferredPass::OnResize(int width, int height)
{
	// DepthStencilView 재설정..
	m_DepthStencilView = g_Resource->GetDepthStencil<DS_Defalt>()->GetDSV();
	
	// ShaderResourceView List 재설정..
	m_SRVList[0] = m_AlbedoRT->GetSRV();
	m_SRVList[1] = m_NormalRT->GetSRV();
	m_SRVList[2] = m_PositionRT->GetSRV();
	m_SRVList[3] = m_ShadowRT->GetSRV();
	m_SRVList[4] = m_DepthRT->GetSRV();
	
	// RenderTargetView List 재설정..
	m_RTVList[0] = m_AlbedoRT->GetRTV();
	m_RTVList[1] = m_NormalRT->GetRTV();
	m_RTVList[2] = m_PositionRT->GetRTV();
	m_RTVList[3] = m_ShadowRT->GetRTV();
	m_RTVList[4] = m_DepthRT->GetRTV();
}

void DeferredPass::Release()
{

}

void DeferredPass::BeginRender()
{
	g_Context->OMSetRenderTargets(5, &m_RTVList[0], m_DepthStencilView);

	// RenderTarget 초기화..
	g_Context->ClearRenderTargetView(m_RTVList[0], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearRenderTargetView(m_RTVList[1], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearRenderTargetView(m_RTVList[2], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearRenderTargetView(m_RTVList[3], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearRenderTargetView(m_RTVList[4], reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

	g_Context->RSSetViewports(1, m_ScreenViewport);
	g_Context->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	g_Context->OMSetDepthStencilState(m_DepthStencilState, 0);
	g_Context->OMSetBlendState(m_BlendState, 0, 0xffffffff);
	g_Context->RSSetState(m_RasterizerState);
}

void DeferredPass::Update(MeshData* mesh, GlobalData* global)
{
	Matrix world = mesh->mWorld;
	Matrix view = *global->mCamView;
	Matrix proj = *global->mCamProj;
	Matrix shadowTrans = *global->mLightVPT;
	Vector3 eye(view._41, view._42, view._43);

	switch (mesh->ObjType)
	{
	case OBJECT_TYPE::BASE:
	{
		CB_MeshObject objectBuf;
		objectBuf.gWorld = world;
		objectBuf.gWorldViewProj = world * view * proj;
		objectBuf.gShadowTransform = world * shadowTrans;

		m_MeshVS->SetConstantBuffer(objectBuf);

		// Vertex Shader Update..
		m_MeshVS->Update();
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		CB_SkinObject objectBuf;
		objectBuf.gWorld = world;
		objectBuf.gWorldViewProj = world * view * proj;
		objectBuf.gShadowTransform = world * shadowTrans;

		for (size_t i = 0; i < mesh->BoneOffsetTM.size(); i++)
		{
			objectBuf.gBoneTransforms[i] = mesh->BoneOffsetTM[i];
		}

		m_SkinVS->SetConstantBuffer(objectBuf);

		// Vertex Shader Update..
		m_SkinVS->Update();
	}
	break;
	default:
		break;
	}

	CB_Material materialBuf;
	materialBuf.gMatID = mesh->Material_Index;
	
	if (mesh->Albedo)
	{
 		materialBuf.gTexID |= ALBEDO_MAP;
		m_DeferredPS->SetShaderResourceView<gDiffuseMap>((ID3D11ShaderResourceView*)mesh->Albedo->TextureBufferPointer);
	}
	if (mesh->Normal)
	{
		materialBuf.gTexID |= NORMAL_MAP;
		m_DeferredPS->SetShaderResourceView<gNormalMap>((ID3D11ShaderResourceView*)mesh->Normal->TextureBufferPointer);
	}

	m_DeferredPS->SetConstantBuffer(materialBuf);

	// Pixel Shader Update..
	m_DeferredPS->Update();
}

void DeferredPass::Render(MeshData* mesh)
{
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
