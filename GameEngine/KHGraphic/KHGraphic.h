#pragma once
#include "GraphicsEngine.h"

interface IRenderManager;
interface IGraphicResourceFactory;

class KHGraphic : public GraphicEngine
{
public:
	KHGraphic();
	~KHGraphic();

public:
	void Initialize(HWND hwnd, int screenWidth, int screenHeight) override;

	void RenderSetting(UINT& renderOption) override;
	void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;
	void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) override;
	void SSAORender(GlobalData* global) override;
	void AlphaRender(std::queue<MeshData*>* meshList, GlobalData* global) override;
	void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) override;
	void LightRender(GlobalData* global) override;
	void DebugRender(std::queue<MeshData*>* meshList, GlobalData* global) override;
	void EndRender() override;

	void OnReSize(int screenWidth, int screenheight) override;
	void Delete() override;

	void CreateMeshBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) override;
	TextureBuffer* CreateTextureBuffer(std::string path) override;

private:
	IGraphicResourceFactory* m_ResourceFactory;

	IRenderManager* m_RenderManager;
};

