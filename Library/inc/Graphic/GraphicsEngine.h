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
class Indexbuffer;
class Vertexbuffer;
class TextureBuffer;

namespace ParserData 
{
	class Mesh;
}

class GraphicEngine
{
public:
	GraphicEngine() {};
	virtual GRAPHIC_DLL ~GraphicEngine() {};

	///그래픽 엔진을 받을 수 있는 함수
	static GRAPHIC_DLL GraphicEngine* Create();

	/// 형선 킴 전용 그래픽 엔진 생성
	static GRAPHIC_DLL GraphicEngine* CreateHS();

	///그래픽 엔진과 게임엔진에서 주고받아야할 함수들
	virtual GRAPHIC_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight) = 0;	// 초기화 함수
	virtual GRAPHIC_DLL Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mModel) = 0;		// 인덱스 버퍼를 만들어준다
	virtual GRAPHIC_DLL Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mModel)	= 0;	// 버텍스 버퍼를 만들어준다
	virtual GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path)  = 0;			// 텍스쳐를 만들어준다
	virtual GRAPHIC_DLL void OnReSize(int Change_Width, int Change_Height) = 0;				// 리사이즈
	virtual GRAPHIC_DLL void Delete() = 0;

	/// 랜더링을 한다 매쉬 랜더데이터 리스트, 글로벌 데이터
	virtual GRAPHIC_DLL void Render(std::queue<MeshData*>* meshList,GlobalData* global) = 0;
	virtual GRAPHIC_DLL void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) { return; };
	virtual GRAPHIC_DLL void SSAORender() { return; };
	virtual GRAPHIC_DLL void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) { return; };
};
