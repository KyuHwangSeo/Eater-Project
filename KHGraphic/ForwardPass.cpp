#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "VertexDefine.h"
#include "EngineData.h"
#include "ForwardPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceBufferDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RasterizerStateDefine.h"
#include "BlendStateDefine.h"

ForwardPass::ForwardPass()
{

}

ForwardPass::~ForwardPass()
{

}

void ForwardPass::Create(int width, int height)
{
	// ViewPort 설정..
	//m_ScreenViewport = g_Resource->GetViewPort(eViewPort::SCREEN);
	
	// BackBuffer 생성..
	//m_BackBuffer = g_Resource->GetMainRenderTarget();
	//m_BackBufferRTV = m_BackBuffer->GetRTV();
	//m_BackBufferSRV = m_BackBuffer->GetSRV();
}

void ForwardPass::Start()
{
	// Shader 설정..
	m_MeshVS = g_Shader->GetShader("MeshVS");
	m_SkinVS = g_Shader->GetShader("SkinVS");
	m_ForwardPS = g_Shader->GetShader("ForwardPS");

	// DepthStencilView 설정..
	m_DSV = g_Resource->GetDepthStencilView<DSV_Defalt>();
	m_DepthStencilView = m_DSV->Get();

	m_DepthStencilState = g_Resource->GetDepthStencilState<DSS_Defalt>();
	m_RasterizerState = g_Resource->GetRasterizerState<RS_Solid>();
	m_BlendState = g_Resource->GetBlendState<BS_AlphaBlend>();
}

void ForwardPass::OnResize(int width, int height)
{
	// BackBuffer RenderTargetView 재설정..
	//m_BackBufferRTV = m_BackBuffer->GetRTV();

	// BackBuffer ShaderResourceView 재설정..
	//m_BackBufferSRV = m_BackBuffer->GetSRV();

	// DepthStencilView 재설정..
	m_DepthStencilView = m_DSV->Get();
}

void ForwardPass::Release()
{

}

void ForwardPass::BeginRender()
{
	//g_Context->OMSetRenderTargets(1, &m_BackBufferRTV, m_DepthStencilView);
	//g_Context->OMSetDepthStencilState(m_DepthStencilState, 0);
	g_Context->OMSetBlendState(m_BlendState, 0, 0xffffffff);
	//g_Context->ClearRenderTargetView(m_BackBufferRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	//g_Context->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//g_Context->RSSetViewports(1, m_ScreenViewport);
	g_Context->RSSetState(m_RasterizerState);
}

void ForwardPass::Update(MeshData* mesh, GlobalData* global)
{
	Matrix world = mesh->mWorld;
	Matrix view = *global->mViewMX;
	Matrix proj = *global->mProj;
	Matrix shadowTrans = *global->mShadowTrans;
	Vector3 eye(view._41, view._42, view._43);
	LightData* lightData = global->mLightData;
	ID3D11ShaderResourceView* diffuse_srv = nullptr;
	ID3D11ShaderResourceView* normal_srv = nullptr;

	switch (mesh->ObjType)
	{
	case OBJECT_TYPE::BASE:
	{
		CB_Object objectBuf;
		objectBuf.gWorld = world;
		objectBuf.gWorldViewProj = world * view * proj;

		CB_Shadow shadowBuf;
		shadowBuf.gShadowTransform = world * shadowTrans;

		m_MeshVS->SetConstantBuffer(objectBuf);
		m_MeshVS->SetConstantBuffer(shadowBuf);

		// Vertex Shader Update..
		m_MeshVS->Update();
	}
		break;
	case OBJECT_TYPE::SKINNING:
	{
		CB_Object objectBuf;
		objectBuf.gWorld = world;
		objectBuf.gWorldViewProj = world * view * proj;

		CB_Skinned skinBuf;
		for (int i = 0; i < mesh->BoneOffsetTM.size(); i++)
		{
			skinBuf.gBoneTransforms[i] = mesh->BoneOffsetTM[i];
		}

		CB_Shadow shadowBuf;
		shadowBuf.gShadowTransform = world * shadowTrans;

		m_SkinVS->SetConstantBuffer(objectBuf);
		m_SkinVS->SetConstantBuffer(skinBuf);
		m_SkinVS->SetConstantBuffer(shadowBuf);

		// Vertex Shader Update..
		m_SkinVS->Update();
	}
		break;
	default:
		break;
	}

	CB_Lights lightBuf;
	lightBuf.gPointLightCount = lightData->gPointLightCount;
	lightBuf.gSpotLightCount = lightData->gSpotLightCount;

	lightBuf.gDirLights[0] = *lightData->DirLights[0];

	for (UINT p = 0; p < lightBuf.gPointLightCount; p++)
	{
		lightBuf.gPointLights[p] = *lightData->PointLights[p];
	}
	for (UINT s = 0; s < lightBuf.gSpotLightCount; s++)
	{
		lightBuf.gSpotLights[s] = *lightData->SpotLights[s];
	}

	CB_Camera cameraBuf;
	cameraBuf.gEyePosW = eye;

	CB_Materials matBuf;
	for (int i = 0; i < 5; i++)
	{
		matBuf.gMaterials[i] = global->mMatData[i];
	}

	m_ForwardPS->SetConstantBuffer(lightBuf);
	m_ForwardPS->SetConstantBuffer(cameraBuf);
	m_ForwardPS->SetConstantBuffer(matBuf);

	if (mesh->Diffuse)
	{
		diffuse_srv = reinterpret_cast<ID3D11ShaderResourceView*>(mesh->Diffuse->TextureBufferPointer);
	}
	if (mesh->Normal)
	{
		normal_srv = reinterpret_cast<ID3D11ShaderResourceView*>(mesh->Normal->TextureBufferPointer);
	}

	m_ForwardPS->SetShaderResourceView<gDiffuseMap>(&diffuse_srv);
	m_ForwardPS->SetShaderResourceView<gNormalMap>(&normal_srv);

	// Pixel Shader Update..
	m_ForwardPS->Update();
}

void ForwardPass::Render(MeshData* mesh)
{
	ID3D11Buffer* iBuffer = reinterpret_cast<ID3D11Buffer*>(mesh->IB->IndexBufferPointer);
	ID3D11Buffer* vBuffer = reinterpret_cast<ID3D11Buffer*>(mesh->VB->VertexbufferPointer);

	UINT indexCount = mesh->IB->Count;
	UINT stride = mesh->VB->VertexDataSize;
	UINT offset = 0;

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	g_Context->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(indexCount, 0, 0);
}
