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
#include "RenderTarget.h"
#include "EngineData.h"
#include "DebugPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "DrawBufferDefine.h"
#include "DepthStencilViewDefine.h"

DebugPass::DebugPass()
{

}

DebugPass::~DebugPass()
{

}

void DebugPass::Create(int width, int height)
{

}

void DebugPass::Start(int width, int height)
{
	m_DebugVS = g_Shader->GetShader("Debug_VS");
	m_DebugPS = g_Shader->GetShader("Debug_PS");

	m_QuadBuffer = g_Resource->GetDrawBuffer<DB_Line_Quad>();
	m_AxisBuffer = g_Resource->GetDrawBuffer<DB_Line_Axis>();
	m_BoxBuffer = g_Resource->GetDrawBuffer<DB_Line_Box>();
	m_CircleBuffer = g_Resource->GetDrawBuffer<DB_Line_Circle>();

	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();

	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();
}

void DebugPass::OnResize(int width, int height)
{
	m_MainRTV = g_Resource->GetMainRenderTarget()->GetRTV()->Get();

	m_DefaltDSV = g_Resource->GetDepthStencilView<DS_Defalt>()->Get();
}

void DebugPass::Release()
{

}

void DebugPass::BeginRender()
{
	g_Context->RSSetState(0);
	g_Context->OMSetRenderTargets(1, &m_MainRTV, m_DefaltDSV);
}

void DebugPass::Render(MeshData* mesh, GlobalData* global)
{
	CB_DebugObject object;
	Matrix world = mesh->mWorld;
	Matrix invView = global->mCamInvView;
	Matrix viewproj = global->mCamVP;

	switch (mesh->ObjType)
	{
	case OBJECT_TYPE::BASE:
	{
		object.gWorldViewProj = world * viewproj;
		
		m_DebugVS->ConstantBufferCopy(&object);

		m_DebugVS->Update();

		m_DebugPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);

		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
	}
		break;
	case OBJECT_TYPE::BONE:
	{
		object.gWorldViewProj = world * viewproj;

		m_DebugVS->ConstantBufferCopy(&object);

		m_DebugVS->Update();

		m_DebugPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);

		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		BufferUpdate(DEBUG_TYPE::DEBUG_BOX);

		object.gWorldViewProj = Matrix::CreateScale(0.05f) * world * viewproj;

		m_DebugVS->ConstantBufferCopy(&object);

		m_DebugVS->Update();

		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
	}
		break;
	case OBJECT_TYPE::CAMERA:
		break;
	case OBJECT_TYPE::PARTICLE:
	{
		ParticleData* particles = mesh->Particle_Data;

		Vector3 radius = particles->Area_Radius;
		
		world._11 = radius.x; world._22 = radius.y; world._33 = radius.z;

		object.gWorldViewProj = world * viewproj;

		m_DebugVS->ConstantBufferCopy(&object);

		m_DebugVS->Update();

		m_DebugPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_CIRCLE);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);


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

			object.gWorldViewProj = particleWorld * viewproj;

			m_DebugVS->ConstantBufferCopy(&object); 
			m_DebugVS->Update();

			BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);
			g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

			BufferUpdate(DEBUG_TYPE::DEBUG_QUAD);
			g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
		}
	}
		break;
	default:
		return;
	}
}

void DebugPass::BufferUpdate(DEBUG_TYPE type)
{
	switch (type)
	{
	case DEBUG_AXIS:
		m_DebugBuffer = m_AxisBuffer;
		break;
	case DEBUG_BOX:
		m_DebugBuffer = m_BoxBuffer;
		break;
	case DEBUG_CIRCLE:
		m_DebugBuffer = m_CircleBuffer;
		break;
	case DEBUG_QUAD:
		m_DebugBuffer = m_QuadBuffer;
		break;
	case DEBUG_CAMERA:
		break;
	case DEBUG_LIGHT:
		break;
	default:
		break;
	}

	g_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	g_Context->IASetVertexBuffers(0, 1, m_DebugBuffer->VB->GetAddress(), &m_DebugBuffer->Stride, &m_DebugBuffer->Offset);
	g_Context->IASetIndexBuffer(m_DebugBuffer->IB->Get(), DXGI_FORMAT_R32_UINT, 0);
}
