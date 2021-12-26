#include "DirectDefine.h"
#include "RenderPassBase.h"
#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "BufferData.h"
#include "EngineData.h"
#include "DebugPass.h"

#include "MathDefine.h"
#include "ResourceFactoryBase.h"
#include "ResourceManagerBase.h"
#include "ShaderManagerBase.h"
#include "ConstantBufferDefine.h"
#include "BufferDataDefine.h"

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

	m_QuadBuffer = g_Resource->GetBuffer<BD_Line_Quad>();
	m_AxisBuffer = g_Resource->GetBuffer<BD_Line_Axis>();
	m_BoxBuffer = g_Resource->GetBuffer<BD_Line_Box>();
	m_CircleBuffer = g_Resource->GetBuffer<BD_Line_Circle>();
}

void DebugPass::OnResize(int width, int height)
{

}

void DebugPass::Release()
{

}

void DebugPass::Render(MeshData* mesh, GlobalData* global)
{
	CB_DebugObject object;
	object.gWorldViewProj = mesh->mWorld * global->mCamVP;

	switch (mesh->ObjType)
	{
	case OBJECT_TYPE::BASE:
	{
		m_DebugVS->ConstantBufferCopy(&object);

		m_DebugVS->Update();

		m_DebugPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);

		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
	}
		break;
	case OBJECT_TYPE::BONE:
	{
		object.gWorldViewProj = Matrix::CreateScale(0.1f) * mesh->mWorld * global->mCamVP;

		m_DebugVS->ConstantBufferCopy(&object);

		m_DebugVS->Update();

		m_DebugPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);

		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		BufferUpdate(DEBUG_TYPE::DEBUG_BOX);

		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);
	}
		break;
	case OBJECT_TYPE::CAMERA:
		break;
	case OBJECT_TYPE::PARTICLE:
	{
		ParticleData* particles = mesh->Particle_Data;

		float radius = particles->Area_Radius;
		Matrix world = mesh->mWorld;
		world._11 = radius; world._22 = radius; world._33 = radius;

		object.gWorldViewProj = world * global->mCamVP;

		m_DebugVS->ConstantBufferCopy(&object);

		m_DebugVS->Update();

		m_DebugPS->Update();

		BufferUpdate(DEBUG_TYPE::DEBUG_BOX);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		BufferUpdate(DEBUG_TYPE::DEBUG_AXIS);
		g_Context->DrawIndexed(m_DebugBuffer->IndexCount, 0, 0);

		Matrix invView = global->mCamInvView;
		Matrix viewproj = global->mCamVP;

		for (int i = 0; i < particles->Particle_Count; i++)
		{
			OneParticle* particle = particles->m_Particles[i];

			if (particle->Playing == false) continue;

			Matrix particleWorld = *particle->World * invView;
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
	g_Context->IASetVertexBuffers(0, 1, m_DebugBuffer->VB.GetAddressOf(), &m_DebugBuffer->Stride, &m_DebugBuffer->Offset);
	g_Context->IASetIndexBuffer(m_DebugBuffer->IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}
