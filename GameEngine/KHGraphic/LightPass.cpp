#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "Buffer.h"
#include "DrawBuffer.h"
#include "Texture2D.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "VertexDefine.h"
#include "EngineData.h"
#include "LightPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "ShaderResourceViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RenderTargetDefine.h"
#include "ViewPortDefine.h"
#include "DrawBufferDefine.h"

LightPass::LightPass()
{

}

LightPass::~LightPass()
{

}

void LightPass::Create(int width, int height)
{
}

void LightPass::Start(int width, int height)
{
	// Shader 설정..
	m_LightVS = g_Shader->GetShader("Light_VS");
	//m_LightPS = g_Shader->GetShader("Light_PS_Option0");
	m_LightPS = g_Shader->GetShader("Light_PS_Option4");

	// Buffer 설정..
	m_QuadBD = g_Resource->GetDrawBuffer<DB_Quad>();

	// BackBuffer 설정..
	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();
	m_OitRTV = g_Resource->GetRenderTarget<RT_OIT>()->GetRTV()->Get();

	// ViewPort 설정..
	m_ScreenVP = g_Resource->GetViewPort<VP_FullScreen>()->Get();

	// DepthStencilView 설정..
	m_LightDSV = g_Resource->GetDepthStencilView<DS_Light>()->Get();

	// Multi RenderTarget 설정..
	m_AlbedoRT = g_Resource->GetRenderTarget<RT_Deffered_Albedo>();
	m_NormalRT = g_Resource->GetRenderTarget<RT_Deffered_Normal>();
	m_PositionRT = g_Resource->GetRenderTarget<RT_Deffered_Position>();
	m_ShadowRT = g_Resource->GetRenderTarget<RT_Deffered_Shadow>();

	// ShaderResource 설정..
	m_LightPS->SetShaderResourceView<gAlbedoRT>(m_AlbedoRT->GetSRV()->Get());
	m_LightPS->SetShaderResourceView<gNormalRT>(m_NormalRT->GetSRV()->Get());
	m_LightPS->SetShaderResourceView<gPositionRT>(m_PositionRT->GetSRV()->Get());
	m_LightPS->SetShaderResourceView<gShadowRT>(m_ShadowRT->GetSRV()->Get());

	// Sub Resource 설정..
	ShaderResourceView* ssaoSRV = g_Resource->GetShaderResourceView<RT_SSAO_Main>();
	ShaderResourceView* shadowSRV = g_Resource->GetShaderResourceView<DS_Shadow>();

	// ShaderResource 설정..
	m_LightPS->SetShaderResourceView<gShadowMap>(shadowSRV->Get());
	m_LightPS->SetShaderResourceView<gSsaoMap>(ssaoSRV->Get());
}

void LightPass::OnResize(int width, int height)
{
	// BackBuffer RenderTargetView 재설정..
	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();
	m_OitRTV = g_Resource->GetRenderTarget<RT_OIT>()->GetRTV()->Get();

	// DepthStencilView 재설정..
	m_LightDSV = g_Resource->GetDepthStencilView<DS_Light>()->Get();
	
	// ShaderResource 재설정..
	m_LightPS->SetShaderResourceView<gAlbedoRT>(m_AlbedoRT->GetSRV()->Get());
	m_LightPS->SetShaderResourceView<gNormalRT>(m_NormalRT->GetSRV()->Get());
	m_LightPS->SetShaderResourceView<gPositionRT>(m_PositionRT->GetSRV()->Get());
	m_LightPS->SetShaderResourceView<gShadowRT>(m_ShadowRT->GetSRV()->Get());

	// Sub Resource 재설정..
	ShaderResourceView* ssaoSRV = g_Resource->GetShaderResourceView<RT_SSAO_Main>();
	ShaderResourceView* shadowSRV = g_Resource->GetShaderResourceView<DS_Shadow>();

	m_LightPS->SetShaderResourceView<gShadowMap>(shadowSRV->Get());
	m_LightPS->SetShaderResourceView<gSsaoMap>(ssaoSRV->Get());
}

void LightPass::Release()
{

}

void LightPass::SetOption(const char* shaderName)
{
	m_LightPS = g_Shader->GetShader(shaderName);

	Reset();
}

void LightPass::Reset()
{
	g_Context->ClearRenderTargetView(m_MainRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));

	// ShaderResource 재설정..
	m_LightPS->SetShaderResourceView<gAlbedoRT>(m_AlbedoRT->GetSRV()->Get());
	m_LightPS->SetShaderResourceView<gNormalRT>(m_NormalRT->GetSRV()->Get());
	m_LightPS->SetShaderResourceView<gPositionRT>(m_PositionRT->GetSRV()->Get());
	m_LightPS->SetShaderResourceView<gShadowRT>(m_ShadowRT->GetSRV()->Get());

	// Sub Resource 재설정..
	ShaderResourceView* ssaoSRV = g_Resource->GetShaderResourceView<RT_SSAO_Main>();
	ShaderResourceView* shadowSRV = g_Resource->GetShaderResourceView<DS_Shadow>();

	m_LightPS->SetShaderResourceView<gShadowMap>(shadowSRV->Get());
	m_LightPS->SetShaderResourceView<gSsaoMap>(ssaoSRV->Get());
}

void LightPass::BeginRender()
{
	//g_Context->OMSetRenderTargets(1, &m_MainRTV, m_LightDSV);
	g_Context->OMSetRenderTargets(1, &m_OitRTV, m_LightDSV);
	g_Context->ClearDepthStencilView(m_LightDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//g_Context->ClearRenderTargetView(m_MainRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearRenderTargetView(m_OitRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->RSSetViewports(1, m_ScreenVP);
}

void LightPass::Render(GlobalData* global)
{
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

	for (UINT m = 0; m < global->mMatData.size(); m++)
	{
		lightBuf.gMaterials[m] = *global->mMatData[m];
	}

	CB_LightSub lightsubBuf;
	lightsubBuf.gEyePosW = global->mCamPos;
	lightsubBuf.gViewProjTex = global->mCamVPT;

	m_LightPS->ConstantBufferCopy(&lightBuf);
	m_LightPS->ConstantBufferCopy(&lightsubBuf);

	// Vertex Shader Update..
	m_LightVS->Update();

	// Pixel Shader Update..
	m_LightPS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_QuadBD->VB->GetAddress(), &m_QuadBD->Stride, &m_QuadBD->Offset);
	g_Context->IASetIndexBuffer(m_QuadBD->IB->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_QuadBD->IndexCount, 0, 0);
	g_Context->ClearDepthStencilView(m_LightDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
