#pragma once
class DebugPass : public RenderPassBase
{
public:
	DebugPass();
	~DebugPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;
	void OnResize(int width, int height) override;
	void Release() override;

	void Render();
};

