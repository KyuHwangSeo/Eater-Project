#include "DirectDefine.h"
#include "D3D11GraphicBase.h"
#include "GraphicState.h"
#include "GraphicView.h"
#include "Buffer.h"
#include "DrawBuffer.h"
#include "Texture2D.h"
#include "DepthStencil.h"
#include "ShaderManagerBase.h"
#include "ResourceFactory.h"
#include "ResourceManager.h"
#include "RenderPassBase.h"
#include "RenderManager.h"

#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

#include "MathDefine.h"
#include "ConstantBufferDefine.h"

#include "ShadowPass.h"
#include "DeferredPass.h"
#include "LightPass.h"
#include "SSAOPass.h"
#include "AlphaPass.h"
#include "OITPass.h"
#include "DebugPass.h"
#include "VertexDefine.h"

RenderManager::RenderManager(ID3D11Graphic* graphic, IGraphicResourceFactory* factory, IGraphicResourceManager* resource, IShaderManager* shader)
{
	// Rendering Initialize..
	RenderPassBase::Initialize(graphic->GetContext(), factory, resource, shader);

	m_SwapChain = graphic->GetSwapChain();

	m_Deferred = new DeferredPass();
	m_Light = new LightPass();
	m_Shadow = new ShadowPass();
	m_SSAO = new SSAOPass();
	m_Alpha = new AlphaPass();
	m_OIT = new OITPass();
	m_Debug = new DebugPass();

	m_RenderPassList.push_back(m_Deferred);
	m_RenderPassList.push_back(m_Light);
	m_RenderPassList.push_back(m_Shadow);
	m_RenderPassList.push_back(m_SSAO);
	m_RenderPassList.push_back(m_Alpha);
	m_RenderPassList.push_back(m_OIT);
	m_RenderPassList.push_back(m_Debug);
}

RenderManager::~RenderManager()
{

}

void RenderManager::Initialize(int width, int height)
{
	// Render Pass Resource Create..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->Create(width, height);
	}

	// Render Pass Resource Set..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->Start(width, height);
	}

	// Render Option ?ʱ? ????..
	UINT startOption = RENDER_DEBUG | RENDER_GAMMA_CORRECTION | RENDER_SHADOW | RENDER_SSAO | RENDER_OIT;
	RenderSetting(startOption);
}

void RenderManager::Release()
{
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		RELEASE_COM(renderPass);
	}

	m_RenderPassList.clear();
}

void RenderManager::RenderSetting(UINT& renderOption)
{
	// ???? ?????? ???? ?ɼ??? ?ٲ??? ?ʾҴٸ? ó?????? ?ʴ´?..
	if (renderOption == m_RenderOption) return;

	m_RenderOption = renderOption;

	// Render Option?? ???? Render Setting..
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->SetOption(renderOption);
	}
}

void RenderManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	/// Deferred Render..
	m_Deferred->BeginRender();

	while (meshList->size() != 0)
	{
		MeshData* mesh = meshList->front();

		switch (mesh->ObjType)
		{
		case OBJECT_TYPE::BASE:
		case OBJECT_TYPE::SKINNING:
		case OBJECT_TYPE::TERRAIN:
		case OBJECT_TYPE::PARTICLE:
			m_Deferred->RenderUpdate(mesh, global);
			break;
		}

		meshList->pop();
	}
}

void RenderManager::ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	if (m_RenderOption & RENDER_SHADOW)
	{
		m_Shadow->BeginRender();

		while (meshList->size() != 0)
		{
			MeshData* mesh = meshList->front();

			switch (mesh->ObjType)
			{
			case OBJECT_TYPE::BASE:
			case OBJECT_TYPE::SKINNING:
			case OBJECT_TYPE::TERRAIN:
				m_Shadow->Update(mesh, global);
				m_Shadow->Render(mesh);
				break;
			}

			meshList->pop();
		}
	}
}

void RenderManager::SSAORender(GlobalData* global)
{
	if (m_RenderOption & RENDER_SSAO)
	{
		m_SSAO->BeginRender();

		m_SSAO->Render(global);
		m_SSAO->BlurRender(4);
	}
}

void RenderManager::AlphaRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	if (m_RenderOption & RENDER_OIT)
	{
		m_OIT->BeginRender();
	}
	else
	{
		m_Alpha->BeginRender();
	}

	while (meshList->size() != 0)
	{
		MeshData* mesh = meshList->front();

		switch (mesh->ObjType)
		{
		case OBJECT_TYPE::PARTICLE:
			m_Alpha->RenderUpdate(mesh, global);
			break;
		}

		meshList->pop();
	}

	if (m_RenderOption & RENDER_OIT)
	{
		m_OIT->RenderUpdate();
	}
}

void RenderManager::UIRender(std::queue<MeshData*>* meshList, GlobalData* global)
{

}

void RenderManager::LightRender(GlobalData* global)
{
	m_Light->BeginRender();
	m_Light->Render(global);
}

void RenderManager::DebugRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	if (m_RenderOption & RENDER_DEBUG)
	{
		m_Debug->BeginRender();

		while (meshList->size() != 0)
		{
			MeshData* mesh = meshList->front();

			switch (mesh->ObjType)
			{
			case OBJECT_TYPE::PARTICLE:
			case OBJECT_TYPE::BASE:
			case OBJECT_TYPE::SKINNING:
			case OBJECT_TYPE::TERRAIN:
			case OBJECT_TYPE::BONE:
				m_Debug->Render(mesh, global);
				break;
			}

			meshList->pop();
		}
	}
}

void RenderManager::EndRender()
{
	// Graphic State Reset..
	RenderPassBase::GraphicReset();

	// ???? ????..
	m_SwapChain->Present(0, 0);
}

void RenderManager::OnResize(int width, int height)
{
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->SetResize(width, height);
	}

	RenderPassBase::g_Resource->OnResize(width, height);

	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		renderPass->OnResize(width, height);
	}
}
