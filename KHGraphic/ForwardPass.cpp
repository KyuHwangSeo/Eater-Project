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
#include "ForwardPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ViewPortDefine.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceBufferDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RasterizerStateDefine.h"
#include "BlendStateDefine.h"

#define ALBEDO_MAP 0x00000001
#define NORMAL_MAP 0x00000010

ForwardPass::ForwardPass()
{

}

ForwardPass::~ForwardPass()
{

}

void ForwardPass::Create(int width, int height)
{

}

void ForwardPass::Start()
{
	// Shader 설정..
	m_MeshVS = g_Shader->GetShader("MeshVS");
	m_SkinVS = g_Shader->GetShader("SkinVS");
	m_ForwardPS = g_Shader->GetShader("ForwardPS");

	// ViewPort 설정..
	m_ScreenViewport = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	// BackBuffer 생성..
	m_BackBuffer = g_Resource->GetMainRenderTarget();
	m_BackBufferRTV = m_BackBuffer->GetRTV();
	m_BackBufferSRV = m_BackBuffer->GetSRV();

	// DepthStencilView 설정..
	m_DepthStencilView = g_Resource->GetDepthStencil<DS_Defalt>()->GetDSV();

	// Graphic State 설정..
	m_DepthStencilState = g_Resource->GetDepthStencilState<DSS_Defalt>()->Get();
	m_RasterizerState = g_Resource->GetRasterizerState<RS_Solid>()->Get();
	m_BlendState = g_Resource->GetBlendState<BS_AlphaBlend>()->Get();
}

void ForwardPass::OnResize(int width, int height)
{
	// BackBuffer RenderTargetView 재설정..
	m_BackBufferRTV = m_BackBuffer->GetRTV();

	// BackBuffer ShaderResourceView 재설정..
	m_BackBufferSRV = m_BackBuffer->GetSRV();

	// DepthStencilView 재설정..
	m_DepthStencilView = g_Resource->GetDepthStencil<DS_Defalt>()->GetDSV();
}

void ForwardPass::Release()
{

}

void ForwardPass::BeginRender()
{
	g_Context->OMSetRenderTargets(1, &m_BackBufferRTV, m_DepthStencilView);
	g_Context->OMSetDepthStencilState(m_DepthStencilState, 0);
	g_Context->OMSetBlendState(m_BlendState, 0, 0xffffffff);
	g_Context->ClearRenderTargetView(m_BackBufferRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	g_Context->RSSetViewports(1, m_ScreenViewport);
	g_Context->RSSetState(m_RasterizerState);
}

void ForwardPass::Update(MeshData* mesh, GlobalData* global)
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

		for (int i = 0; i < mesh->BoneOffsetTM.size(); i++)
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

	LightData* lightData = global->mLightData;

	CB_Light lightBuf;
	lightBuf.gPointLightCount = lightData->gPointLightCount;
	lightBuf.gSpotLightCount = lightData->gSpotLightCount;

	lightBuf.gDirLights = *lightData->DirLights[0];

	for (UINT p = 0; p < lightBuf.gPointLightCount; p++)
	{
		lightBuf.gPointLights[p] = *lightData->PointLights[p];
	}
	for (UINT s = 0; s < lightBuf.gSpotLightCount; s++)
	{
		lightBuf.gSpotLights[s] = *lightData->SpotLights[s];
	}

	for (UINT m = 0; m < 5; m++)
	{
		lightBuf.gMaterials[m] = global->mMatData[m];
	}
	m_ForwardPS->SetConstantBuffer(lightBuf);

	CB_LightSub lightsubBuf;
	lightsubBuf.gEyePosW = -eye;

	m_ForwardPS->SetConstantBuffer(lightsubBuf);

	CB_Material materialBuf;
	materialBuf.gMatID = mesh->Material_Index;

	if (mesh->Albedo)
	{
		materialBuf.gTexID |= ALBEDO_MAP;
		m_ForwardPS->SetShaderResourceView<gDiffuseMap>((ID3D11ShaderResourceView*)mesh->Albedo->TextureBufferPointer);
	}
	if (mesh->Normal)
	{
		materialBuf.gTexID |= NORMAL_MAP;
		m_ForwardPS->SetShaderResourceView<gNormalMap>((ID3D11ShaderResourceView*)mesh->Normal->TextureBufferPointer);
	}

	m_ForwardPS->SetConstantBuffer(materialBuf);

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

void ForwardPass::StaticUpdate(StaticData* data)
{
	LightData* lightData = data->mLightData;

	CB_Light lightBuf;
	lightBuf.gPointLightCount = lightData->gPointLightCount;
	lightBuf.gSpotLightCount = lightData->gSpotLightCount;

	lightBuf.gDirLights = *lightData->DirLights[0];

	for (UINT p = 0; p < lightBuf.gPointLightCount; p++)
	{
		lightBuf.gPointLights[p] = *lightData->PointLights[p];
	}
	for (UINT s = 0; s < lightBuf.gSpotLightCount; s++)
	{
		lightBuf.gSpotLights[s] = *lightData->SpotLights[s];
	}

	for (UINT m = 0; m < 5; m++)
	{
		lightBuf.gMaterials[m] = data->mMatData[m];
	}
	m_ForwardPS->SetConstantBuffer(lightBuf);
}
