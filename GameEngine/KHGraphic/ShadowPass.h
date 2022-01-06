#pragma once
class ShadowPass : public RenderPassBase
{
public:
	ShadowPass();
	~ShadowPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;

	void SetOption(UINT renderOption) override;

public:
	void BeginRender();
	void Update(MeshData* mesh, GlobalData* global);
	void Render(MeshData* mesh);

private:
	VertexShader* m_MeshShadowVS;
	VertexShader* m_SkinShadowVS;

	DepthStencil* m_ShadowDS;
	
	ID3D11DepthStencilView* m_ShadowDSV;
	ID3D11RasterizerState* m_DepthRS;
	D3D11_VIEWPORT* m_ShadowVP;
};

