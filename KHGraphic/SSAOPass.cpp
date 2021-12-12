#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "Texture2D.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "VertexDefine.h"
#include "BufferData.h"
#include "EngineData.h"
#include "ConstantBufferDefine.h"
#include "SSAOPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ShaderResourceBufferDefine.h"
#include "DepthStencilStateDefine.h"
#include "DepthStencilViewDefine.h"
#include "RenderTargetDefine.h"
#include "ViewPortDefine.h"
#include "BufferDataDefine.h"
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
	// SSAO ViewPort ����..
	g_Factory->CreateViewPort<VP_SSAO>(0.0f, 0.0f, 0.5f, 0.5f, (float)width, (float)height);

	// Constant Buffer Update..
	m_BlurOption.gTexelSize = DirectX::SimpleMath::Vector2(1.0f / (float)width, 1.0f / (float)height);
	
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

	// RenderTarget ����..
	g_Factory->CreateRenderTarget<RT_SSAO_Main>(&texDesc, nullptr, nullptr);
	g_Factory->CreateRenderTarget<RT_SSAO_Blur>(&texDesc, nullptr, nullptr);

	// OffsetVector ����..
	SetOffsetVectors();

	// RandomVectorTexture ����..
	SetRandomVectorTexture();

	// Frustum ����..
	SetFrustumFarCorners(width, height);
}

void SSAOPass::Start()
{
	// Shader ����..
	m_SsaoVS = g_Shader->GetShader("SSAOVS");
	m_SsaoPS = g_Shader->GetShader("SSAOPS");
	m_BlurVS = g_Shader->GetShader("SSAOBlurVS");
	m_BlurPS = g_Shader->GetShader("SSAOBlurPS");

	// Buffer ����..
	m_SsaoBuffer = g_Resource->GetBuffer<BD_SSAOScreen>();

	// ViewPort ����..
	m_SsaoViewport = g_Resource->GetViewPort<VP_SSAO>()->Get();

	// RenderTarget ����..
	m_SsaoRT = g_Resource->GetRenderTarget<RT_SSAO_Main>();
	m_SsaoBlurRT = g_Resource->GetRenderTarget<RT_SSAO_Blur>();
	m_SsaoRTV = m_SsaoRT->GetRTV();
	m_SsaoBlurRTV = m_SsaoBlurRT->GetRTV();

	// Resource RenderTarget ����..
	m_DepthRT = g_Resource->GetRenderTarget<RT_Deffered_Depth>();

	// ShaderResource ����..
	ID3D11ShaderResourceView* randomVecMap = g_Resource->GetShaderResourceView<gRandomVecMap>()->Get();
	
	m_SsaoPS->SetShaderResourceView<gDepthMap>(m_DepthRT->GetSRV());
	m_SsaoPS->SetShaderResourceView<gRandomVecMap>(randomVecMap);
	m_BlurPS->SetShaderResourceView<gDepthMap>(m_DepthRT->GetSRV());
}

void SSAOPass::OnResize(int width, int height)
{
	// Frustum �缳��..
	SetFrustumFarCorners(width, height);

	// RenderTarget �缳��..
	m_SsaoRTV = m_SsaoRT->GetRTV();
	m_SsaoBlurRTV = m_SsaoBlurRT->GetRTV();

	// ShaderResource �缳��..
	ID3D11ShaderResourceView* randomVecMap = g_Resource->GetShaderResourceView<gRandomVecMap>()->Get();

	m_SsaoPS->SetShaderResourceView<gDepthMap>(m_DepthRT->GetSRV());
	m_SsaoPS->SetShaderResourceView<gRandomVecMap>(randomVecMap);
	m_BlurPS->SetShaderResourceView<gDepthMap>(m_DepthRT->GetSRV());
}

void SSAOPass::Release()
{

}

void SSAOPass::BeginRender()
{
	g_Context->OMSetBlendState(0, 0, 0xffffffff);
	g_Context->OMSetRenderTargets(1, &m_SsaoRTV, 0);
	g_Context->ClearRenderTargetView(m_SsaoRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->RSSetViewports(1, m_SsaoViewport);
}

void SSAOPass::Render(GlobalData* global)
{
	CB_SsaoObject objectBuf;
	objectBuf.gViewToTexSpace = *global->mCamPT;

	m_SsaoPS->SetConstantBuffer(objectBuf);

	// Vertex Shader Update..
	m_SsaoVS->Update();

	// Pixel Shader Update..
	m_SsaoPS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_SsaoBuffer->VB.GetAddressOf(), &m_SsaoBuffer->Stride, &m_SsaoBuffer->Offset);
	g_Context->IASetIndexBuffer(m_SsaoBuffer->IB.Get(), DXGI_FORMAT_R16_UINT, 0);

	g_Context->DrawIndexed(m_SsaoBuffer->IndexCount, 0, 0);
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
		g_Context->OMSetRenderTargets(1, &m_SsaoRTV, 0);
		g_Context->ClearRenderTargetView(m_SsaoRTV, reinterpret_cast<const float*>(&DXColors::Black));

		m_BlurPS->SetShaderResourceView<gInputMap>(m_SsaoRT->GetSRV());
	}
	else
	{
		g_Context->OMSetRenderTargets(1, &m_SsaoBlurRTV, 0);
		g_Context->ClearRenderTargetView(m_SsaoBlurRTV, reinterpret_cast<const float*>(&DXColors::Black));

		m_BlurPS->SetShaderResourceView<gInputMap>(m_SsaoBlurRT->GetSRV());
	}

	// Vertex Shader Update..
	m_BlurVS->Update();

	// Pixel Shader Update..
	m_BlurPS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_SsaoBuffer->VB.GetAddressOf(), &m_SsaoBuffer->Stride, &m_SsaoBuffer->Offset);
	g_Context->IASetIndexBuffer(m_SsaoBuffer->IB.Get(), DXGI_FORMAT_R16_UINT, 0);

	g_Context->DrawIndexed(m_SsaoBuffer->IndexCount, 0, 0);
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

		// OffsetVector Constant Buffer Data ����..
		XMStoreFloat4(&option.gOffsetVectors[i], v);
	}
	
	// SSAO Option Constant Buffer Update..
	m_SsaoPS->SetConstantBuffer(option);
}

void SSAOPass::SetRandomVectorTexture()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 256;
	texDesc.Height = 256;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.SysMemPitch = 256 * sizeof(XMCOLOR);

	std::mt19937 randEngine;
	randEngine.seed(std::random_device()());
	std::uniform_real_distribution<float> randF(0.0f, 1.0f);

	XMCOLOR color[256 * 256];
	for (int i = 0; i < 256 * 256; ++i)
	{
		color[i] = XMCOLOR(randF(randEngine), randF(randEngine), randF(randEngine), 0.0f);
	}

	initData.pSysMem = color;
	initData.SysMemPitch = 256 * sizeof(XMCOLOR);

	// RandomVectorMap ����..
	g_Factory->CreateShaderResourceView<gRandomVecMap>(&texDesc, nullptr);
}

void SSAOPass::SetFrustumFarCorners(int width, int height)
{
	CB_SsaoFrustum frustum;
	
	float aspect = (float)width / (float)height;

	float fovY = 0.25f * 3.141592f;
	float farZ = 4000.0f;
	float halfHeight = farZ * tanf(0.5f * fovY);
	float halfWidth = aspect * halfHeight;

	frustum.gFrustumCorners[0] = XMFLOAT4(-halfWidth, -halfHeight, farZ, 0.0f);
	frustum.gFrustumCorners[1] = XMFLOAT4(-halfWidth, +halfHeight, farZ, 0.0f);
	frustum.gFrustumCorners[2] = XMFLOAT4(+halfWidth, +halfHeight, farZ, 0.0f);
	frustum.gFrustumCorners[3] = XMFLOAT4(+halfWidth, -halfHeight, farZ, 0.0f);

	// FrustumCorner Constant Buffer Data ����..
	m_SsaoVS->SetConstantBuffer(frustum);
}