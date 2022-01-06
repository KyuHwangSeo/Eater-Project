#pragma once
#include <windows.h>
#include <map>
#include <string>
#include <queue>

/// <summary>
///	여러개의 그래픽엔진을 쉽게 사용하기위해 만든 그래픽엔진 관리 매니저.
/// </summary>

class ObjectManager;
class MeshData;
class GlobalData;
class GraphicEngine;
class LoadMeshData;
class IndexBuffer;
class VertexBuffer;
class TextureBuffer;

namespace ParserData
{
	class Mesh;
}


class GraphicEngineManager
{
public:
	GraphicEngineManager();
	~GraphicEngineManager();
public:
	//선택한 그래픽엔진을 초기화
	void Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight,ObjectManager* GM);

	//선택한 그래픽엔진 랜더링
	void RenderSetting(UINT& renderOption);
	void Render(std::queue<MeshData*>* meshList, GlobalData* global);
	void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global);
	void SSAORender(GlobalData* global);
	void AlphaRender(std::queue<MeshData*>* meshList, GlobalData* global);
	void UIRender(std::queue<MeshData*>* meshList, GlobalData* global);
	void LightRender(GlobalData* global);
	void DebugRender(std::queue<MeshData*>* meshList, GlobalData* global);
	void EndRender();

	//선택한 그래픽엔진으로 인덱스버퍼를 생성함
	void CreateMeshBuffer(ParserData::Mesh* mModel, LoadMeshData* meshData);

	//선택한 그래픽엔진으로 텍스쳐 생성
	TextureBuffer* CreateTextureBuffer(std::string Name);

	//선택한 그래픽 엔진으로 Resize
	void OnReSize(int Change_Width, int Change_Height);

private:
	GraphicEngine* GEngine;

	ObjectManager* ObjManager;
};

