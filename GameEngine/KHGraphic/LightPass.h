#pragma once
class LightPass : public RenderPassBase
{
public:
	LightPass();
	~LightPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void SetOption(UINT renderOption) override;

	void Reset();

	void BeginRender();
	void Render(GlobalData* global);

private:
	DrawBuffer* m_QuadBD;

	VertexShader* m_LightVS;
	PixelShader* m_LightPS; 
	
	ID3D11DepthStencilView* m_OutPutDSV;
	ID3D11RenderTargetView* m_OutPutRTV;

	RenderTexture* m_OutPutRT;

	RenderTexture* m_AlbedoRT;
	RenderTexture* m_NormalRT;
	RenderTexture* m_PositionRT;
	RenderTexture* m_ShadowRT;

	D3D11_VIEWPORT* m_ScreenVP;
};

