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
#include "AlphaPass.h"

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
#include "RasterizerStateDefine.h"

AlphaPass::AlphaPass()
{

}

AlphaPass::~AlphaPass()
{

}

void AlphaPass::Create(int width, int height)
{


}

void AlphaPass::Start(int width, int height)
{
	// Shader 설정..
	m_ParticleVS = g_Shader->GetShader("Particle_VS");
	m_ParticlePS = g_Shader->GetShader("Particle_PS");

	// Graphic State 설정..
	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();

	m_AlphaBlendBS = g_Resource->GetBlendState<BS_AlphaBlend>()->Get();
	m_NoCullRS = g_Resource->GetRasterizerState<RS_NoCull>()->Get();
}

void AlphaPass::OnResize(int width, int height)
{
	// Graphic State 설정..
	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();
	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();
}

void AlphaPass::Release()
{

}

void AlphaPass::SetOption(UINT renderOption)
{
	if (renderOption & RENDER_OIT)
	{
		m_ParticlePS = g_Shader->GetShader("OIT_Particle_PS");
	}
	else
	{
		m_ParticlePS = g_Shader->GetShader("Particle_PS");
	}
}

void AlphaPass::BeginRender()
{
	g_Context->OMSetRenderTargets(1, &m_MainRTV, m_DefaltDSV);
	g_Context->OMSetBlendState(m_AlphaBlendBS, 0, 0xffffffff);
}

void AlphaPass::BufferUpdate(MeshData* mesh)
{
	m_IndexBuffer = (ID3D11Buffer*)(mesh->IB->IndexBufferPointer);
	m_VertexBuffer = (ID3D11Buffer*)(mesh->VB->VertexBufferPointer);

	m_IndexCount = mesh->IB->Count;
	m_Stride = mesh->VB->VertexDataSize;

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &m_Stride, &m_Offset);
	g_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void AlphaPass::RenderUpdate(MeshData* mesh, GlobalData* global)
{
	Matrix view = global->mCamView;
	Matrix invView = global->mCamInvView;
	Matrix viewproj = global->mCamVP;
	MaterialData* mat = mesh->Material_Data;

	// Buffer Update..
	BufferUpdate(mesh);

	switch (mesh->ObjType)
	{
	case OBJECT_TYPE::PARTICLE:
	{
		ParticleData* particles = mesh->Particle_Data;

		Matrix converseTM = Matrix::Identity;
		switch (particles->RenderType)
		{
		case PARTICLE_RENDER_OPTION::BILLBOARD:
			converseTM = invView;
			break;
		case PARTICLE_RENDER_OPTION::VERTICAL_BILLBOARD:
			converseTM = invView;
			converseTM._21 = 0; converseTM._22 = 1; converseTM._23 = 0;
			break;
		case PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD:
			converseTM = Matrix::CreateRotationX(3.1415926535f / 2.0f);
			break;
		case PARTICLE_RENDER_OPTION::MESH:
			break;
		default:
			break;
		}
		
		for (int i = 0; i < particles->Particle_Count; i++)
		{
			OneParticle* particle = particles->m_Particles[i];

			if (particle->Playing == false) continue;

			Matrix particleWorld = *particle->World * converseTM;

			particleWorld._41 = particle->World->_41;
			particleWorld._42 = particle->World->_42;
			particleWorld._43 = particle->World->_43;

			// Veretex Shader Update..
			CB_ParticleObject objectBuf;
			objectBuf.gWorld = particleWorld;
			objectBuf.gWorldView = particleWorld * view;
			objectBuf.gWorldViewProj = particleWorld * viewproj;
			objectBuf.gTexTransform = *particle->Tex;

			m_ParticleVS->ConstantBufferCopy(&objectBuf);

			m_ParticleVS->Update();

			CB_ParticleOption optionBuf;
			optionBuf.gColor = particle->Color;

			m_ParticlePS->ConstantBufferCopy(&optionBuf);

			// Pixel Shader Update..
			if (mat->Albedo)
			{
				m_ParticlePS->SetShaderResourceView<gDiffuseMap>((ID3D11ShaderResourceView*)mat->Albedo->TextureBufferPointer);
			}

			m_ParticlePS->Update();

			// Draw..
			g_Context->DrawIndexed(m_IndexCount, 0, 0);
		}

	}
	break;
	default:
		break;
	}
}
