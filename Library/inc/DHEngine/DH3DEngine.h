#pragma once

#include "GraphicsEngine.h"
#include "DH3DEngineDefine.h"
#include "DH3DEngineHeader.h"

#ifdef DH3DENGINE_EXPORTS
#define DHENGINE_DLL __declspec(dllexport)
#else
#define DHENGINE_DLL __declspec(dllimport)

#ifdef _DEBUG
#pragma comment(lib,"DH3DEngine_x64d")
#else
#pragma comment(lib,"DH3DEngine_x64r")
#endif

#endif

class AxisGrid;

class DH3DEngine : public GraphicEngine
{

	/// Graphic Engine 고유 상속.
public:
	DHENGINE_DLL DH3DEngine();
	DHENGINE_DLL virtual ~DH3DEngine();
	/// 초기화 관련
	DHENGINE_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight);
	/// 화면 갱신시 Resize를 할 함수.
	DHENGINE_DLL Indexbuffer* CreateIndexBuffer(ParserData::Mesh* mModel);	//인덱스 버퍼를 만들어준다
	DHENGINE_DLL Vertexbuffer* CreateVertexBuffer(ParserData::Mesh* mModel);	//버텍스 버퍼를 만들어준다
	DHENGINE_DLL TextureBuffer* CreateTextureBuffer(std::string path);					//텍스쳐를 만들어준다
	DHENGINE_DLL void OnReSize(int Change_Width, int Change_Height);
	DHENGINE_DLL void Delete();
	/// 랜더링을 한다 매쉬 랜더데이터 리스트, 글로벌 데이터
	DHENGINE_DLL void Render(std::queue<MeshData*>* meshList, GlobalData* global);

	DHENGINE_DLL void SetViewPort(void* VPT, int Change_Width, int Change_Height);
	DHENGINE_DLL void SetDevice(void* Devie, void* DevieContext);


	/// 엔진내부에서 관리하면 좋은 변수들?
private:
	/// View Mat
	DirectX::SimpleMath::Matrix View_Mat;
	/// Proj Mat
	DirectX::SimpleMath::Matrix Proj_Mat;
	/// World Mat
	DirectX::SimpleMath::Matrix World_Mat;
	// WVP 관련
	DirectX::SimpleMath::Matrix Mul_WVP;		// W * V * P
	DirectX::SimpleMath::Matrix World_Inverse;	// 월드 역행렬
	DirectX::SimpleMath::Matrix World_Inverse_Transpose;

	HWND g_hWnd;
	float g_Screen_Width = 0.0f;
	float g_Screen_Height = 0.0f;
	POINT m_Last_Mouse_Position = { 0, };
	// 디버그 모드인가?
	bool Is_Debug = true;
	// skybox
	std::string Sky_Box_Path = "";

public:
	struct DHVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector3 Normal;
		DirectX::SimpleMath::Vector2 Tex;
		DirectX::SimpleMath::Vector3 Tangent;
	};
	/// 초기 생성자.
	void CreateGraphicResource();
	/// 디버그 세팅값
	void SetDebug(bool _Is_Debug, int _Grid_Num = 50);
	/// 엔진 Draw 시작
	void BeginDraw();
	/// 엔진 Draw 종료.
	void EndDraw();
	/// DX11 고유의 함수들.
private:
	/// 현재 종횡비를 리턴하는 부분.
	float GetAspectRatio() const;
	/// 랜더 스테이트를 여러개 만들어두고 스위칭.
	void CreateRenderStates();

private:
	/// Render State
	// 미리 여러 세트를 만들어두고 스위칭한다.
	ID3D11RasterizerState* mWireframeRS = nullptr;
	ID3D11RasterizerState* mSolidRS = nullptr;

	// 폰트때문에 뎁스스탠실 스테이트가 강제.
	ID3D11DepthStencilState* NormalDSS = nullptr;

	/// Axis, Grid
	AxisGrid* m_AxisGrid = nullptr;

	/// 어뎁터 정보
	DXGI_ADAPTER_DESC1 m_Adapter_Desc1;

	/// DX11 관련 필요변수.
	int Video_Card_Memory = 0;
	D3D_FEATURE_LEVEL featureLevel;

	IDXGISwapChain* DX11_Swap_Chain = nullptr;
	ID3D11Device* DX11_Device = nullptr;
	ID3D11DeviceContext* DX11_Device_Context = nullptr;
	ID3D11RenderTargetView* DX11_Render_Target_View = nullptr;
	ID3D11Texture2D* DX11_Depth_Stencil_Buffer = nullptr;
	ID3D11DepthStencilState* DX11_Depth_Stencil_State = nullptr;
	ID3D11DepthStencilView* DX11_Depth_Stencil_View = nullptr;
	ID3D11RasterizerState* DX11_Raster_State = nullptr;	
	D3D11_VIEWPORT DX11_View_Port = { 0, };					// 뷰포트

private:
	/// 쓰이고있는거
	ID3D11Buffer* Render_VB = nullptr;
	ID3D11Buffer* Render_IB = nullptr;

	/// Render에서 매번 할당 / 해제 하는 변수들은 따로 선언해두자.
	// Buffer 관련
	UINT Vertex_Buffer_Stride = 0;
	UINT Vertex_Buffer_Offset = 0;


	/// 안쓰이고 있는거
	ID3DX11EffectMatrixVariable* mfxWorldViewProj = nullptr;

	ID3DX11Effect* mFX = nullptr;
	ID3DX11EffectTechnique* mTech = nullptr;
};

