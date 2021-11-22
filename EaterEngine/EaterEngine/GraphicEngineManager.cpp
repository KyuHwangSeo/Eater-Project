#include "GraphicEngineManager.h"
#include "DebugManager.h"
#include "GraphicsEngine.h"
#include "EngineData.h"
#include "ParserData.h"
#include "ObjectManager.h"
#include "MultiRenderEngine.h"

GraphicEngineManager::GraphicEngineManager()
{
	MultiEngine = nullptr;
}

GraphicEngineManager::~GraphicEngineManager()
{
	

}

void GraphicEngineManager::Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight, ObjectManager* GM)
{
	//멀티 엔진 초기화
	ObjManager = GM;
	MultiEngine = MultiRenderEngine::Initialize(Hwnd, WinSizeWidth, WinSizeHeight);
}

void GraphicEngineManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//멀티 엔진 랜더링 시작 
	MultiEngine->BeginRender();

	//스플릿된 개수 만큼 랜더
	for (int i = 0; i < MultiEngine->GetWindowCount(); i++)
	{
		//하나의 엔진이 끝날때마다 ReQueue는 사라짐 계속 넣어준다
		ObjManager->CreateRenderQueue();
		//해당 엔진을 랜더링
		MultiEngine->Render(i,ObjManager->GetRenderQueue(), global);
	}

	//멀티 엔진 랜더링 종료
	MultiEngine->EndRender();
}

void GraphicEngineManager::ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//스플릿된 개수 만큼 랜더
	for (int i = 0; i < MultiEngine->GetWindowCount(); i++)
	{
		//하나의 엔진이 끝날때마다 ReQueue는 사라짐 계속 넣어준다
		ObjManager->CreateRenderQueue();
		//해당 엔진을 랜더링
		MultiEngine->ShadowRender(i, ObjManager->GetRenderQueue(), global);
	}
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
	return MultiEngine->CreateIndexBuffer(mModel);
}

Vertexbuffer* GraphicEngineManager::CreateVertexBuffer(ParserData::Mesh* mModel)
{
	//버텍스 버퍼 생성
	return MultiEngine->CreateVertexBuffer(mModel);
}

TextureBuffer* GraphicEngineManager::CreateTextureBuffer(std::string Name)
{
	//텍스쳐 생성
	return MultiEngine->CreateTextureBuffer(Name);
}

void GraphicEngineManager::OnReSize(int Change_Width, int Change_Height)
{
	//사이즈 변경
	MultiEngine->OnResize(Change_Width, Change_Height);
}

void GraphicEngineManager::PushEngine(int Number, GraphicEngine* Engine, std::string Name)
{
	//엔진을 밀어 넣어주고 들어온 넘버와 엔진을 매칭 시켜준다
	MultiEngine->RegisterRenderer(Engine, Name);
	MultiEngine->SetRenderer(Number, Name);
}

void GraphicEngineManager::SplitWindow(int X, int Y)
{
	//윈도우 개수만큼 나눠줌
	MultiEngine->SplitWindow(X, Y);
}
