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
#include "MathDefine.h"
#include "EngineData.h"
#include "OITPass.h"

#include "VertexDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "DepthStencilViewDefine.h"
#include "DepthStencilStateDefine.h"
#include "BlendStateDefine.h"
#include "ShaderResourceViewDefine.h"
#include "RenderTargetDefine.h"
#include "RenderBufferDefine.h"
#include "RasterizerStateDefine.h"
#include "DrawBufferDefine.h"


OITPass::OITPass()
{
	m_Multiple = 4;
}

OITPass::~OITPass()
{

}

void OITPass::Create(int width, int height)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	g_Factory->CreateRenderTarget<RT_OutPut>(&texDesc, nullptr, &rtvDesc, &srvDesc);

	D3D11_BUFFER_DESC structbufferDesc;
	structbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	structbufferDesc.ByteWidth = width * height * m_Multiple * 12;
	structbufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	structbufferDesc.CPUAccessFlags = 0;
	structbufferDesc.StructureByteStride = 12;
	structbufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_BUFFER_DESC rawbufferDesc;
	rawbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	rawbufferDesc.ByteWidth = width * height * m_Multiple;
	rawbufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	rawbufferDesc.CPUAccessFlags = 0;
	rawbufferDesc.StructureByteStride = 0;
	rawbufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	D3D11_SHADER_RESOURCE_VIEW_DESC structsrvDesc;
	structsrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	structsrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	structsrvDesc.Buffer.FirstElement = 0;
	structsrvDesc.Buffer.NumElements = width * height * m_Multiple;

	D3D11_SHADER_RESOURCE_VIEW_DESC rawsrvDesc;
	rawsrvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	rawsrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	rawsrvDesc.BufferEx.FirstElement = 0;
	rawsrvDesc.BufferEx.NumElements = width * height;
	rawsrvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;

	D3D11_UNORDERED_ACCESS_VIEW_DESC structuavDesc;
	structuavDesc.Format = DXGI_FORMAT_UNKNOWN;
	structuavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	structuavDesc.Buffer.FirstElement = 0;
	structuavDesc.Buffer.NumElements = width * height * m_Multiple;
	structuavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;

	D3D11_UNORDERED_ACCESS_VIEW_DESC rawuavDesc;
	rawuavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	rawuavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	rawuavDesc.Buffer.FirstElement = 0;
	rawuavDesc.Buffer.NumElements = width * height;
	rawuavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

	g_Factory->CreateRenderBuffer<RB_PieceLink>(&structbufferDesc, nullptr, nullptr, &structsrvDesc, &structuavDesc);
	g_Factory->CreateRenderBuffer<RB_FirstOffset>(&rawbufferDesc, nullptr, nullptr, &rawsrvDesc, &rawuavDesc);
}

void OITPass::Start(int width, int height)
{
	// Shader 설정..
	m_OITRenderVS = g_Shader->GetShader("OIT_VS");
	m_OITRenderPS = g_Shader->GetShader("OIT_PS");
	m_OITParticlePS = g_Shader->GetShader("OIT_Particle_PS");

	// Buffer 설정..
	m_QuadBD = g_Resource->GetDrawBuffer<DB_Quad>();

	// BackBuffer 설정..
	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();

	// DepthStencilView 설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();
	m_OutputDSV = g_Resource->GetDepthStencilView<DS_OutPut>()->Get();

	// Render Buffer 설정..
	m_PieceLinkRB = g_Resource->GetRenderBuffer<RB_PieceLink>();
	m_PieceLinkRB->SetResize();
	m_FirstOffsetRB = g_Resource->GetRenderBuffer<RB_FirstOffset>();
	m_FirstOffsetRB->SetResize();

	// UnorderedAccessView 설정..
	m_PieceLinkUAV = m_PieceLinkRB->GetUAV()->Get();
	m_FirstOffsetUAV = m_FirstOffsetRB->GetUAV()->Get();

	// Graphic State 설정..
	m_NoDepthStencilDSS = g_Resource->GetDepthStencilState<DSS_NoDepthStencil>()->Get();
	m_NoCullRS = g_Resource->GetRasterizerState<RS_NoCull>()->Get();

	// Shader Resource 설정..
	CB_OitFrame oitBuf;
	oitBuf.gFrameWidth = width;
	m_OITRenderPS->ConstantBufferCopy<CB_OitFrame>(&oitBuf);
	m_OITParticlePS->ConstantBufferCopy<CB_OitFrame>(&oitBuf);

	ShaderResourceView* backGround = g_Resource->GetShaderResourceView<RT_OutPut>();
	
	m_OITRenderPS->SetShaderResourceView<gPieceLinkBuffer>(m_PieceLinkRB->GetSRV()->Get());
	m_OITRenderPS->SetShaderResourceView<gFirstOffsetBuffer>(m_FirstOffsetRB->GetSRV()->Get());
	m_OITRenderPS->SetShaderResourceView<gBackGround>(backGround->Get());
}

void OITPass::OnResize(int width, int height)
{
	// BackBuffer 설정..
	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();

	// UnorderedAccessView 설정..
	m_PieceLinkUAV = m_PieceLinkRB->GetUAV()->Get();
	m_FirstOffsetUAV = m_FirstOffsetRB->GetUAV()->Get();

	// DepthStencilView 설정..
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();
	m_OutputDSV = g_Resource->GetDepthStencilView<DS_OutPut>()->Get();

	// Shader Resource 설정..
	CB_OitFrame oitBuf;
	oitBuf.gFrameWidth = width;
	m_OITRenderPS->ConstantBufferCopy<CB_OitFrame>(&oitBuf);
	m_OITParticlePS->ConstantBufferCopy<CB_OitFrame>(&oitBuf);

	ShaderResourceView* backGround = g_Resource->GetShaderResourceView<RT_OutPut>();

	m_OITRenderPS->SetShaderResourceView<gPieceLinkBuffer>(m_PieceLinkRB->GetSRV()->Get());
	m_OITRenderPS->SetShaderResourceView<gFirstOffsetBuffer>(m_FirstOffsetRB->GetSRV()->Get());
	m_OITRenderPS->SetShaderResourceView<gBackGround>(backGround->Get());
}

void OITPass::Release()
{

}

void OITPass::SetResize(int width, int height)
{
	m_PieceLinkRB->OnResize(width * height * m_Multiple * 12, width * height * m_Multiple);
	m_FirstOffsetRB->OnResize(width * height * m_Multiple, width * height);
}

void OITPass::BeginRender()
{
	g_Context->RSSetState(m_NoCullRS);
	g_Context->OMSetDepthStencilState(m_NoDepthStencilDSS, 0);
	g_Context->OMSetRenderTargets(0, 0, m_DefaltDSV);
	g_Context->OMSetBlendState(0, 0, 0xffffffff);

	UINT magicValue[1] = { 0xffffffff };
	g_Context->ClearUnorderedAccessViewUint(m_PieceLinkUAV, magicValue);
	g_Context->ClearUnorderedAccessViewUint(m_FirstOffsetUAV, magicValue);

	ID3D11UnorderedAccessView* pUAVs[2] = { m_PieceLinkUAV, m_FirstOffsetUAV };
	UINT initCounts[2] = { 0, 0 };
	g_Context->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL,
		nullptr, nullptr, 1, 2, pUAVs, initCounts);
}

void OITPass::RenderUpdate()
{
	g_Context->ClearRenderTargetView(m_MainRTV, reinterpret_cast<const float*>(&DXColors::DeepDarkGray));
	g_Context->ClearDepthStencilView(m_OutputDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	g_Context->OMSetRenderTargets(1, &m_MainRTV, m_OutputDSV);
	g_Context->OMSetDepthStencilState(nullptr, 0);

	m_OITRenderVS->Update();
	m_OITRenderPS->Update();

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, m_QuadBD->VB->GetAddress(), &m_QuadBD->Stride, &m_QuadBD->Offset);
	g_Context->IASetIndexBuffer(m_QuadBD->IB->Get(), DXGI_FORMAT_R32_UINT, 0);

	g_Context->DrawIndexed(m_QuadBD->IndexCount, 0, 0);

}
