#pragma once
typedef enum DEBUG_TYPE
{
	DEBUG_AXIS,
	DEBUG_BOX,
	DEBUG_CIRCLE,
	DEBUG_QUAD,
	DEBUG_CAMERA,
	DEBUG_LIGHT
}DEBUG_TYPE;

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

	void Render(MeshData* mesh, GlobalData* global);
	void BufferUpdate(DEBUG_TYPE type);
private:
	VertexShader* m_DebugVS;
	PixelShader* m_DebugPS;

	DrawBuffer* m_QuadBuffer;
	DrawBuffer* m_AxisBuffer;
	DrawBuffer* m_BoxBuffer;
	DrawBuffer* m_CircleBuffer;

	DrawBuffer* m_DebugBuffer;


};

