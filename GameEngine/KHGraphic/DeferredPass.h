#pragma once
class DeferredPass : public RenderPassBase
{
public:
	DeferredPass();
	~DeferredPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void SetOption(UINT renderOption);

	void BeginRender();
	void BufferUpdate(MeshData* mesh);
	void RenderUpdate(MeshData* mesh, GlobalData* global);

private:
	VertexShader* m_MeshVS;
	VertexShader* m_SkinVS;
	VertexShader* m_TerrainVS;
	VertexShader* m_ParticleVS;

	PixelShader* m_DeferredPS;
	PixelShader* m_TerrainPS;
	PixelShader* m_ParticlePS;

	ID3D11DepthStencilView* m_DefaltDSV;
	
	ID3D11DepthStencilState* m_DefaltDSS;
	ID3D11RasterizerState* m_SolidRS;
	ID3D11BlendState* m_AlphaBlendBS;

	RenderTexture* m_AlbedoRT;
	RenderTexture* m_NormalRT;
	RenderTexture* m_PositionRT;
	RenderTexture* m_ShadowRT;
	RenderTexture* m_DepthRT;

	std::vector<ID3D11ShaderResourceView*> m_SRVList;
	std::vector<ID3D11RenderTargetView*> m_RTVList;

	ID3D11Buffer* m_IndexBuffer;
	ID3D11Buffer* m_VertexBuffer;
	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;

	D3D11_VIEWPORT* m_ScreenVP;
};

