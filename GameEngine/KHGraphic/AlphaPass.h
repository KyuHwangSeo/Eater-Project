#pragma once
class AlphaPass : public RenderPassBase
{
public:
	AlphaPass();
	~AlphaPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void BeginRender();
	void BufferUpdate(MeshData* mesh);
	void RenderUpdate(MeshData* mesh, GlobalData* global);

private:
	VertexShader* m_ParticleVS;
	PixelShader* m_ParticlePS;

	ID3D11RenderTargetView* m_MainRTV;
	ID3D11DepthStencilView* m_DefaltDSV;
	ID3D11DepthStencilState* m_DepthStencilState;
	ID3D11RasterizerState* m_NoCullRS;
	ID3D11BlendState* m_AlphaBlendBS;

	ID3D11Buffer* m_IndexBuffer;
	ID3D11Buffer* m_VertexBuffer;
	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;
};

