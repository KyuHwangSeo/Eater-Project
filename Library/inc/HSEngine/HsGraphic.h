#pragma once

#include "HsGraphicDLL.h"
#include "GraphicsEngine.h"

class ShaderManager;
class RenderingManager;
class GraphicDebugManager;
class TextureBuffer;
class Indexbuffer;
class Vertexbuffer;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct D3D11_VIEWPORT;
struct IDXGISwapChain;
struct ID3D11RasterizerState;

class HsGraphic : public GraphicEngine
{
public:
	HS_GRAPHICDLL HsGraphic();
	virtual HS_GRAPHICDLL ~HsGraphic();


	virtual HS_GRAPHICDLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight) override;
	virtual HS_GRAPHICDLL void OnReSize(int Change_Width, int Change_Height) override;
	virtual HS_GRAPHICDLL void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;
	virtual HS_GRAPHICDLL void Delete() override;
	virtual HS_GRAPHICDLL void SetViewPort(void* VPT, int Change_Width, int Change_Height);
	virtual HS_GRAPHICDLL void SetDevice(void* Devie, void* DevieContext);

	//텍스쳐 버퍼를생성
	virtual HS_GRAPHICDLL TextureBuffer* CreateTextureBuffer(std::string path) override;
	//인덱스 버퍼를 생성
	virtual HS_GRAPHICDLL Indexbuffer*	CreateIndexBuffer(ParserData::Mesh* mModel) override;
	//버텍스 버퍼를 생성
	virtual HS_GRAPHICDLL Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mModel) override;
	
	virtual HS_GRAPHICDLL Vertexbuffer* CreateTerrainVertexBuffer(ParserData::Mesh* mModel, std::string maskName) override;

	ID3D11RenderTargetView* GetEngineRTV();
	ID3D11DepthStencilView* GetEngineDSV();
private:
	void Create_SwapChain_RenderTarget();	//랜더타겟 생성
	void Create_ViewPort();					//뷰포트 생성
	void CreateDevice();		//엔진의 디바이스를 생성해준다
	void BeginRender();			//랜더링 시작
	void EndRender();			//랜더링 종료
	
	//메쉬 타입별 생성
	Vertexbuffer* CreateBasicVertexBuffer(ParserData::Mesh* mModel);
	Vertexbuffer* CreateSkinngingVertexBuffer(ParserData::Mesh* mModel);
private:

	///엔진 데이터
	HWND					hwnd;				//윈도우 핸들
	ID3D11Device*			Device;				//디바이스
	ID3D11DeviceContext*	DeviceContext;		//디바이스 컨텍스트
	ID3D11RenderTargetView* mRenderTargetView;	//랜더 타겟
	ID3D11DepthStencilView* mDepthStencilView;	//뎁스 스텐실뷰
	D3D11_VIEWPORT*			mScreenViewport;	//뷰포트
	IDXGISwapChain*			mSwapChain;			//스왑체인	

	//윈도우 사이즈
	int WinSizeX;	//윈도우 사이즈 가로
	int WinSizeY;	//윈도우 사이즈 세로

	//랜더링 모드
	ID3D11RasterizerState* mWireframe;
	ID3D11RasterizerState* mSolid;

	//매니저들
	ShaderManager*			mShaderManager;
	RenderingManager*		mRenderManager;
	GraphicDebugManager*	mDebugManager;
};