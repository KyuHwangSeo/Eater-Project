#pragma once

#include "HsGraphicDLL.h"
#include "../KHGraphic/Export/GraphicsEngine.h"
#include "d3d11.h"

class ShaderManager;
class RenderingManager;
class GraphicDebugManager;
class TextureBuffer;
class Indexbuffer;
class Vertexbuffer;

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

	ID3D11RenderTargetView* GetEngineRTV();
	ID3D11DepthStencilView* GetEngineDSV();
private:
	void CreateRenderTarget();	//랜더타겟 뎁스스텐실 뷰포트를 생성한다
	void CreateDevice();		//엔진의 디바이스를 생성해준다
	void BeginRender();			//랜더링 시작
	void EndRender();			//랜더링 종료
	
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