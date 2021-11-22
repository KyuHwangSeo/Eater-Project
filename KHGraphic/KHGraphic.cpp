#include "DirectDefine.h"
#include "D3D11Graphic.h"
#include "KHGraphic.h"

#include "ShaderManagerBase.h"
#include "ResourceManagerBase.h"
#include "ResourceFactory.h"
#include "RenderManager.h"
#include "RenderPassBase.h"

KHGraphic::KHGraphic()
	:m_ResourceFactory(nullptr), m_RenderManager(nullptr)
{

}

KHGraphic::~KHGraphic()
{

}

void KHGraphic::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	// DirectX11 Device 생성..
	D3D11Graphic* graphic = new D3D11Graphic();
	graphic->Initialize(hwnd, screenWidth, screenHeight);

	// Resource Factory 생성 및 초기화..
	m_ResourceFactory = new GraphicResourceFactory(graphic);
	m_ResourceFactory->Initialize(screenWidth, screenHeight);

	// Render Manager 생성 및 초기화..
	m_RenderManager = new RenderManager(graphic, m_ResourceFactory);
	m_RenderManager->Initialize(screenWidth, screenHeight);
}

void KHGraphic::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	m_RenderManager->Render(meshList, global);
}

void KHGraphic::ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	m_RenderManager->ShadowRender(meshList, global);
}

void KHGraphic::SSAORender()
{

}

void KHGraphic::UIRender(std::queue<MeshData*>* meshList, GlobalData* global)
{

}

void KHGraphic::OnReSize(int screenWidth, int screenheight)
{
	m_RenderManager->OnResize(screenWidth, screenheight);
}

void KHGraphic::Delete()
{

}

Indexbuffer* KHGraphic::CreateIndexBuffer(ParserData::Mesh* mesh)
{
	return m_ResourceFactory->CreateIndexBuffer(mesh);
}

Vertexbuffer* KHGraphic::CreateVertexBuffer(ParserData::Mesh* mesh)
{
	return m_ResourceFactory->CreateVertexBuffer(mesh);
}

TextureBuffer* KHGraphic::CreateTextureBuffer(std::string path)
{
	return m_ResourceFactory->CreateTextureBuffer(path);
}

GRAPHIC_DLL void KHGraphic::SetViewPort(void* VPT, int Change_Width, int Change_Height)
{
	RenderManager* rm = reinterpret_cast<RenderManager*>(m_RenderManager);
	rm->m_ViewPort = reinterpret_cast<D3D11_VIEWPORT*>(VPT);

	m_ResourceFactory->Initialize(Change_Width, Change_Height);
	m_RenderManager->Initialize(Change_Width, Change_Height);
}

GRAPHIC_DLL void KHGraphic::SetDevice(void* Devie, void* DevieContext)
{
	ID3D11Device** device = reinterpret_cast<ID3D11Device**>(&Devie);
	ID3D11DeviceContext** context = reinterpret_cast<ID3D11DeviceContext**>(&DevieContext);

	// Resource Factory 생성 및 초기화..
	m_ResourceFactory = new GraphicResourceFactory(device, context);

	// Render Manager 생성 및 초기화..
	m_RenderManager = new RenderManager(nullptr, m_ResourceFactory);

	RenderPassBase::g_Context = *context;
}
