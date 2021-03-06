#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "Buffer.h"
#include "Texture2D.h"
#include "DrawBuffer.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "VertexDefine.h"
#include "EngineData.h"
#include "ConstantBufferDefine.h"
#include "SSAOPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ShaderResourceViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RenderTargetDefine.h"
#include "ViewPortDefine.h"
#include "DrawBufferDefine.h"
#include <random>
#include <DirectXPackedVector.h>

using namespace DirectX::PackedVector;

SSAOPass::SSAOPass()
{

}

SSAOPass::~SSAOPass()
{

}

void SSAOPass::Create(int width, int height)
{
	// SSAO ViewPort 생성..
	g_Factory->CreateViewPort<VP_SSAO>(0.0f, 0.0f, 0.5f, 0.5f, (float)width, (float)height);

	// Texture 2D
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width / 2;
	texDesc.Height = height / 2;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// RenderTarget 생성..
	g_Factory->CreateRenderTarget<RT_SSAO_Main>(&texDesc, nullptr, nullptr);
	g_Factory->CreateRenderTarget<RT_SSAO_Blur>(&texDesc, nullptr, nullptr);

}

void SSAOPass::Start(int width, int height)
{
	// Shader 설정..
	m_SsaoVS = g_Shader->GetShader("SSAO_VS");
	m_SsaoPS = g_Shader->GetShader("SSAO_PS");
	m_BlurVS = g_Shader->GetShader("SSAOBlur_VS");
	m_BlurPS = g_Shader->GetShader("SSAOBlur_PS");

	// Buffer 설정..
	m_SsaoDB = g_Resource->GetDrawBuffer<DB_SSAO>();

	// ViewPort 설정..
	m_SsaoVP = g_Resource->GetViewPort<VP_SSAO>()->Get();
	
	// Constant Buffer Update..
	m_BlurOption.gTexelSize = DirectX::SimpleMath::Vector2(1.0f / m_SsaoVP->Width, 1.0f / m_SsaoVP->Height);

	// RenderTarget 설정..
	m_SsaoRT = g_Resource->GetRenderTarget<RT_SSAO_Main>();
	m_SsaoRT->SetRatio(0.5f, 0.5f);
	m_SsaoBlurRT = g_Resource->GetRenderTarget<RT_SSAO_Blur>();
	m_SsaoBlurRT->SetRatio(0.5f, 0.5f);

	m_SsaoRTV = m_SsaoRT->GetRTV()->Get();
	m_SsaoSRV = m_SsaoRT->GetSRV()->Get();
	m_SsaoBlurRTV = m_SsaoBlurRT->GetRTV()->Get();
	m_SsaoBlurSRV = m_SsaoBlurRT->GetSRV()->Get();

	// OffsetVector 설정..
	SetOffsetVectors();

	// Frustum 설정..
	SetFrustumFarCorners(width, height);

	// ShaderResource 설정..
	ShaderResourceView* depthMap = g_Resource->GetShaderResourceView<RT_Deffered_Depth>();
	
	m_SsaoPS->SetShaderResourceView<gDepthMap>(depthMap->Get());
	m_BlurPS->SetShaderResourceView<gDepthMap>(depthMap->Get());
}

void SSAOPass::OnResize(int width, int height)
{
	// Frustum 재설정..
	SetFrustumFarCorners(width, height);

	// Constant Buffer Update..
	m_BlurOption.gTexelSize = DirectX::SimpleMath::Vector2(1.0f / m_SsaoVP->Width, 1.0f / m_SsaoVP->Height);

	// RenderTarget Resource 재설정..
	m_SsaoRTV = m_SsaoRT->GetRTV()->Get();
	m_SsaoSRV = m_SsaoRT->GetSRV()->Get();
	m_SsaoBlurRTV = m_SsaoBlurRT->GetRTV()->Get();
	m_SsaoBlurSRV = m_SsaoBlurRT->GetSRV()->Get();

	// ShaderResource 설정..
	ShaderResourceView* depthMap = g_Resource->GetShaderResourceView<RT_Deffered_Depth>();

	m_SsaoPS->SetShaderResourceView<gDepthMap>(depthMap->Get());
	m_BlurPS->SetShaderResourceView<gDepthMap>(depthMap->Get());
}

void SSAOPass::Release()
{

}

void SSAOPass::SetOption(UINT renderOption)
{
	g_Context->ClearRenderTargetView(m_SsaoRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
}

void SSAOPass::BeginRender()
{
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->OMSetRenderTargets(1, &m_SsaoRTV, 0);
	g_Context->ClearRenderTargetView(m_SsaoRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->RSSetViewports(1, m_SsaoVP);
}

void SSAOPass::Render(GlobalData* global)
{
	CB_SsaoObject objectBuf;
	objectBuf.gViewToTexSpace = global->mCamPT;

	m_SsaoPS->ConstantBufferCopy(&objectBuf);

	// Vertex Shader Update..
	m_SsaoVS->Update();

	// Pixel Shader Update..
	m_SsaoPS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_SsaoDB->VB->GetAddress(), &m_SsaoDB->Stride, &m_SsaoDB->Offset);
	g_Context->IASetIndexBuffer(m_SsaoDB->IB->Get(), DXGI_FORMAT_R16_UINT, 0);

	g_Context->DrawIndexed(m_SsaoDB->IndexCount, 0, 0);
}

void SSAOPass::BlurRender(int blurCount)
{
	for (int i = 0; i < blurCount; i++)
	{
		BlurRender(true);
		BlurRender(false);
	}
}

void SSAOPass::BlurRender(bool horizon)
{
	// Blur Type..
	m_BlurOption.gHorizon = horizon;

	if (horizon)
	{
		g_Context->OMSetRenderTargets(1, &m_SsaoBlurRTV, 0);
		g_Context->ClearRenderTargetView(m_SsaoBlurRTV, reinterpret_cast<const float*>(&DXColors::Red));

		m_BlurPS->SetShaderResourceView<gInputMap>(m_SsaoSRV);
	}
	else
	{
		g_Context->OMSetRenderTargets(1, &m_SsaoRTV, 0);
		g_Context->ClearRenderTargetView(m_SsaoRTV, reinterpret_cast<const float*>(&DXColors::Red));

		m_BlurPS->SetShaderResourceView<gInputMap>(m_SsaoBlurSRV);
	}

	m_BlurPS->ConstantBufferCopy(&m_BlurOption);

	// Vertex Shader Update..
	m_BlurVS->Update();

	// Pixel Shader Update..
	m_BlurPS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_SsaoDB->VB->GetAddress(), &m_SsaoDB->Stride, &m_SsaoDB->Offset);
	g_Context->IASetIndexBuffer(m_SsaoDB->IB->Get(), DXGI_FORMAT_R16_UINT, 0);

	g_Context->DrawIndexed(m_SsaoDB->IndexCount, 0, 0);
}

void SSAOPass::SetOffsetVectors()
{
	CB_SsaoOption option;

	// 8 cube corners
	XMFLOAT4 m_Offsets[14];
	m_Offsets[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	m_Offsets[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	m_Offsets[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	m_Offsets[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	m_Offsets[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	m_Offsets[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	m_Offsets[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	m_Offsets[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	m_Offsets[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_Offsets[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	m_Offsets[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	m_Offsets[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	m_Offsets[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	m_Offsets[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	std::mt19937 randEngine;
	randEngine.seed(std::random_device()());
	std::uniform_real_distribution<float> randF(0.25f, 1.0f);
	for (int i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		float s = randF(randEngine);
		
		XMVECTOR v = s * XMVector4Normalize(XMLoadFloat4(&m_Offsets[i]));

		// OffsetVector Constant Buffer Data 삽입..
		XMStoreFloat4(&option.gOffsetVectors[i], v);
	}

	// SSAO Option Constant Buffer Update..
	m_SsaoPS->ConstantBufferCopy(&option);
}

void SSAOPass::SetFrustumFarCorners(int width, int height)
{
	CB_SsaoFrustum frustum;
	
	float aspect = (float)width / (float)height;

	float fovY = 0.25f * 3.1415926535f;
	float farZ = 4000.0f;
	float halfHeight = farZ * tanf(0.5f * fovY);
	float halfWidth = aspect * halfHeight;

	frustum.gFrustumCorners[0] = XMFLOAT4(-halfWidth, -halfHeight, farZ, 0.0f);
	frustum.gFrustumCorners[1] = XMFLOAT4(-halfWidth, +halfHeight, farZ, 0.0f);
	frustum.gFrustumCorners[2] = XMFLOAT4(+halfWidth, +halfHeight, farZ, 0.0f);
	frustum.gFrustumCorners[3] = XMFLOAT4(+halfWidth, -halfHeight, farZ, 0.0f);

	// FrustumCorner Constant Buffer Data 삽입..
	m_SsaoVS->ConstantBufferCopy(&frustum);
}
