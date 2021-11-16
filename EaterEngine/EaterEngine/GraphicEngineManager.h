#pragma once
#include <windows.h>
#include <map>
#include <string>
#include <queue>

/// <summary>
///	여러개의 그래픽엔진을 쉽게 사용하기위해 만든 그래픽엔진 관리 매니저.
/// </summary>

class MeshData;
class GlobalData;
class GraphicEngine;

class Indexbuffer;
class Vertexbuffer;
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


	//사용할 그래픽엔진을 리스트에 넣는다
	template<typename T>
	void PushEngine(std::string Name);

	void Update();
	//모든 그래픽 엔진 초기화
	void EngineAllInitialize(HWND Hwnd,int WinSizeWidth,int WinSizeHeight);
	//사용할 그래픽엔진 선택
	void ChoiceEngine(std::string Name);
public:
	//선택한 그래픽엔진을 초기화
	void Initialize(HWND Hwnd, int WinSizeWidth, int WinSizeHeight);

	//선택한 그래픽엔진 랜더링
	void Render(std::queue<MeshData*>* meshList, GlobalData* global);
	void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global);
	void SSAORender(std::queue<MeshData*>* meshList, GlobalData* global);
	void UIRender(std::queue<MeshData*>* meshList, GlobalData* global);

	//선택한 그래픽엔진으로 인덱스버퍼를 생성함
	Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mModel);

	//선택한 그래픽 엔진으로 버텍스 버퍼를 생성함
	Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mModel);

	//선택한 그래픽엔진으로 텍스쳐 생성
	TextureBuffer* CreateTextureBuffer(std::string Name);

	//선택한 그래픽 엔진으로 Resize
	void OnReSize(int Change_Width, int Change_Height);

	//모든 엔진 삭제
	void Delete();
private:
	//현재 선택된 그래픽 엔진
	GraphicEngine* NowEngine;

	std::string EngineName;
	//그래픽 엔진들을 관리할 리스트
	std::map<std::string,GraphicEngine*> GEngineList;
};

template<typename T>
inline void GraphicEngineManager::PushEngine(std::string Name)
{
	T* temp = new T();
	GEngineList.insert({Name,temp});
}
