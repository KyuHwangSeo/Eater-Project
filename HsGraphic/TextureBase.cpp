#include "d3d11.h"
#include "TextureBase.h"

TextureBase::TextureBase()
{
	mDSV = nullptr;
	mRTV = nullptr;
	mSRV = nullptr;

	Device		= nullptr;
	mContext	= nullptr;
}

TextureBase::~TextureBase()
{
	mDSV = nullptr;
	mRTV = nullptr;
	mSRV = nullptr;

	Device	 = nullptr;
	mContext = nullptr;
}

void TextureBase::Reset(int TextureSizeX, int TextureSizeY)
{
	//텍스쳐를 삭제하고 크기조절후 다시생성
	SizeX = TextureSizeX;
	SizeY = TextureSizeY;

	Delete();
	Create();
}

void TextureBase::Create()
{
	//랜더타겟과 뎁스스텐실의 기본 텍스쳐 2D를 생성해준다
	ID3D11Texture2D* RenderTarget_Texture2D = CreateBasic_RenderTarget_Texture2D();
	ID3D11Texture2D* DepthStencil_Texture2D = CreateBasic_DepthStencil_Texture2D();
	

	///생성
	Device->CreateRenderTargetView(RenderTarget_Texture2D, 0, &mRTV);	//랜더타겟
	Device->CreateDepthStencilView(DepthStencil_Texture2D, 0, &mDSV);	//뎁스 스텐실
	Create_ShaderResourceView(RenderTarget_Texture2D);					//쉐이더 리소스뷰


	//Texture2D는 삭제
	if (RenderTarget_Texture2D != nullptr)
	{
		RenderTarget_Texture2D->Release();
	}

	if (DepthStencil_Texture2D != nullptr)
	{
		DepthStencil_Texture2D->Release();
	}
}

void TextureBase::Create(D3D11_TEXTURE2D_DESC& RTV, D3D11_TEXTURE2D_DESC& DSV, D3D11_SHADER_RESOURCE_VIEW_DESC& SRV)
{
	ID3D11Texture2D* R = nullptr;
	ID3D11Texture2D* D = nullptr;

	Device->CreateTexture2D(&RTV, 0, &R);
	Device->CreateTexture2D(&DSV, 0, &D);
	Device->CreateShaderResourceView(R, &SRV, &mSRV);


	if (R != nullptr)
	{
		R->Release();
	}
	
	if(D != nullptr)
	{
		D->Release();
	}
}

void TextureBase::Delete()
{
	//뎁스 스텐실 삭제
	if (mDSV != nullptr)
	{
		mDSV->Release();
	}

	//랜더타겟 리소스뷰 삭제
	if (mRTV != nullptr)
	{
		mRTV->Release();
	}

	//쉐이더 리소스뷰 삭제
	if (mSRV != nullptr)
	{
		mSRV->Release();
	}
}

const ID3D11DepthStencilView* TextureBase::GetDSV()
{
	return mDSV;
}

const ID3D11RenderTargetView* TextureBase::GetRTV()
{
	return mRTV;
}

const ID3D11ShaderResourceView* TextureBase::GetSRV()
{
	return mSRV;
}

ID3D11Texture2D* TextureBase::CreateBasic_RenderTarget_Texture2D()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = SizeX;	//텍스쳐가로 
	texDesc.Height = SizeY;	//텍스쳐세로
	texDesc.MipLevels	= 1;							//밉맵수준 (깊이 스텐실 버퍼를 위한텍스쳐에서는 하나만있으면된다)
	texDesc.ArraySize	= 1;							//택스쳐 배열의택스쳐 개수 (깊이 스텐실 버퍼를 위한텍스쳐에서는 하나만있으면된다)
	texDesc.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;	//픽셀의 형식을 뜻함
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	//랜더타겟에 사용될 텍스쳐 2D를 생성함
	ID3D11Texture2D* Tex;
	Device->CreateTexture2D(&texDesc, 0,&Tex);

	return Tex;
}

ID3D11Texture2D* TextureBase::CreateBasic_DepthStencil_Texture2D()
{
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width		= SizeX;				//텍스쳐가로 
	depthDesc.Height	= SizeY;				//텍스쳐세로
	depthDesc.MipLevels = 1;					//밉맵수준 (깊이 스텐실 버퍼를 위한텍스쳐에서는 하나만있으면된다)
	depthDesc.ArraySize = 1;					//택스쳐 배열의택스쳐 개수 (깊이 스텐실 버퍼를 위한텍스쳐에서는 하나만있으면된다)
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;	//픽셀의 형식을 뜻함
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	//랜더타겟에 사용될 텍스쳐 2D를 생성함
	ID3D11Texture2D* Tex2D;
	Device->CreateTexture2D(&depthDesc, 0, &Tex2D);

	return Tex2D;
}

void TextureBase::Create_ShaderResourceView(ID3D11Texture2D* Tex)
{
	//쉐이더 리소스뷰를 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;

	Device->CreateShaderResourceView(Tex, &srvDesc, &mSRV); 
}
