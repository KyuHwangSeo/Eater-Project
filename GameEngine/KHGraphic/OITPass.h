#pragma once
class OITPass : public RenderPassBase
{
public:
	OITPass();
	~OITPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void SetResize(int width, int height) override;

	void BeginRender();
	void RenderUpdate();

private:
	UINT m_Multiple;

	DrawBuffer* m_QuadBD;
	
	VertexShader* m_OITRenderVS;
	PixelShader* m_OITRenderPS;
	PixelShader* m_OITParticlePS;

	ID3D11DepthStencilView* m_DefaltDSV;
	ID3D11DepthStencilView* m_OutputDSV;
	ID3D11DepthStencilState* m_NoDepthStencilDSS;
	ID3D11RasterizerState* m_NoCullRS;

	ID3D11RenderTargetView* m_MainRTV;

	ID3D11UnorderedAccessView* m_PieceLinkUAV;
	ID3D11UnorderedAccessView* m_FirstOffsetUAV;

	RenderBuffer* m_PieceLinkRB;
	RenderBuffer* m_FirstOffsetRB;
};

