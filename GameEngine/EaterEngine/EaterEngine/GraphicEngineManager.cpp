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

void GraphicEngineManager::BeginRender(UINT& renderOption)
{
	if (renderOption)
	{
		GEngine->BeginRender(renderOption);
	}
}

void GraphicEngineManager::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//�ϳ��� ������ ���������� ReQueue�� ����� ��� �־��ش�
	ObjManager->CreateRenderQueue();

	//�ش� ������ ������
	GEngine->Render(ObjManager->GetRenderQueue(), global);
}

void GraphicEngineManager::ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//�ϳ��� ������ ���������� ReQueue�� ����� ��� �־��ش�
	ObjManager->CreateRenderQueue();

	//�ش� ������ ������
	GEngine->ShadowRender(ObjManager->GetRenderQueue(), global);
}

void GraphicEngineManager::SSAORender(GlobalData* global)
{
	//�ش� ������ ������
	GEngine->SSAORender(global);
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
	GEngine->LightRender(global);
}

void GraphicEngineManager::EndRender()
{
	//�ش� ������ ������
	GEngine->EndRender();
}

void GraphicEngineManager::CreateMeshBuffer(ParserData::Mesh* mModel, LoadMeshData* meshData)
{
	//�Ž� ����
	GEngine->CreateMeshBuffer(mModel, meshData);
}

TextureBuffer* GraphicEngineManager::CreateTextureBuffer(std::string Name)
{
	//�ؽ��� ����
	return GEngine->CreateTextureBuffer(Name);
}

void GraphicEngineManager::OnReSize(int Change_Width, int Change_Height)
{
	//������ ����
	GEngine->OnReSize(Change_Width, Change_Height);
}