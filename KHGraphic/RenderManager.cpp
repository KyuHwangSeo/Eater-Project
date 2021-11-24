#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "ResourceFactoryBase.h"
#include "ShaderManagerBase.h"
#include "ResourceManagerBase.h"
#include "RenderPassBase.h"
#include "RenderManager.h"

#include "ShaderBase.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"

#include "MathDefine.h"
#include "ForwardPass.h"
#include "ShadowPass.h"
#include "DeferredPass.h"
#include "LightPass.h"
#include "VertexDefine.h"

RenderManager::RenderManager(D3D11Graphic* graphic, IGraphicResourceFactory* factory)
{
	// Rendering Initialize..
	RenderPassBase::Initialize(nullptr, factory, factory->GetResourceManager(), factory->GetShaderManager());

	m_Farward = new ForwardPass();
	//m_Deferred = new DeferredPass();
	//m_Light = new LightPass();
	m_Shadow = new ShadowPass();

	m_RenderPassList.push_back(m_Farward);
	//m_RenderPassList.push_back(m_Deferred);
	//m_RenderPassList.push_back(m_Light);
	m_RenderPassList.push_back(m_Shadow);
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
		renderPass->Start();
	}
}

void RenderManager::Release()
{
	for (RenderPassBase* renderPass : m_RenderPassList)
	{
		RELEASE_COM(renderPass);
	}

	m_RenderPassList.clear();
}

void RenderManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	m_Farward->BeginRender();

	RenderPassBase::g_Context->RSSetViewports(1, m_ViewPort);

	while (meshList->size() != 0)
	{
		MeshData* mesh = meshList->front();

		switch (mesh->ObjType)
		{
		case OBJECT_TYPE::BASE:
		case OBJECT_TYPE::SKINNING:
			m_Farward->Update(mesh, global);
			m_Farward->Render(mesh);
			break;
		}

		meshList->pop();
	}

	// 최종 출력..
	//m_SwapChain->Present(0, 0);
}

void RenderManager::ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	m_Shadow->BeginRender();

	while (meshList->size() != 0)
	{
		MeshData* mesh = meshList->front();

		switch (mesh->ObjType)
		{
		case OBJECT_TYPE::BASE:
		case OBJECT_TYPE::SKINNING:
			m_Shadow->Update(mesh, global);
			m_Shadow->Render(mesh);
			break;
		}

		meshList->pop();
	}
}

void RenderManager::SSAORender()
{

}

void RenderManager::UIRender(std::queue<MeshData*>* meshList, GlobalData* global)
{

}

void RenderManager::OnResize(int width, int height)
{
	//RenderPassBase::g_Resource->OnResize(width, height);
	//
	//for (RenderPassBase* renderPass : m_RenderPassList)
	//{
	//	renderPass->OnResize(width, height);
	//}
}
