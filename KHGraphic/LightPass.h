#pragma once
class BufferData;

class LightPass : public RenderPassBase
{
public:
	LightPass();
	~LightPass();

public:
	void Create(int width, int height) override;
	void Start() override;

	void OnResize(int width, int height) override;
	void Release() override;

	void BeginRender();
	void Render(GlobalData* global);

private:
	BufferData* m_ScreenBuffer;

	VertexShader* m_LightVS;
	PixelShader* m_LightPS; 
	
	DepthStencilView* m_DSV;
	ID3D11DepthStencilView* m_DepthStencilView;

	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11RasterizerState* m_RasterizerState;
	ID3D11BlendState* m_BlendState;

	BasicRenderTarget* m_BackBuffer;

	ID3D11RenderTargetView* m_BackBufferRTV;
	ID3D11ShaderResourceView* m_BackBufferSRV;

	BasicRenderTarget* m_AlbedoRT;
	BasicRenderTarget* m_NormalRT;
	BasicRenderTarget* m_PositionRT;
	BasicRenderTarget* m_ShadowRT;
	BasicRenderTarget* m_SSAORT;

	D3D11_VIEWPORT* m_ScreenViewport;
};

