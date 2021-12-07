#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "RenderTargetBase.h"
#include "BasicRenderTarget.h"
#include "VertexDefine.h"
#include "BufferData.h"
#include "EngineData.h"
#include "LightPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceBufferDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RenderTargetDefine.h"
#include "ViewPortDefine.h"
#include "BufferDataDefine.h"

LightPass::LightPass()
{

}

LightPass::~LightPass()
{

}

void LightPass::Create(int width, int height)
{
}

void LightPass::Start()
{
	// Shader 설정..
	m_LightVS = g_Shader->GetShader("LightVS");
	m_LightPS = g_Shader->GetShader("LightPS");

	// Buffer 설정..
	m_ScreenBuffer = g_Resource->GetBuffer<BD_FullScreen>();

	// BackBuffer 설정..
	m_BackBuffer = g_Resource->GetMainRenderTarget();
	m_BackBufferRTV = m_BackBuffer->GetRTV();
	m_BackBufferSRV = m_BackBuffer->GetSRV();

	// ViewPort 설정..
	m_ScreenViewport = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	// DepthStencilView 설정..
	m_DepthStencilView = g_Resource->GetDepthStencilView<DSV_Defalt>()->GetDSV();

	// Multi RenderTarget 설정..
	m_AlbedoRT = g_Resource->GetRenderTarget<RT_Deffered_Albedo>();
	m_NormalRT = g_Resource->GetRenderTarget<RT_Deffered_Normal>();
	m_PositionRT = g_Resource->GetRenderTarget<RT_Deffered_Position>();
	m_ShadowRT = g_Resource->GetRenderTarget<RT_Deffered_Shadow>();
	m_SSAORT = g_Resource->GetRenderTarget<RT_SSAO>();

	// ShaderResource 설정..
	m_LightPS->SetShaderResourceView<gAlbedoRT>(m_AlbedoRT->GetSRV());
	m_LightPS->SetShaderResourceView<gNormalRT>(m_NormalRT->GetSRV());
	m_LightPS->SetShaderResourceView<gPositionRT>(m_PositionRT->GetSRV());
	m_LightPS->SetShaderResourceView<gShadowRT>(m_ShadowRT->GetSRV());
	m_LightPS->SetShaderResourceView<gSSAOMap>(m_SSAORT->GetSRV());
}

void LightPass::OnResize(int width, int height)
{
	// BackBuffer RenderTargetView 재설정..
	m_BackBufferRTV = m_BackBuffer->GetRTV();
	
	// BackBuffer ShaderResourceView 재설정..
	m_BackBufferSRV = m_BackBuffer->GetSRV();
	
	// DepthStencilView 재설성..
	m_DepthStencilView = m_DSV->GetDSV();
	
	// ShaderResource 재설정..
	m_LightPS->SetShaderResourceView<gAlbedoRT>(m_AlbedoRT->GetSRV());
	m_LightPS->SetShaderResourceView<gNormalRT>(m_NormalRT->GetSRV());
	m_LightPS->SetShaderResourceView<gPositionRT>(m_PositionRT->GetSRV());
	m_LightPS->SetShaderResourceView<gShadowRT>(m_ShadowRT->GetSRV());
	m_LightPS->SetShaderResourceView<gSSAOMap>(m_SSAORT->GetSRV());
}

void LightPass::Release()
{

}

void LightPass::BeginRender()
{
	g_Context->OMSetRenderTargets(1, &m_BackBufferRTV, m_DepthStencilView);
	g_Context->ClearRenderTargetView(m_BackBufferRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->RSSetViewports(1, m_ScreenViewport);
}

void LightPass::Render(GlobalData* global)
{
	Matrix view = *global->mViewMX;
	Matrix proj = *global->mProj;
	Vector3 eye(view._41, view._42, view._43);
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

	CB_LightSub lightsubBuf;
	lightsubBuf.gEyePosW = -eye;
	//lightsubBuf.gViewProjTex = view * proj * Tex;

	m_LightPS->SetConstantBuffer(lightBuf);
	m_LightPS->SetConstantBuffer(lightsubBuf);

	// Vertex Shader Update..
	m_LightVS->Update();

	// Pixel Shader Update..
	m_LightPS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_ScreenBuffer->VB.GetAddressOf(), &m_ScreenBuffer->Stride, &m_ScreenBuffer->Offset);
	g_Context->IASetIndexBuffer(m_ScreenBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_ScreenBuffer->IndexCount, 0, 0);
}
