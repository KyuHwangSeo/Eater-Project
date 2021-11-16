#pragma once

#ifdef MULTIRENDERENGINE_EXPORTS
#define MULTIENGINE_DLL __declspec(dllexport)
#else
#define MULTIENGINE_DLL __declspec(dllimport)

#ifdef _DEBUG
#pragma comment(lib,"MultiRenderEngine_x64d")
#else
#pragma comment(lib,"MultiRenderEngine_x64r")
#endif

#endif

// 최대 나누어질 수 있는 랜더타겟 뷰. (20개로 지정함..)
#define MAX_SPLIT_TARGET	20
// 최대 가로축
#define MAX_HORIZONTAL	6
// 최대 세로축
#define MAX_VERTICAL	5


#include <windows.h>
#include <map>
#include <queue>
#include <string>

struct ID3D11RenderTargetView;
class GraphicEngine;
class MeshData;
class GlobalData;

class MultiRenderEngine
{
private:
	/// 생성자를 Private으로 감추어, Initialize 를 호출하게끔 한다.
	MultiRenderEngine();
	~MultiRenderEngine();

	static MultiRenderEngine* m_Engine;
	
private:
	// 쪼갠 화면의 수
	std::map<int, std::pair<ID3D11RenderTargetView* , GraphicEngine*>> Split_Window;
	// 현재 등록되어있는 엔진들을 관리.
	std::map<std::string, GraphicEngine*> Registered_Engine_List;

public:
	MULTIENGINE_DLL static MultiRenderEngine* Initialize(HWND hwnd, int screenWidth, int screenHeight);
	
	/// 화면을 얼만큼 쪼갤지 정함. 만약 최대 개수를 넘거나, 가로/세로 축의 최대값을 넘으면 return 값이 0(false)이다.
	/*
		예를 들어 Horizontal 4, Vertical 3 이라면.. 다음과 같이 나눠진다
		 _______________________________
		|		|		|		|		|
		|	0	|	1	|	2	|	3	|
		|_______|_______|_______|_______|
		|		|		|		|		|
		|	4	|	5	|	6	|	7	|
		|_______|_______|_______|_______|
		|		|		|		|		|
		|	8	|	9	|	10	|	11	|
		|_______|_______|_______|_______|
	
	*/
	MULTIENGINE_DLL BOOL SplitWindow(int _Horizontal, int _Vertical);
	MULTIENGINE_DLL BOOL RegisterRenderer(GraphicEngine* _Renderer, std::string _Engine_Name);
	MULTIENGINE_DLL BOOL SetRenderer(int _ViewPort_Number, std::string _Engine_Name);

	MULTIENGINE_DLL void Render(std::queue<MeshData*>* meshList, GlobalData* global);
	MULTIENGINE_DLL void Delete();

private:
	void BeginRender();
	void EndRender();
};

