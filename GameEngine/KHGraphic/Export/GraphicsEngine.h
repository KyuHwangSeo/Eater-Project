#pragma once
#include <windows.h>
#include <string>
#include <queue>
#include "GraphicDLL.h"

/// <summary>
/// 게임 엔진에서 제공하는 그래픽 엔진 최상위 클래스
/// 해당 클래스를 통해 Graphic Engine을 받을 수 있다..
/// </summary>

class MeshData;
class GlobalData;
class IndexBuffer;
class VertexBuffer;
class TextureBuffer;
class LoadMeshData;

namespace ParserData 
{
	class Mesh;
}

class GraphicEngine
{
protected:
	GraphicEngine() {};
	virtual ~GraphicEngine() {};

public:
	///그래픽 엔진을 받을 수 있는 함수
	static GRAPHIC_DLL GraphicEngine* Create();

public:
	///그래픽 엔진과 게임엔진에서 주고받아야할 함수들
	virtual GRAPHIC_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight) abstract;	// 초기화 함수
	virtual GRAPHIC_DLL void OnReSize(int Change_Width, int Change_Height) abstract;				// 리사이즈
	virtual GRAPHIC_DLL void Delete() abstract;

	virtual GRAPHIC_DLL void CreateMeshBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) abstract;	// 버퍼를 만들어준다
	virtual GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path) abstract;			// 텍스쳐를 만들어준다

	/// 랜더링을 한다 매쉬 랜더데이터 리스트, 글로벌 데이터
	virtual GRAPHIC_DLL void RenderSetting(UINT& renderOption) abstract;
	virtual GRAPHIC_DLL void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void Render(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void SSAORender(GlobalData* global) abstract;
	virtual GRAPHIC_DLL void AlphaRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void LightRender(GlobalData* global) abstract;
	virtual GRAPHIC_DLL void DebugRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void EndRender() abstract;
};
