#pragma once


/// <summary>
/// 랜더타겟과 뎁스스텐실 쉐이더 리소스뷰를 관리하기위한 인터페이스 클래스
/// </summary>


struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

class TextureBase
{
public:
	TextureBase();
	~TextureBase();
	void Reset(int TextureSizeX = 256,int TextureSizeY= 256);

	void Create();
	void Create(D3D11_TEXTURE2D_DESC& RTV, D3D11_TEXTURE2D_DESC& DSV , D3D11_SHADER_RESOURCE_VIEW_DESC& SRV);
	void Delete();

	///Get
	const ID3D11DepthStencilView*	 GetDSV();
	const ID3D11RenderTargetView*	 GetRTV();
	const ID3D11ShaderResourceView*	 GetSRV();
protected:
	//디바이스 와 컨텍스트
	ID3D11Device*			Device;
	ID3D11DeviceContext*	mContext;

	//뎁스 스텐실
	ID3D11DepthStencilView*		mDSV;
	//랜더 타겟뷰
	ID3D11RenderTargetView*		mRTV;
	//쉐이더 리소스
	ID3D11ShaderResourceView*	mSRV;
	
	//사이즈
	int SizeX;
	int SizeY;
private:
	ID3D11Texture2D* CreateBasic_RenderTarget_Texture2D();
	ID3D11Texture2D* CreateBasic_DepthStencil_Texture2D();
	void Create_ShaderResourceView(ID3D11Texture2D* Tex);
};