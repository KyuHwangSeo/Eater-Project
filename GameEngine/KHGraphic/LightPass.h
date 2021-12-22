#pragma once
class BufferData;

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
	BufferData* m_ScreenBuffer;

	VertexShader* m_LightVS;
	PixelShader* m_LightPS; 
	
	DepthStencil* m_DSV;
	ID3D11DepthStencilView* m_DepthStencilView;

	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11RasterizerState* m_RasterizerState;
	ID3D11BlendState* m_BlendState;

	RenderTarget* m_BackBuffer;

	ID3D11RenderTargetView* m_BackBufferRTV;

	RenderTarget* m_AlbedoRT;
	RenderTarget* m_NormalRT;
	RenderTarget* m_PositionRT;
	RenderTarget* m_ShadowRT;

	D3D11_VIEWPORT* m_ScreenViewport;
};

