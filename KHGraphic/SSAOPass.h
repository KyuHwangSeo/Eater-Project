#pragma once

class SSAOPass : public RenderPassBase
{
public:
	SSAOPass();
	~SSAOPass();

public:
	void Create(int width, int height) override;
	void Start() override;
	void OnResize(int width, int height) override;
	void Release() override;

public:
	void BeginRender();
	void Render(GlobalData* global);
	void BlurRender(int blurCount);
	void BlurRender(bool horizon);

private:
	void SetOffsetVectors();
	void SetRandomVectorTexture();
	void SetFrustumFarCorners(int width, int height);

private:
	BufferData* m_SsaoBuffer;

	VertexShader* m_SsaoVS;
	PixelShader* m_SsaoPS;
	VertexShader* m_BlurVS;
	PixelShader* m_BlurPS;

	RenderTarget* m_SsaoRT;
	RenderTarget* m_SsaoBlurRT;
	RenderTarget* m_DepthRT;

	ID3D11RenderTargetView* m_SsaoRTV;
	ID3D11RenderTargetView* m_SsaoBlurRTV;

	CB_BlurOption m_BlurOption;



	D3D11_VIEWPORT* m_SsaoViewport;
};