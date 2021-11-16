#include "GraphicEngineManager.h"
#include "DebugManager.h"
#include "GraphicsEngine.h"
#include "EngineData.h"
#include "ParserData.h"

GraphicEngineManager::GraphicEngineManager()
{
	NowEngine = nullptr;
}

GraphicEngineManager::~GraphicEngineManager()
{
	
}

void GraphicEngineManager::Update()
{
	if (NowEngine == nullptr)
	{
		DebugManager::Print("선택한 그래픽엔진이 없습니다", 80, 0, DebugManager::MSG_TYPE::MSG_ERROR);
	}
	else
	{
		std::string temp = "선택된 그래픽 엔진 :" + EngineName;
		DebugManager::Print(temp, 80, 0, DebugManager::MSG_TYPE::MSG_ENGINE);
	}

}

void GraphicEngineManager::EngineAllInitialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight)
{
	//모든 엔진을 초기화 시킨다
	std::map<std::string, GraphicEngine*>::iterator it = GEngineList.begin();
	for (it; it != GEngineList.end(); it++) 
	{

		if ((*it).second == nullptr) { continue; }
		(*it).second->Initialize(Hwnd, WinSizeWidth, WinSizeHeight);
	}
}

void GraphicEngineManager::Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight)
{
	NowEngine->Initialize(Hwnd, WinSizeWidth, WinSizeHeight);
}

void GraphicEngineManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//선택한 엔진 랜더링
	if (NowEngine != nullptr)
	{
		NowEngine->Render(meshList, global);
	}
}

void GraphicEngineManager::ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	if (NowEngine != nullptr)
	{
		NowEngine->ShadowRender(meshList, global);
	}
}

void GraphicEngineManager::SSAORender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	if (NowEngine != nullptr)
	{
		NowEngine->SSAORender();
	}
}

void GraphicEngineManager::UIRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	if (NowEngine != nullptr)
	{
		NowEngine->UIRender(meshList, global);
	}
}

Indexbuffer* GraphicEngineManager::CreateIndexBuffer(ParserData::Mesh* mModel)
{
	//인덱스버퍼 생성
	return NowEngine->CreateIndexBuffer(mModel);
}

Vertexbuffer* GraphicEngineManager::CreateVertexBuffer(ParserData::Mesh* mModel)
{
	//버텍스 버퍼 생성
	return NowEngine->CreateVertexBuffer(mModel);
}

TextureBuffer* GraphicEngineManager::CreateTextureBuffer(std::string Name)
{
	//텍스쳐 생성
	return NowEngine->CreateTextureBuffer(Name);
}

void GraphicEngineManager::OnReSize(int Change_Width, int Change_Height)
{
	//사이즈 변경
	NowEngine->OnReSize(Change_Width, Change_Height);
}

void GraphicEngineManager::ChoiceEngine(std::string Name)
{
	//랜더링할 엔진 선택
	if (GEngineList.find(Name) == GEngineList.end())
	{
		DebugManager::Print("그래픽엔진을 찾지못했습니다", 80, 1, DebugManager::MSG_TYPE::MSG_ERROR);
	}
	else
	{
		EngineName = Name;
		NowEngine = GEngineList[Name];
	}
}

void GraphicEngineManager::Delete()
{
	//그래픽 엔진을 종료 시킨다
	std::map<std::string, GraphicEngine*>::iterator it = GEngineList.begin();
	for (it; it != GEngineList.end(); it++)
	{

		if ((*it).second == nullptr) { continue; }
		(*it).second->Delete();
	}
	GEngineList.clear();
}
