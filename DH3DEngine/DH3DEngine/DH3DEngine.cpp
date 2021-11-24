
#include "DH3DEngine.h"
#include "ParserData.h"
#include "EngineData.h"
#include "Effects.h"
#include "StructHelper.h"
#include "AxisGrid.h"

DH3DEngine::DH3DEngine()
{

}

DH3DEngine::~DH3DEngine()
{
	InputLayouts::DestroyAll();
	Effects::DestroyAll();
}

void DH3DEngine::Initialize(HWND _hWnd, int screenWidth, int screenHeight)
{
	g_hWnd = _hWnd;
	g_Screen_Width = (float)screenWidth;
	g_Screen_Height = (float)screenHeight;
	CreateGraphicResource();

}

void DH3DEngine::CreateGraphicResource()
{
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;

	// DXGI 팩토리 생성.
	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));

	// Use the factory to create an adapter for the primary graphics interface (video card).
	HR(factory->EnumAdapters(0, &adapter));

	// Enumerate the primary adapter output (monitor).
	HR(adapter->EnumOutputs(0, &adapterOutput));

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	HR(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];

	if (!displayModeList)
	{
		return;
	}

	// Now fill the display mode list structures.
	HR(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)g_Screen_Width)
		{
			if (displayModeList[i].Height == (unsigned int)g_Screen_Height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	HR(adapter->GetDesc(&adapterDesc));

	// 비디오카드 메모리를 MB 단위로 바꿔준다.
	Video_Card_Memory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	SAFE_RELEASE(adapterOutput);

	// Release the adapter.
	SAFE_RELEASE(adapter);

	// Release the factory.
	SAFE_RELEASE(factory);

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = (UINT)g_Screen_Width;
	swapChainDesc.BufferDesc.Height = (UINT)g_Screen_Height;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.

	// Vsync 지원
	//swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
	//swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;

	// Vsync 미지원
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = g_hWnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	/// 풀 스크린 모드.
	swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// 스왑 체인과 Device를 같이 생성. (D2D를 지원하기 위해 옵션으로 BGRA 를 셋팅 해 두었다.)
	HR(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &DX11_Swap_Chain, &DX11_Device, NULL, &DX11_Device_Context));

	// Get the pointer to the back buffer.
	HR(DX11_Swap_Chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr));

	// Create the render target view with the back buffer pointer.
	HR(DX11_Device->CreateRenderTargetView(backBufferPtr, NULL, &DX11_Render_Target_View));

	// Release pointer to the back buffer as we no longer need it.
	SAFE_RELEASE(backBufferPtr);

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = (UINT)g_Screen_Width;
	depthBufferDesc.Height = (UINT)g_Screen_Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	HR(DX11_Device->CreateTexture2D(&depthBufferDesc, NULL, &DX11_Depth_Stencil_Buffer));

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	HR(DX11_Device->CreateDepthStencilState(&depthStencilDesc, &DX11_Depth_Stencil_State));

	// Set the depth stencil state.
	DX11_Device_Context->OMSetDepthStencilState(DX11_Depth_Stencil_State, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	HR(DX11_Device->CreateDepthStencilView(DX11_Depth_Stencil_Buffer, &depthStencilViewDesc, &DX11_Depth_Stencil_View));

	/// 렌더타겟뷰, 뎁스/스탠실뷰를 파이프라인에 바인딩한다.
	DX11_Device_Context->OMSetRenderTargets(1, &DX11_Render_Target_View, DX11_Depth_Stencil_View);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	HR(DX11_Device->CreateRasterizerState(&rasterDesc, &DX11_Raster_State));

	// Now set the rasterizer state.
	DX11_Device_Context->RSSetState(DX11_Raster_State);

	// Setup the viewport for rendering.
	viewport.Width = (float)g_Screen_Width;
	viewport.Height = (float)g_Screen_Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	DX11_Device_Context->RSSetViewports(1, &viewport);

	// Mesh
	//BuildGeometryBuffers();

	//BuildFX();
	//BuildFX_Create();
	//BuildVertexLayout();

	// Render State
	CreateRenderStates();

	SetDebug(true, 50);
}

void DH3DEngine::SetDebug(bool _Is_Debug, int _Grid_Num)
{
	Is_Debug = _Is_Debug;

	/// 해당 디버그 모드에서의 Grid 개수지정. default = 50
	//_Grid_Num = 150;
	//m_AxisGrid->Initialize(_Grid_Num);
}

void DH3DEngine::BeginDraw()
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = 0.17f;
	color[1] = 0.17f;
	color[2] = 0.17f;
	color[3] = 1.0f;

	// Clear the back buffer.
	DX11_Device_Context->ClearRenderTargetView(DX11_Render_Target_View, color);

	// Clear the depth buffer.
	DX11_Device_Context->ClearDepthStencilView(DX11_Depth_Stencil_View, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void DH3DEngine::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	DX11_Device_Context->RSSetViewports(1, &DX11_View_Port);

	View_Mat = DirectX::SimpleMath::Matrix(*global->mViewMX);
	Proj_Mat = DirectX::SimpleMath::Matrix(*global->mProj);

	/// AxisGrid를 그린적이 없다면 그려줌! 만약 Begin 과 End 사이에 한번이라도 그렸으면 다시는 안그림.
	m_AxisGrid->Render(&View_Mat, &Proj_Mat);

	/// 그릴 메시가 존재하면 메시리스트를 순회하며 그린다.
 	while (!meshList->empty())
	{
		// 메시 데이터를 하나 꺼내옴.
		MeshData* _Mesh_Data = meshList->front();
		meshList->pop();

		/// 오브젝트들을 그린다. (Draw Primitive)
		if (_Mesh_Data->ObjType != OBJECT_TYPE::BASE)
		{
			continue;
		}

		// 해당 메시의 VB,IB 를 받아옴.
		Render_VB = reinterpret_cast<ID3D11Buffer*>(_Mesh_Data->VB->VertexbufferPointer);
		Render_IB = reinterpret_cast<ID3D11Buffer*>(_Mesh_Data->IB->IndexBufferPointer);

		// 입력 배치 객체 셋팅
		DX11_Device_Context->IASetInputLayout(InputLayouts::PosNormalTexBiNormalTangent);
		DX11_Device_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		//// 렌더 스테이트
		DX11_Device_Context->RSSetState(DX11_Raster_State);

		// 버텍스버퍼와 인덱스버퍼 셋팅
		Vertex_Buffer_Stride = sizeof(DHVertex);
		Vertex_Buffer_Offset = 0;

		/// WVP TM등을 셋팅
		World_Mat = DirectX::SimpleMath::Matrix(_Mesh_Data->mWorld);
		Mul_WVP = World_Mat * View_Mat * Proj_Mat;

		// 월드의 역행렬
		World_Inverse = World_Mat.Invert();

		// Set per frame constants.
		DirectionalLightData _temp_Dir;
		DirectX::SimpleMath::Vector4 _Ambient = DirectX::SimpleMath::Vector4(0.2f, 0.2f, 0.2f, 1.0f);
		DirectX::SimpleMath::Vector4 _Diffuse = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		DirectX::SimpleMath::Vector4 _Specular = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		DirectX::SimpleMath::Vector3 _Direction = DirectX::SimpleMath::Vector3(-0.57735f, -0.57735f, 0.57735f);
		_temp_Dir.Ambient = _Ambient;
		_temp_Dir.Diffuse = _Diffuse;
		_temp_Dir.Specular = _Specular;
		_temp_Dir.Direction = _Direction;

		MaterialData _Temp_Mat;
		DirectX::SimpleMath::Vector4 _Ambient1 = DirectX::SimpleMath::Vector4(0.2f, 0.2f, 0.2f, 1.0f);
		DirectX::SimpleMath::Vector4 _Diffuse1 = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		DirectX::SimpleMath::Vector4 _Specular1 = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		DirectX::SimpleMath::Vector4 _Reflect1 = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		_Temp_Mat.Ambient = _Ambient1;
		_Temp_Mat.Diffuse = _Diffuse1;
		_Temp_Mat.Specular = _Specular1;
		_Temp_Mat.Reflect = _Reflect1;


		Effects::BasicFX->SetDirLights(_temp_Dir);

		// 월드 Eye 포지션.
		DirectX::SimpleMath::Vector3 _Camera_Vec(View_Mat._41, View_Mat._42, View_Mat._43);
		Effects::BasicFX->SetEyePosW(_Camera_Vec);

		ID3DX11EffectTechnique* mTech = nullptr;

		/// 텍스쳐 사용
		//mTech = Effects::BasicFX->Light1Tech;
		/// 텍스쳐 미사용
		mTech = Effects::BasicFX->Light2Tech;

		D3DX11_TECHNIQUE_DESC techDesc;
		mTech->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			DX11_Device_Context->IASetVertexBuffers(0, 1, &Render_VB, &Vertex_Buffer_Stride, &Vertex_Buffer_Offset);
			DX11_Device_Context->IASetIndexBuffer(Render_IB, DXGI_FORMAT_R32_UINT, 0);

			World_Inverse_Transpose = MathHelper::InverseTranspose(World_Mat);
			Effects::BasicFX->SetWorld(World_Mat);
			Effects::BasicFX->SetWorldInvTranspose(World_Inverse_Transpose);
			Effects::BasicFX->SetWorldViewProj(Mul_WVP);
			Effects::BasicFX->SetMaterial(_Temp_Mat);
			Effects::BasicFX->SetTexTransform(DirectX::SimpleMath::Matrix::Identity);

			mTech->GetPassByIndex(p)->Apply(0, DX11_Device_Context);
			DX11_Device_Context->DrawIndexed(_Mesh_Data->IB->Count, 0, 0);
		}

	}

	// Restore default.
	DX11_Device_Context->RSSetState(0);

	//EndDraw();
}

void DH3DEngine::SetViewPort(void* VPT, int Change_Width, int Change_Height)
{
	DX11_View_Port = *(D3D11_VIEWPORT*)(VPT);
}

void DH3DEngine::SetDevice(void* Devie, void* DevieContext)
{
	DX11_Device = (ID3D11Device*)(Devie);
	DX11_Device_Context = (ID3D11DeviceContext*)(DevieContext);
	Effects::InitAll(DX11_Device);
	InputLayouts::InitAll(DX11_Device);
	m_AxisGrid = new AxisGrid(DX11_Device, DX11_Device_Context, DX11_Raster_State);
	m_AxisGrid->Initialize(150);
}

void DH3DEngine::EndDraw()
{
	// Present as fast as possible.
	DX11_Swap_Chain->Present(0, 0);
}

void DH3DEngine::Delete()
{
	/// 일련의 작업 후..
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (DX11_Swap_Chain)
	{
		DX11_Swap_Chain->SetFullscreenState(false, NULL);
	}

	if (DX11_Raster_State)
	{
		DX11_Raster_State->Release();
		DX11_Raster_State = 0;
	}

	if (DX11_Depth_Stencil_View)
	{
		DX11_Depth_Stencil_View->Release();
		DX11_Depth_Stencil_View = 0;
	}

	if (DX11_Depth_Stencil_State)
	{
		DX11_Depth_Stencil_State->Release();
		DX11_Depth_Stencil_State = 0;
	}

	if (DX11_Depth_Stencil_Buffer)
	{
		DX11_Depth_Stencil_Buffer->Release();
		DX11_Depth_Stencil_Buffer = 0;
	}

	if (DX11_Render_Target_View)
	{
		DX11_Render_Target_View->Release();
		DX11_Render_Target_View = 0;
	}

	if (DX11_Device_Context)
	{
		DX11_Device_Context->Release();
		DX11_Device_Context = 0;
	}

	if (DX11_Device)
	{
		DX11_Device->Release();
		DX11_Device = 0;
	}

	if (DX11_Swap_Chain)
	{
		DX11_Swap_Chain->Release();
		DX11_Swap_Chain = 0;
	}
}

float DH3DEngine::GetAspectRatio() const
{
	return static_cast<float>(g_Screen_Width) / g_Screen_Height;
}

TextureBuffer* DH3DEngine::CreateTextureBuffer(std::string path)
{
	// string to wstring
	std::wstring W_Path;
	W_Path.assign(path.begin(), path.end());

	TextureBuffer* _TextureBuffer = new TextureBuffer();
	
	// 텍스쳐 생성을 위한 임시 객체
	ID3D11Resource* Texture_Resource = nullptr;
	// 텍스쳐 SRV
	ID3D11ShaderResourceView* DX11_SRV = nullptr;
	// 텍스쳐 정보 셋팅.
	CreateWICTextureFromFile(DX11_Device, W_Path.c_str(), &Texture_Resource, &DX11_SRV);
	ReleaseCOM(Texture_Resource);

	_TextureBuffer->TextureBufferPointer = _TextureBuffer;
	
	return _TextureBuffer;

}

void DH3DEngine::OnReSize(int Change_Width, int Change_Height)
{

	/// 바뀐 화면의 사이즈를 저장해 준뒤에
	g_Screen_Width = (float)Change_Width;
	g_Screen_Height = (float)Change_Height;

	/// 화면을 나타내는 예전의 view를 삭제하고 현재것으로 새로 생성한다.
	assert(DX11_Device_Context);
	assert(DX11_Device);
	assert(DX11_Swap_Chain);

	/// 재구성 하는데 필요한 정보들 => 랜더타겟의 뷰, 스탠실 뷰,버퍼
	ReleaseCOM(DX11_Render_Target_View);
	ReleaseCOM(DX11_Depth_Stencil_View);
	ReleaseCOM(DX11_Depth_Stencil_Buffer);

	HR(DX11_Swap_Chain->ResizeBuffers(1, (UINT)g_Screen_Width, (UINT)g_Screen_Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(DX11_Swap_Chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(DX11_Device->CreateRenderTargetView(backBuffer, 0, &DX11_Render_Target_View));
	ReleaseCOM(backBuffer);

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = (UINT)g_Screen_Width;
	depthStencilDesc.Height = (UINT)g_Screen_Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	/// 4X MSAA 모드를 사용하지 않음.
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	/// 스탠실 Desc를 기반으로 하여 스탠실 버퍼와 뷰를 재생성한다.
	HR(DX11_Device->CreateTexture2D(&depthStencilDesc, 0, &DX11_Depth_Stencil_Buffer));
	HR(DX11_Device->CreateDepthStencilView(DX11_Depth_Stencil_Buffer, 0, &DX11_Depth_Stencil_View));


	/// 렌더타겟뷰, 뎁스/스탠실뷰를 파이프라인에 바인딩한다.
	DX11_Device_Context->OMSetRenderTargets(1, &DX11_Render_Target_View, DX11_Depth_Stencil_View);


	// Set the viewport transform.
	/// 뷰포트 변환을 셋팅한다.
	DX11_View_Port.TopLeftX = 0;
	DX11_View_Port.TopLeftY = 0;
	DX11_View_Port.Width = static_cast<float>(g_Screen_Width);
	DX11_View_Port.Height = static_cast<float>(g_Screen_Height);
	DX11_View_Port.MinDepth = 0.0f;
	DX11_View_Port.MaxDepth = 1.0f;

	DX11_Device_Context->RSSetViewports(1, &DX11_View_Port);

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	
	/// 창의 크기가 변했으므로, 종횡비를 업데이트하고 투영 행렬을 재계산한다.
	//m_DHCamera->SetLens(0.25f * MathHelper::Pi, GetAspectRatio(), 1.0f, 1000.0f);
}

Indexbuffer* DH3DEngine::CreateIndexBuffer(ParserData::Mesh* mModel)
{
	ID3D11Buffer* mIB = nullptr;
	Indexbuffer* indexbuffer = new Indexbuffer();

	//모델의 계수
	int Icount = (int)mModel->m_IndexList.size();

	std::vector<UINT> index;
	index.resize(Icount * 3);

	int j = 0;
	for (int i = 0; i < Icount; i++)
	{
		index[j] = mModel->m_IndexList[i]->m_Index[0];
		j++;
		index[j] = mModel->m_IndexList[i]->m_Index[1];
		j++;
		index[j] = mModel->m_IndexList[i]->m_Index[2];
		j++;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * Icount * 3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &index[0];

	HR(DX11_Device->CreateBuffer(&ibd, &iinitData, &mIB));

	indexbuffer->IndexBufferPointer = mIB;

	return indexbuffer;
}

Vertexbuffer* DH3DEngine::CreateVertexBuffer(ParserData::Mesh* mModel)
{
	ID3D11Buffer* mVB = nullptr;
	Vertexbuffer* vertexbuffer = new Vertexbuffer();

	//모델의 계수
	int Vcount = (int)mModel->m_VertexList.size();
	std::vector<DHVertex> temp;
	temp.resize(Vcount);
	for (int i = 0; i < Vcount; i++)
	{
		temp[i].Pos = mModel->m_VertexList[i]->m_Pos;
		temp[i].Normal = mModel->m_VertexList[i]->m_Normal;
		temp[i].Tex = mModel->m_VertexList[i]->m_UV;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(DHVertex) * Vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &temp[0];

	HR(DX11_Device->CreateBuffer(&vbd, &vinitData, &mVB));

	vertexbuffer->VertexbufferPointer = mVB;

	return vertexbuffer;
}

void DH3DEngine::CreateRenderStates()
{
	// Render State 중 Rasterizer State
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	HR(DX11_Device->CreateRasterizerState(&solidDesc, &mSolidRS));

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(DX11_Device->CreateRasterizerState(&wireframeDesc, &mWireframeRS));
}
