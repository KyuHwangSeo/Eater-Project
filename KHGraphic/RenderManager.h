#pragma once
#include "RenderManagerBase.h"

class RenderPassBase;
class ForwardPass;
class ShadowPass;
class DeferredPass;
class LightPass;
class RenderManager : public IRenderManager
{
public:
	RenderManager(D3D11Graphic* graphic, IGraphicResourceFactory* factory);
	~RenderManager();

public:
	void Initialize(int width, int height) override;
	void Release() override;

	void Render(std::queue<MeshData*>* meshList, GlobalData* global) override;
	void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) override; 
	void SSAORender() override;
	void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) override;

	void OnResize(int width, int height) override;

public:
	D3D11_VIEWPORT* m_ViewPort;

private:
	std::vector<RenderPassBase*> m_RenderPassList;


	ForwardPass* m_Farward;
	ShadowPass* m_Shadow;
	DeferredPass* m_Deferred;
	LightPass* m_Light;
};
