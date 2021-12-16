#include "GraphicDebugManager.h"
#include "HsGraphic.h"
#include "HsEngineHelper.h"
#include "ParserData.h"
#include "EngineData.h"
#include "ShaderManager.h"
#include "RenderingManager.h"
#include "BufferData.h"
#include "InputLayout.h"
#include "HsDefine.h"
#include "TextureBase.h"
#include <vector>
#include "Data.h"

using namespace DirectX;

HsGraphic::HsGraphic()
{
	hwnd = 0;
	Device				= nullptr;
	DeviceContext		= nullptr;
	mRenderTargetView	= nullptr;
	mDepthStencilView	= nullptr;
	mScreenViewport		= nullptr;
	mSwapChain			= nullptr;

	//윈도우 사이즈
	WinSizeX = 0;
	WinSizeY = 0;

	mWireframe	= nullptr;
	mSolid		= nullptr;

	mShaderManager	= nullptr;
	mRenderManager	= nullptr;
	mDebugManager	= nullptr;
}

HsGraphic::~HsGraphic()
{
	
}

void HsGraphic::Initialize(HWND _hWnd, int screenWidth, int screenHeight)
{
	WinSizeX	= screenWidth;
	WinSizeY	= screenHeight;
	hwnd		= _hWnd;

	//디바이스 생성
	CreateDevice();
	//뷰포트 생성
	Create_ViewPort();
	//스왑체인에서 렌더타겟을 생성
	Create_SwapChain_RenderTarget();


	//디바이스와 컨텍스트를 받은 동시에 각종 매니저 초기화
	mShaderManager = new ShaderManager();
	mShaderManager->Initialize(Device, DeviceContext);

	//디버깅 매니저 초기화
	mDebugManager = new GraphicDebugManager();
	mDebugManager->Initialize(Device, DeviceContext);

	//랜더링 매니저 초기화
	mRenderManager = new RenderingManager();
	mRenderManager->Initialize(Device, DeviceContext, mShaderManager, mDebugManager);
}

Indexbuffer* HsGraphic::CreateIndexBuffer(ParserData::Mesh* mModel)
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

	Device->CreateBuffer(&ibd, &iinitData, &mIB);

	indexbuffer->IndexBufferPointer = mIB;
	indexbuffer->Count = IndexCount * 3;

	return indexbuffer;
}

Vertexbuffer* HsGraphic::CreateVertexBuffer(ParserData::Mesh* mModel)
{
	//모델의 	
	Vertexbuffer* vertexbuffer;
	switch (mModel->m_MeshType)
	{
	case MESH_TYPE::SKIN_MESH:
		vertexbuffer = CreateSkinngingVertexBuffer(mModel);
		break;
	default:
		vertexbuffer = CreateBasicVertexBuffer(mModel);
		break;
	}

	return vertexbuffer;
}

Vertexbuffer* HsGraphic::CreateTerrainVertexBuffer(ParserData::Mesh* mModel, std::string maskName)
{
	return nullptr;
}

ID3D11RenderTargetView* HsGraphic::GetEngineRTV()
{
	return mRenderTargetView;
}

ID3D11DepthStencilView* HsGraphic::GetEngineDSV()
{
	return mDepthStencilView;
}

void HsGraphic::CreateDevice()
{
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
	swapChainDesc.BufferDesc.Width = WinSizeX;
	swapChainDesc.BufferDesc.Height = WinSizeY;

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
	(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featurelevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &Device, NULL, &DeviceContext));
}

void HsGraphic::BeginRender()
{
	XMVECTORF32 DeepDarkGray = { 0.15f, 0.15f, 0.15f, 1.0f };
	DeviceContext->ClearRenderTargetView(mRenderTargetView, DeepDarkGray);
	DeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void HsGraphic::EndRender()
{
	//엔진 랜더링 종료
	mSwapChain->Present(0, 0);
}

Vertexbuffer* HsGraphic::CreateBasicVertexBuffer(ParserData::Mesh* mModel)
{
	///스키닝이없는 기본 오브젝트를 생성해줌
	ID3D11Buffer* mVB = nullptr;
	Vertexbuffer* vertexbuffer = new Vertexbuffer();

	//포지션 , 노말, uv, 탄젠타 값만 읽어옴
	std::vector<Deferred32> temp;
	int Vcount = (int)mModel->m_VertexList.size();
	temp.resize(Vcount);
	for (int i = 0; i < Vcount; i++)
	{
		temp[i].Pos = mModel->m_VertexList[i]->m_Pos;
		temp[i].Nomal = mModel->m_VertexList[i]->m_Normal;
		temp[i].Tex = mModel->m_VertexList[i]->m_UV;
		temp[i].Tangent = mModel->m_VertexList[i]->m_Tanget;
	}

	//버퍼 생성
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Deferred32) * Vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &temp[0];
	Device->CreateBuffer(&vbd, &vinitData, &mVB);

	vertexbuffer->VertexbufferPointer = mVB;
	vertexbuffer->Count = Vcount;

	/////////////////////////////////////////////////// 중요함 꼭넣어주세요
	vertexbuffer->VertexDataSize = sizeof(Deferred32);
	//////////////////////////////////////////////////
	

	return vertexbuffer;
}

Vertexbuffer* HsGraphic::CreateSkinngingVertexBuffer(ParserData::Mesh* mModel)
{
	///스키닝이 오브젝트를 생성해줌
	ID3D11Buffer* mVB = nullptr;
	Vertexbuffer* vertexbuffer = new Vertexbuffer();

	//포지션 , 노말, uv, 탄젠타 값만 읽어옴
	std::vector<Skinning32> temp;
	int Vcount = (int)mModel->m_VertexList.size();
	temp.resize(Vcount);


	for (int i = 0; i < Vcount; i++)
	{
		ParserData::Vertex* One = mModel->m_VertexList[i];

		temp[i].Pos = One->m_Pos;
		temp[i].Nomal = One->m_Normal;
		temp[i].Tex = One->m_UV;
		temp[i].Tangent = One->m_Tanget;


		int Count = (int)One->m_BoneWeights.size();
		for (int j = 0; j < Count; j++)
		{
			if (j <= 3)
			{
				temp[i].BoneWeights01[j] = One->m_BoneWeights[j];
				temp[i].BoneIndex01[j] = One->m_BoneIndices[j];
			}
			else
			{
				temp[i].BoneWeights02[j - 4] = One->m_BoneWeights[j];
				temp[i].BoneIndex02[j - 4] = One->m_BoneIndices[j];
			}
		}

	}


	//버퍼 생성
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Skinning32) * Vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &temp[0];
	Device->CreateBuffer(&vbd, &vinitData, &mVB);

	vertexbuffer->VertexbufferPointer = mVB;
	vertexbuffer->Count = Vcount;

	/////////////////////////////////////////////////// 중요함 꼭넣어주세요
	vertexbuffer->VertexDataSize = sizeof(Skinning32);
	//////////////////////////////////////////////////

	return vertexbuffer;
}


TextureBuffer* HsGraphic::CreateTextureBuffer(std::string path)
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
		DirectX::CreateDDSTextureFromFile(Device, w_path, &texResource, &newTex);
	}
	else if (path.rfind(".png") != std::string::npos)
	{
		//문자열 변환
		std::wstring _path = std::wstring(path.begin(), path.end());
		const wchar_t* w_path = _path.c_str();

		//생성
		DirectX::CreateWICTextureFromFile(Device, w_path, &texResource, &newTex);
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

void HsGraphic::OnReSize(int Change_Width, int Change_Height)
{
	WinSizeX = Change_Width;
	WinSizeY = Change_Height;

	assert(DeviceContext);
	assert(Device);
	assert(mSwapChain);

	//랜더타겟 삭제
	if (mRenderTargetView != nullptr)
	{
		mRenderTargetView->Release();
		mRenderTargetView = nullptr;
	}

	//뎁스스텐실 삭제
	if (mDepthStencilView != nullptr)
	{
		mDepthStencilView->Release();
		mDepthStencilView = nullptr;
	}


	//스왑체인 재설정
	mSwapChain->ResizeBuffers(1, WinSizeX, WinSizeY, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	//뎁스스텐실 랜더타겟 재생성
	//CreateRenderTarget();

	//재설정
	DeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	DeviceContext->RSSetViewports(1, mScreenViewport);
}

void HsGraphic::Render(std::queue<MeshData*>* meshList, GlobalData* global)
{
	//멀티 랜더링 엔진에서 받아온 DSV, RTV, VPT로 그릴준비
	BeginRender();

	DeviceContext->RSSetViewports(1, mScreenViewport);

	while (meshList->size() != 0)
	{
		//가장 먼저들어온 오브젝트를 가져온다
		MeshData* Mesh = meshList->front();
		
		switch (Mesh->ObjType)
		{
			case OBJECT_TYPE::CAMERA: //카메라 오브젝트
			{
				mRenderManager->CameraUpdate(global);
				#ifdef _DEBUG
				mRenderManager->DebugUpdate();
				#endif
				break;
			}

			case OBJECT_TYPE::SKINNING: //스키닝 매쉬
			{
				mRenderManager->SkinningUpdate(Mesh);
				mRenderManager->Rendering(Mesh,RenderingManager::ShaderType::SKINNING);
				break;
			}

			case OBJECT_TYPE::BASE: //기본 매쉬
			{
				mRenderManager->MeshUpdate(Mesh);
				mRenderManager->Rendering(Mesh, RenderingManager::ShaderType::BASIC);
				break;
			}

			case OBJECT_TYPE::BONE: //기본 매쉬
			{
				mRenderManager->BoneUpdate(Mesh);
				break;
			}
		}


		//사용한것은 뺴준다
		meshList->pop();
	}
	
	EndRender();
}

void HsGraphic::Delete()
{
	mWireframe->Release();
	mSolid->Release();
}

void HsGraphic::SetViewPort(void* VPT, int Change_Width, int Change_Height)
{
	//랜더타겟과 뎁스스텐실 뷰포트를 받아온다
	mScreenViewport		= reinterpret_cast<D3D11_VIEWPORT*>(VPT);

	WinSizeX = Change_Width;
	WinSizeY = Change_Height;
}
	
void HsGraphic::SetDevice(void* mDevie, void* mDevieContext)
{
	//디바이스와 컨텍스트를 받아온다 
	Device			= reinterpret_cast<ID3D11Device*>(mDevie);
	DeviceContext	= reinterpret_cast<ID3D11DeviceContext*>(mDevieContext);


	//디바이스와 컨텍스트를 받은 동시에 각종 매니저 초기화
	mShaderManager = new ShaderManager();
	mShaderManager->Initialize(Device, DeviceContext);

	//디버깅 매니저 초기화
	mDebugManager = new GraphicDebugManager();
	mDebugManager->Initialize(Device, DeviceContext);

	//랜더링 매니저 초기화
	mRenderManager = new RenderingManager();
	mRenderManager->Initialize(Device, DeviceContext, mShaderManager, mDebugManager);
}
void HsGraphic::Create_SwapChain_RenderTarget()
{
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	Device->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	backBuffer->Release();

	ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = WinSizeX;
	depthStencilDesc.Height = WinSizeY;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;

	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	Device->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	Device->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
	DeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}
void HsGraphic::Create_ViewPort()
{
	mScreenViewport = new D3D11_VIEWPORT();
	mScreenViewport->TopLeftX = (float)0;
	mScreenViewport->TopLeftY = (float)0;
	mScreenViewport->Width = static_cast<float>(WinSizeX);
	mScreenViewport->Height = static_cast<float>(WinSizeY);
	mScreenViewport->MinDepth = 0.0f;
	mScreenViewport->MaxDepth = 1.0f;
}
