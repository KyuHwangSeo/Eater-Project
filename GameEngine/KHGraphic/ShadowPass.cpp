#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "Buffer.h"
#include "Texture2D.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "MathDefine.h"
#include "EngineData.h"
#include "ShadowPass.h"

#include "VertexDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "ViewPortDefine.h"
#include "RenderTargetDefine.h"
#include "RasterizerStateDefine.h"

ShadowPass::ShadowPass()
	:m_ShadowDSV(nullptr)
{
}

ShadowPass::~ShadowPass()
{

}

void ShadowPass::Create(int width, int height)
{
	// ViewPort 설정..
	g_Factory->CreateViewPort<VP_Shadow>(0.0f, 0.0f, 4.0f, 4.0f, (float)width, (float)height);

	// DepthStencilView 설정..
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width * 4;
	texDesc.Height = height * 4;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	// Shadow DepthStencilView 생성..
	g_Factory->CreateDepthStencil<DS_Shadow>(&texDesc, nullptr, &dsvDesc, &srvDesc);
}

void ShadowPass::Start(int width, int height)
{
	// Shader 설정..
	m_MeshShadowVS = g_Shader->GetShader("MeshShadow_VS");
	m_SkinShadowVS = g_Shader->GetShader("SkinShadow_VS");
	
	m_ShadowDS = g_Resource->GetDepthStencil<DS_Shadow>();
	m_ShadowDS->SetRatio(4.0f, 4.0f);

	m_ShadowVP = g_Resource->GetViewPort<VP_Shadow>()->Get();
	m_DepthRS = g_Resource->GetRasterizerState<RS_Depth>()->Get();

	// Shadow DepthStencilView 설정..
	m_ShadowDSV = m_ShadowDS->GetDSV()->Get();
}

void ShadowPass::OnResize(int width, int height)
{
	// Shadow DepthStencilView 재설정..
	m_ShadowDSV = m_ShadowDS->GetDSV()->Get();
}

void ShadowPass::Release()
{

}

void ShadowPass::SetOption(UINT renderOption)
{
	g_Context->ClearDepthStencilView(m_ShadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void ShadowPass::BeginRender()
{
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->RSSetViewports(1, m_ShadowVP);

	// 그리기만 할 것이므로 null Render Target 설정..
	// 깊이 버퍼, null Rendering 대상을 설정하면 색상 쓰기가 비활성화 된다..
	g_Context->OMSetRenderTargets(0, nullptr, m_ShadowDSV);
	g_Context->ClearDepthStencilView(m_ShadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	g_Context->RSSetState(m_DepthRS);
}

void ShadowPass::Update(MeshData* mesh, GlobalData* global)
{
	Matrix world = mesh->mWorld;
	Matrix cameraViewProj = global->mCamVP;
	Matrix lightViewProj = global->mLightVP;

	switch (mesh->ObjType)
	{
	case OBJECT_TYPE::BASE:
	case OBJECT_TYPE::TERRAIN:
	{
		CB_ShadowMeshObject shadowBuf;
		shadowBuf.gWorldViewProj = world * lightViewProj;

		m_MeshShadowVS->ConstantBufferCopy(&shadowBuf);

		m_MeshShadowVS->Update();
	}
	break;
	case OBJECT_TYPE::SKINNING:
	{
		CB_ShadowSkinObject shadowBuf;
		shadowBuf.gWorldViewProj = world * lightViewProj;

		for (int i = 0; i < mesh->BoneOffsetTM.size(); i++)
		{
			shadowBuf.gBoneTransforms[i] = mesh->BoneOffsetTM[i];
		}

		m_SkinShadowVS->ConstantBufferCopy(&shadowBuf);

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
	ID3D11Buffer* vBuffer = reinterpret_cast<ID3D11Buffer*>(mesh->VB->VertexBufferPointer);

	UINT indexCount = mesh->IB->Count;
	UINT stride = mesh->VB->VertexDataSize;
	UINT offset = 0;

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	g_Context->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw..
	g_Context->DrawIndexed(indexCount, 0, 0);
}
