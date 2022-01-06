#pragma once

class SSAOPass : public RenderPassBase
{
public:
	SSAOPass();
	~SSAOPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;
	
	void SetOption(UINT renderOption) override;

public:
	void BeginRender();
	void Render(GlobalData* global);
	void BlurRender(int blurCount);
	void BlurRender(bool horizon);

private:
	void SetOffsetVectors();
	void SetFrustumFarCorners(int width, int height);

private:
	DrawBuffer* m_SsaoDB;

	VertexShader* m_SsaoVS;
	PixelShader* m_SsaoPS;
	VertexShader* m_BlurVS;
	PixelShader* m_BlurPS;

	RenderTexture* m_SsaoRT;
	RenderTexture* m_SsaoBlurRT;

	ID3D11RenderTargetView* m_SsaoRTV;
	ID3D11RenderTargetView* m_SsaoBlurRTV;

	ID3D11ShaderResourceView* m_SsaoSRV;
	ID3D11ShaderResourceView* m_SsaoBlurSRV;

	CB_BlurOption m_BlurOption;



	D3D11_VIEWPORT* m_SsaoVP;
};