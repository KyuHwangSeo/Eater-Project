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

	void SetOption(const char* shaderName) override;

	void Reset();

	void BeginRender();
	void Render(GlobalData* global);

private:
	DrawBuffer* m_QuadBD;

	VertexShader* m_LightVS;
	PixelShader* m_LightPS; 
	
	ID3D11DepthStencilView* m_LightDSV;
	ID3D11RenderTargetView* m_MainRTV;
	ID3D11RenderTargetView* m_OitRTV;

	RenderTarget* m_AlbedoRT;
	RenderTarget* m_NormalRT;
	RenderTarget* m_PositionRT;
	RenderTarget* m_ShadowRT;

	D3D11_VIEWPORT* m_ScreenVP;
};

