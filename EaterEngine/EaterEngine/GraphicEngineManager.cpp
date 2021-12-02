#include "GraphicEngineManager.h"
#include "DebugManager.h"
#include "GraphicsEngine.h"
#include "EngineData.h"
#include "ParserData.h"
#include "ObjectManager.h"

GraphicEngineManager::GraphicEngineManager()
{
	GEngine = nullptr;
}

GraphicEngineManager::~GraphicEngineManager()
{
	

}

void GraphicEngineManager::Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight, ObjectManager* GM)
{
	ObjManager = GM;

	// Graphic Engine Create..
	GEngine = GraphicEngine::Create();

	// Graphic Engine Initialize..
	GEngine->Initialize(Hwnd, WinSizeWidth, WinSizeHeight);
}

void GraphicEngineManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//하나의 엔진이 끝날때마다 ReQueue는 사라짐 계속 넣어준다
	ObjManager->CreateRenderQueue();

	//해당 엔진을 랜더링
	GEngine->Render(ObjManager->GetRenderQueue(), global);
}

void GraphicEngineManager::ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//하나의 엔진이 끝날때마다 ReQueue는 사라짐 계속 넣어준다
	ObjManager->CreateRenderQueue();

	//해당 엔진을 랜더링
	GEngine->ShadowRender(ObjManager->GetRenderQueue(), global);
}

void GraphicEngineManager::SSAORender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//if (NowEngine != nullptr)
	//{
	//	NowEngine->SSAORender();
	//}
}

void GraphicEngineManager::UIRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//if (NowEngine != nullptr)
	//{
	//	NowEngine->UIRender(meshList, global);
	//}
}

Indexbuffer* GraphicEngineManager::CreateIndexBuffer(ParserData::Mesh* mModel)
{
	//인덱스버퍼 생성
	return GEngine->CreateIndexBuffer(mModel);
}

Vertexbuffer* GraphicEngineManager::CreateVertexBuffer(ParserData::Mesh* mModel)
{
	//버텍스 버퍼 생성
	return GEngine->CreateVertexBuffer(mModel);
}

TextureBuffer* GraphicEngineManager::CreateTextureBuffer(std::string Name)
{
	//텍스쳐 생성
	return GEngine->CreateTextureBuffer(Name);
}

void GraphicEngineManager::OnReSize(int Change_Width, int Change_Height)
{
	//사이즈 변경
	GEngine->OnReSize(Change_Width, Change_Height);
}