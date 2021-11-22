#include "GraphicsEngine.h"
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
}

HsGraphic::~HsGraphic()
{
	
}

void HsGraphic::Initialize(HWND _hWnd, int screenWidth, int screenHeight)
{
	
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

	return indexbuffer;
}

Vertexbuffer* HsGraphic::CreateVertexBuffer(ParserData::Mesh* mModel)
{
	//모델의 	
	Vertexbuffer* vertexbuffer;
	if (mModel->m_IsSkinningObject == true)
	{
		vertexbuffer = CreateSkinngingVertexBuffer(mModel);
	}
	else
	{
		vertexbuffer = CreateBasicVertexBuffer(mModel);
	}

	return vertexbuffer;
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
	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice
	(
		0,							//디스플레이 어뎁터  0 = 디폴트 
		D3D_DRIVER_TYPE_HARDWARE,	// ???
		0,							// 소프트웨어 구동기 지정
		createDeviceFlags,			// ???
		0,							// 점검순서
		0,							// ???
		D3D11_SDK_VERSION,			//항상 D3D11_SDK_VERSION 을 지정
		&Device,					//함수가 생성한장치 돌려줌
		&featureLevel,				//null 값으로 한경우 지원하는 가장 높은 기능수준으로
		&DeviceContext			//원래값 돌려줌
	);

	//생성 여부
	if (FAILED(hr))
	{
		return;
	}

	//버전확인
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		//MessageBox(0, L"Direct3D LEVEL 11", 0, MB_OK);
		return;
	}


	//4xMSAA 품질 수준 지원 점검??
	//하드웨어가 4xMSAA를 위한 품질 수준을 지원하는지 점검

	UINT m4xMsaaQuality;
	Device->CheckMultisampleQualityLevels
	(
		DXGI_FORMAT_R8G8_B8G8_UNORM,
		4,
		&m4xMsaaQuality
	);

	//assert를 몰라서 써놓음
	//assert는 조건이 맞지않을때 프로그램을 중단하며 참일때는 계속 실행시킴
	//assert(m4xMsaaQuality > 0);

	//교환 사슬 설정?
	//구조체 값 설정
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	//BufferDesc = 생성하고자 하는 후면 버퍼의 속성들을 서술하는 개별적인 구조체

	sd.BufferCount = 1;  //교환 사슬에서 사용할 후면 버퍼의 개수
	sd.BufferDesc.Width = WinSizeX;
	sd.BufferDesc.Height = WinSizeY;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //버퍼의 용도
	sd.OutputWindow = hwnd; //랜더링 결과를 표시할 창의 핸들

	//sampleDesc = 다중 표본화를 이해 추출할 표본 개수와 품질수준
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;	//
	sd.Windowed = true;			//윈도우 창모드를 원하면 true 전체모드 false

	//교환 사슬의 생성

	//이과정에서 오류를 피하려면 장치의 생성에 쓰인 IDXGIFactory 인스턴스를 사용해야한다
	//그리고 그 인스턴스를 얻으려면 다음과 같이 일련의 COM 질의 과정을 거쳐야함..
	IDXGIDevice* dxgiDevice = 0;
	Device->QueryInterface
	(
		__uuidof(IDXGIDevice),
		(void**)&dxgiDevice
	);


	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent
	(
		__uuidof(IDXGIAdapter),
		(void**)&dxgiAdapter
	);

	//IDXGIFactory 인터페이스 얻음
	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent
	(
		__uuidof(IDXGIFactory),
		(void**)&dxgiFactory
	);

	//사슬 교환 생성
	dxgiFactory->CreateSwapChain(Device, &sd, &mSwapChain);

	//필요없는건 삭제
	dxgiAdapter->Release();
	dxgiDevice->Release();
	dxgiFactory->Release();
}

void HsGraphic::BeginRender()
{
	XMVECTORF32 DeepDarkGray = { 0, 0, 0, 1.0f };
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
	int Vcount = mModel->m_VertexList.size();
	temp.resize(Vcount);
	for (int i = 0; i < Vcount; i++)
	{
		temp[i].Pos = mModel->m_VertexList[i]->m_Pos;
		temp[i].Nomal = mModel->m_VertexList[i]->m_Normal;
		temp[i].Tex = { mModel->m_VertexList[i]->m_U ,mModel->m_VertexList[i]->m_V };
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

	/////////////////////////////////////////////////// 중요함 꼭넣어주세요
	vertexbuffer->VertexDataSize = sizeof(Deferred32);
	//////////////////////////////////////////////////
	

	return nullptr;
}

Vertexbuffer* HsGraphic::CreateSkinngingVertexBuffer(ParserData::Mesh* mModel)
{
	///스키닝이 오브젝트를 생성해줌
	ID3D11Buffer* mVB = nullptr;
	Vertexbuffer* vertexbuffer = new Vertexbuffer();

	//포지션 , 노말, uv, 탄젠타 값만 읽어옴
	std::vector<Skinning32> temp;
	int Vcount = mModel->m_VertexList.size();
	temp.resize(Vcount);


	for (int i = 0; i < Vcount; i++)
	{
		ParserData::Vertex* One = mModel->m_VertexList[i];

		temp[i].Pos = One->m_Pos;
		temp[i].Nomal = One->m_Normal;
		temp[i].Tex = { One->m_U ,One->m_V };
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

	/////////////////////////////////////////////////// 중요함 꼭넣어주세요
	vertexbuffer->VertexDataSize = sizeof(Skinning32);
	//////////////////////////////////////////////////

	return vertexbuffer;
}


TextureBuffer* HsGraphic::CreateTextureBuffer(std::string path)
{
	ID3D11ShaderResourceView* Textures = nullptr;
	ID3D11Resource* texResource = nullptr;


	std::wstring _path = std::wstring(path.begin(), path.end());
	const wchar_t* w_path = _path.c_str();

	if (CreateDDSTextureFromFile(Device, w_path, &texResource, &Textures, 0))
	{
		return nullptr;
	}
	
	TextureBuffer* buffer = new TextureBuffer();
	buffer->TextureBufferPointer = Textures;
	texResource->Release();

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
	//BeginRender();

	DeviceContext->RSSetViewports(1, mScreenViewport);

	while (meshList->size() != 0)
	{
		//가장 먼저들어온 오브젝트를 가져온다
		MeshData* Mesh = meshList->front();
		
		switch (Mesh->ObjType)
		{
			case OBJECT_TYPE::Camera: //카메라 오브젝트
			{
				mRenderManager->CameraUpdate(global);
				#ifdef _DEBUG
				mRenderManager->DebugUpdate();
				#endif
				break;
			}

			case OBJECT_TYPE::Skinning: //스키닝 매쉬
			{
				mRenderManager->SkinningUpdate(Mesh);
				mRenderManager->Rendering(Mesh,RenderingManager::ShaderType::SKINNING);
				break;
			}

			case OBJECT_TYPE::Base: //기본 매쉬
			{
				mRenderManager->MeshUpdate(Mesh);
				mRenderManager->Rendering(Mesh, RenderingManager::ShaderType::BASIC);
				break;
			}

			case OBJECT_TYPE::Bone: //기본 매쉬
			{
				mRenderManager->BoneUpdate(Mesh);
				//mRenderManager->Rendering(Mesh, RenderingManager::ShaderType::BASIC);
				break;
			}
		}


		//사용한것은 뺴준다
		meshList->pop();
	}
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


