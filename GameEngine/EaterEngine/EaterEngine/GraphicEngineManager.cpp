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

void GraphicEngineManager::RenderSetting(UINT& renderOption)
{
	GEngine->RenderSetting(renderOption);
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

void GraphicEngineManager::SSAORender(GlobalData* global)
{
	//해당 엔진을 랜더링
	GEngine->SSAORender(global);
}

void GraphicEngineManager::AlphaRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//하나의 엔진이 끝날때마다 ReQueue는 사라짐 계속 넣어준다
	ObjManager->CreateRenderQueue();

	//해당 엔진을 랜더링
	GEngine->AlphaRender(ObjManager->GetRenderQueue(), global);
}

void GraphicEngineManager::UIRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//if (NowEngine != nullptr)
	//{
	//	NowEngine->UIRender(meshList, global);
	//}
}

void GraphicEngineManager::LightRender(GlobalData* global)
{
	//해당 엔진을 랜더링
	GEngine->LightRender(global);
}

void GraphicEngineManager::DebugRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//하나의 엔진이 끝날때마다 ReQueue는 사라짐 계속 넣어준다
	ObjManager->CreateRenderQueue();
	
	//해당 엔진을 랜더링
	GEngine->DebugRender(meshList, global);
}

void GraphicEngineManager::EndRender()
{
	//해당 엔진을 랜더링
	GEngine->EndRender();
}

void GraphicEngineManager::CreateMeshBuffer(ParserData::Mesh* mModel, LoadMeshData* meshData)
{
	//매쉬 생성
	GEngine->CreateMeshBuffer(mModel, meshData);
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