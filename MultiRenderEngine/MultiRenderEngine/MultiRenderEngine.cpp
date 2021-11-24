#include "MultiRenderEngine.h"
#include "EngineData.h"
#include "GraphicsEngine.h"
#include "MacroDefine.h"
#include "InputLayoutData.h"

#include <iostream>
#include <d3d11.h>


#ifdef _WIN64
#ifdef _DEBUG
#pragma comment( lib, "DirectXTK_x64d.lib" )
#else
#pragma comment( lib, "DirectXTK_x64r.lib" )
#endif
#endif

//텍스쳐용
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"


#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

MultiRenderEngine* MultiRenderEngine::m_Engine = nullptr;

MultiRenderEngine::MultiRenderEngine()
{
	m_Device = nullptr;
	m_DeviceContext = nullptr;
	m_SwapChain = nullptr;
	m_ViewPort = nullptr;

	m_RenderTargetView = nullptr;
	m_DepthStencilView = nullptr;

	m_ScreenHeight = 0;
	m_ScreenWidth = 0;

	Split_X_Count = 0;
	Split_Y_Count = 0;

	WindowCount = 0;
	m_Hwnd = NULL;

}

MultiRenderEngine::~MultiRenderEngine()
{


}

MultiRenderEngine* MultiRenderEngine::Initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	m_Engine = new MultiRenderEngine();

	//디바이스와 컨텍스트를 생성한다
	m_Engine->CreateDevice(hwnd, screenWidth, screenHeight);

	//스왑체인에 붙어있는 랜더타겟을 생성한다
	m_Engine->Create_SwapChain_RenderTarget();
	return m_Engine;
}

BOOL MultiRenderEngine::SplitWindow(int _Horizontal, int _Vertical)
{
	// 이미 쪼개서 생성한 경우.. (추후 어떻게 관리될진 모르겠지만 일단 막아놨음)
	if (!Split_Window.empty())
	{
		std::cout << "[MultiRenderEngine::SplitWindow() 오류] 이미 Split된 Window가 존재합니다. (중복호출)" << std::endl;
		return false;
	}
	// 지정된 최대 개수를 초과한 경우 false 리턴..
	if (_Horizontal > MAX_HORIZONTAL)
	{
		std::cout << "[MultiRenderEngine::SplitWindow() 오류]" << MAX_HORIZONTAL << " 줄 이상의 Horizontal 생성은 불가능합니다." << std::endl;
		return false;
	}
	if (_Vertical > MAX_VERTICAL)
	{
		std::cout << "[MultiRenderEngine::SplitWindow() 오류]" << MAX_VERTICAL << " 줄 이상의 Vertical 생성은 불가능합니다." << std::endl;
		return false;
	}
	if ((_Horizontal * _Vertical) > MAX_SPLIT_TARGET)
	{
		std::cout << "[MultiRenderEngine::SplitWindow() 오류]" << MAX_SPLIT_TARGET << " 개 이상의 Window Split은 불가능합니다." << std::endl;
		std::cout << " 현재 입력값 => Horizontal : " << _Horizontal << "\tVertical : " << _Vertical << std::endl;
		return false;
	}

	//뷰포트를 들어온 스플릿 값 만큼 설정해주고 생성한다
	ViewPortSetting(_Horizontal, _Vertical);

	return (BOOL)Split_Window.size();
}

BOOL MultiRenderEngine::RegisterRenderer(GraphicEngine* _Renderer, std::string _Engine_Name)
{
	// 이미 존재하는 엔진이라면 return false;
	if (Registered_Engine_List.find(_Engine_Name) != Registered_Engine_List.end())
	{
		std::cout << "[MultiRenderEngine::RegisterRenderer() 오류] 이미 등록된 Graphic Engine이 존재합니다. (중복호출)" << std::endl;
		return false;
	}

	// 할당받은 엔진을 리스트에 등록해놓는다.
	Registered_Engine_List.insert({ _Engine_Name, _Renderer });

	//디바이스와 디바이스 컨텍스트를 넣어준다
	_Renderer->SetDevice(m_Device, m_DeviceContext);
	return true;
}

BOOL MultiRenderEngine::SetRenderer(int _ViewPort_Number, std::string _Engine_Name)
{
	// 윈도우가 생성되어있지 않은경우 return false;
	if (Split_Window.empty())
	{
		std::cout << "[MultiRenderEngine::SetRenderer() 오류] 아직 Split Window 를 하지 않았습니다." << std::endl;
		return false;
	}

	/// 입력받은 뷰포트의 데이터를 받아온다.
	auto Split_Window_Data = Split_Window.find(_ViewPort_Number);
	// 만약 생성되지 않은 뷰포트 index를 호출하게된다면 false를 리턴.
	if (Split_Window_Data == Split_Window.end())
	{
		std::cout << "[MultiRenderEngine::SetRenderer() 오류] 잘못된 ViewPort Number을 호출하셨습니다." << std::endl;
		std::cout << " 현재 생성된 ViewPort 개수 => " << Split_Window.size() << std::endl;
		return false;
	}

	/// 입력받은 엔진의 데이터를 받아온다.
	auto Find_Engine_Ptr = Registered_Engine_List.find(_Engine_Name);
	// 만약 등록되지 않은 엔진을 호출한다면..
	if (Find_Engine_Ptr == Registered_Engine_List.end())
	{
		std::cout << "[MultiRenderEngine::SetRenderer() 오류] 등록되지 않은 Engine을 호출하셨습니다. 먼저 RegisterRenderer() 를 통해 그래픽 엔진을 등록해주세요." << std::endl;
		return false;
	}

	/// 해당하는 뷰포트에 엔진을 매칭시킨다.
	GraphicEngine* Target_Engine = Find_Engine_Ptr->second;
	Split_Window_Data->second.second = Target_Engine;


	/// 여기서 꼭!!!!!!!!!!!!!!!!!! 엔진에 ViewPort를 줘야함.
	D3D11_VIEWPORT* Target_ViewPort = Split_Window_Data->second.first;
	Target_Engine->SetViewPort(Target_ViewPort, m_ScreenWidth, m_ScreenHeight);
	// ex) Target_Engine->SetViewPort(Target_ViewPort); 이런식으로..


	return true;
}

MULTIENGINE_DLL BOOL MultiRenderEngine::OnResize(int Change_Width, int Change_Height)
{
	m_ScreenWidth = Change_Width;
	m_ScreenHeight = Change_Height;

	assert(m_DeviceContext);
	assert(m_Device);
	assert(m_SwapChain);

	//랜더타겟 삭제
	if (m_RenderTargetView != nullptr)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = nullptr;
	}

	//뎁스스텐실 삭제
	if (m_DepthStencilView != nullptr)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView = nullptr;
	}


	//스왑체인 재설정
	m_SwapChain->ResizeBuffers(1, m_ScreenWidth, m_ScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	//뎁스스텐실 랜더타겟 재생성
	Create_SwapChain_RenderTarget();

	//재설정
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	//현재 들어있는 엔진들의 뷰포트 설정을 다시해준다
	ViewPortSetting(Split_X_Count, Split_Y_Count, false);

	return true;
}

MULTIENGINE_DLL void MultiRenderEngine::ShadowRender(int count, std::queue<MeshData*>* meshList, GlobalData* global)
{
	if ((Split_Window[count]).second != nullptr)
	{
		(Split_Window[count]).second->ShadowRender(meshList, global);
	}
}

void MultiRenderEngine::Render(int count, std::queue<MeshData*>* meshList, GlobalData* global)
{
	if ((Split_Window[count]).second != nullptr)
	{
		(Split_Window[count]).second->Render(meshList, global);
	}

}

void MultiRenderEngine::Delete()
{
	Registered_Engine_List.clear();
	Split_Window.clear();
}



Vertexbuffer* MultiRenderEngine::CreateVertexBuffer(ParserData::Mesh* mModel)
{

	if (mModel->m_IsSkinningObject == true)
	{
		Vertexbuffer* vertexbuffer = SkinningVertexBuffer(mModel);
		return vertexbuffer;
	}
	else
	{
		Vertexbuffer* vertexbuffer = BasicVertexBuffer(mModel);
		return vertexbuffer;
	}

}

Indexbuffer* MultiRenderEngine::CreateIndexBuffer(ParserData::Mesh* mModel)
{
	ID3D11Buffer* mIB = nullptr;
	Indexbuffer* indexbuffer = new Indexbuffer();

	//모델의 계수
	int IndexFaceCount = (int)mModel->m_IndexList.size();
	int IndexCount = IndexFaceCount * 3;

	std::vector<UINT> index;
	index.resize(IndexCount);

	int j = 0;
	for (int i = 0; i < IndexFaceCount; i++)
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
	ibd.ByteWidth = sizeof(UINT) * IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &index[0];

	m_Device->CreateBuffer(&ibd, &iinitData, &mIB);

	indexbuffer->IndexBufferPointer = mIB;

	return indexbuffer;
}

TextureBuffer* MultiRenderEngine::CreateTextureBuffer(std::string path)
{
	TextureBuffer* buffer = nullptr;

	ID3D11Resource* texResource = nullptr;
	ID3D11ShaderResourceView* newTex = nullptr;

	//해당 문자열에 dds가있다면 (rfind 는 뒤에서부터 찾는다)
	if (path.rfind(".dds") != std::string::npos)
	{
		//문자열 변환
		std::wstring _path = std::wstring(path.begin(), path.end());
		const wchar_t* w_path = _path.c_str();

		//생성
		DirectX::CreateDDSTextureFromFile(m_Device, w_path, &texResource, &newTex);
	}
	else if (path.rfind(".png") != std::string::npos)
	{
		//문자열 변환
		std::wstring _path = std::wstring(path.begin(), path.end());
		const wchar_t* w_path = _path.c_str();

		//생성
		DirectX::CreateWICTextureFromFile(m_Device, w_path, &texResource, &newTex);
	}

	// Texture 생성 성공시 Texture Buffer 생성..
	if (newTex)
	{
		buffer = new TextureBuffer();
		buffer->TextureBufferPointer = newTex;

		texResource->Release();
	}

	return buffer;
}

void MultiRenderEngine::CreateDevice(HWND hwnd, int screenWidth, int screenHeight)
{
	//데이터 받기
	m_ScreenHeight = screenHeight;
	m_ScreenWidth = screenWidth;
	m_Hwnd = hwnd;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 스왑 체인 설정을 초기화..
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// swapChainDesc을 단일 백 버퍼로 설정..
	swapChainDesc.BufferCount = 1;

	// 백 버퍼의 너비와 높이를 설정..
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// 백 버퍼에 일반 32 비트 표면을 설정..
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 백 버퍼의 재생률을 설정..
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// 스캔 라인 순서 및 배율을 지정되지 않음으로 설정..
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 백 버퍼의 사용 설정..
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.OutputWindow = (HWND)hwnd;

	// 멀티 샘플링을 끕니다..
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가 플래그 설정 안 함..
	swapChainDesc.Flags = createDeviceFlags;

	// featureLevel을 DirectX 11로 설정..
	D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_0;

	// 스왑 체인, Direct3D 장치 및 Direct3D 장치 컨텍스트 생성..
	/// MSDN에선 SwapChain 과 Device 를 한번에 생성해주는 함수를 지향함..
	HR(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featurelevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext));
}

void MultiRenderEngine::Create_SwapChain_RenderTarget()
{
	///스왑체인과 연결된 랜더타겟과 뎁스스텐실을 생성

	ID3D11Texture2D* backBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	HR(m_Device->CreateRenderTargetView(backBuffer, 0, &m_RenderTargetView));
	backBuffer->Release();

	ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_ScreenWidth;
	depthStencilDesc.Height = m_ScreenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;


	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;


	m_Device->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	HR(m_Device->CreateDepthStencilView(mDepthStencilBuffer, 0, &m_DepthStencilView));
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

}

void MultiRenderEngine::Create_ViewPort(int KeyNumber, int StartX, int StartY, int Width, int Height)
{
	m_ViewPort = new D3D11_VIEWPORT();
	m_ViewPort->TopLeftX = (float)StartX;
	m_ViewPort->TopLeftY = (float)StartY;
	m_ViewPort->Width = static_cast<float>(Width);
	m_ViewPort->Height = static_cast<float>(Height);
	m_ViewPort->MinDepth = 0.0f;
	m_ViewPort->MaxDepth = 1.0f;

	Split_Window.insert({ KeyNumber, {m_ViewPort,nullptr } });
}

void MultiRenderEngine::ViewPortSetting(int m_Horizontal, int m_Vertical, BOOL Create)
{
	//뷰포트를 생성하면서  시작지점 X,Y 가로세로 길이를 설정해준다 
	WindowCount = m_Horizontal * m_Vertical;
	int Width = m_ScreenWidth / m_Horizontal;
	int Height = m_ScreenHeight / m_Vertical;
	int StartX = 0;
	int StartY = 0;
	int count = 0;

	Split_X_Count = m_Horizontal;
	Split_Y_Count = m_Vertical;

	for (int i = 0; i < m_Vertical; i++)
	{
		for (int j = 0; j < m_Horizontal; j++)
		{
			if (Create)
			{
				//뷰포트를 생성해서 넣어준다
				Create_ViewPort(count, StartX, StartY, Width, Height);
			}
			else
			{
				//뷰포트를 Resize한 값으로 변경해서 다시 넣어준다
				ReSetting_ViewPort(count, StartX, StartY, Width, Height);
			}

			if (StartX < m_ScreenWidth - Width)
			{
				StartX += Width;
			}
			else
			{
				StartX = 0;
			}
			count++;
		}

		if (StartY < m_ScreenHeight - Height)
		{
			StartY += Height;
		}
		else
		{
			StartY = 0;
		}
	}
}

void MultiRenderEngine::ReSetting_ViewPort(int count, int StartX, int StartY, int Width, int Height)
{
	m_ViewPort = Split_Window[count].first;
	m_ViewPort->TopLeftX = (float)StartX;
	m_ViewPort->TopLeftY = (float)StartY;
	m_ViewPort->Width = static_cast<float>(Width);
	m_ViewPort->Height = static_cast<float>(Height);
	m_ViewPort->MinDepth = 0.0f;
	m_ViewPort->MaxDepth = 1.0f;

	(Split_Window[count]).second->SetViewPort(m_ViewPort, m_ScreenWidth, m_ScreenHeight);
}

Vertexbuffer* MultiRenderEngine::BasicVertexBuffer(ParserData::Mesh* mModel)
{
	///스키닝이없는 기본 오브젝트를 생성해줌
	ID3D11Buffer* mVB = nullptr;
	Vertexbuffer* vertexbuffer = new Vertexbuffer();

	//포지션 , 노말, uv, 탄젠트 값만 읽어옴
	std::vector<MeshVertex> temp;
	int Vcount = (int)mModel->m_VertexList.size();
	temp.resize(Vcount);
	for (int i = 0; i < Vcount; i++)
	{
		temp[i].Pos = mModel->m_VertexList[i]->m_Pos;
		temp[i].Normal = mModel->m_VertexList[i]->m_Normal;
		temp[i].Tex = mModel->m_VertexList[i]->m_UV;
		temp[i].Tangent = mModel->m_VertexList[i]->m_Tanget;
	}


	//버퍼 생성
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(MeshVertex) * Vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &temp[0];
	m_Device->CreateBuffer(&vbd, &vinitData, &mVB);

	vertexbuffer->VertexbufferPointer = mVB;

	/////////////////////////////////////////////////// 중요함 꼭넣어주세요
	vertexbuffer->VertexDataSize = sizeof(MeshVertex);
	///////////////////////////////////////////////////

	return vertexbuffer;
}

Vertexbuffer* MultiRenderEngine::SkinningVertexBuffer(ParserData::Mesh* mModel)
{

	///스키닝이 오브젝트를 생성해줌
	ID3D11Buffer* mVB = nullptr;
	Vertexbuffer* vertexbuffer = new Vertexbuffer();

	//포지션 , 노말, uv, 탄젠타 값만 읽어옴
	std::vector<SkinVertex> temp;
	int Vcount = (int)mModel->m_VertexList.size();
	temp.resize(Vcount);

	for (int i = 0; i < Vcount; i++)
	{
		ParserData::Vertex* One = mModel->m_VertexList[i];

		temp[i].Pos = One->m_Pos;
		temp[i].Normal = One->m_Normal;
		temp[i].Tex = One->m_UV;
		temp[i].Tangent = One->m_Tanget;


		int Count = (int)One->m_BoneWeights.size();
		for (int j = 0; j < Count; j++)
		{
			if (j <= 3)
			{
				temp[i].BoneWeight1[j]	= One->m_BoneWeights[j];
				temp[i].BoneIndex1[j]	= One->m_BoneIndices[j];
			}
			else
			{
				temp[i].BoneWeight2[j - 4]	= One->m_BoneWeights[j];
				temp[i].BoneIndex2[j - 4]	= One->m_BoneIndices[j];
			}

		}


	}

	//버퍼 생성
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SkinVertex) * Vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &temp[0];
	m_Device->CreateBuffer(&vbd, &vinitData, &mVB);

	vertexbuffer->VertexbufferPointer = mVB;

	/////////////////////////////////////////////////// 중요함 꼭넣어주세요
	vertexbuffer->VertexDataSize = sizeof(SkinVertex);
	//////////////////////////////////////////////////

	return vertexbuffer;
}




void MultiRenderEngine::BeginRender()
{
	//클리어
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = 0.17f;
	color[1] = 0.17f;
	color[2] = 0.17f;
	color[3] = 1.0f;
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

}

void MultiRenderEngine::EndRender()
{
	m_SwapChain->Present(0, 0);
	//엔진 랜더링 종료
}

int MultiRenderEngine::GetWindowCount()
{
	return WindowCount;
}


