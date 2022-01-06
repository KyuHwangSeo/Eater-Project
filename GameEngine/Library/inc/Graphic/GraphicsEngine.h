#pragma once
#include <windows.h>
#include <string>
#include <queue>
#include "GraphicDLL.h"

/// <summary>
/// ���� �������� �����ϴ� �׷��� ���� �ֻ��� Ŭ����
/// �ش� Ŭ������ ���� Graphic Engine�� ���� �� �ִ�..
/// </summary>

class MeshData;
class GlobalData;
class IndexBuffer;
class VertexBuffer;
class TextureBuffer;
class LoadMeshData;

namespace ParserData 
{
	class Mesh;
}

class GraphicEngine
{
protected:
	GraphicEngine() {};
	virtual ~GraphicEngine() {};

public:
	///�׷��� ������ ���� �� �ִ� �Լ�
	static GRAPHIC_DLL GraphicEngine* Create();

public:
	///�׷��� ������ ���ӿ������� �ְ�޾ƾ��� �Լ���
	virtual GRAPHIC_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight) abstract;	// �ʱ�ȭ �Լ�
	virtual GRAPHIC_DLL void OnReSize(int Change_Width, int Change_Height) abstract;				// ��������
	virtual GRAPHIC_DLL void Delete() abstract;

	virtual GRAPHIC_DLL void CreateMeshBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) abstract;	// ���۸� ������ش�
	virtual GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path) abstract;			// �ؽ��ĸ� ������ش�

	/// �������� �Ѵ� �Ž� ���������� ����Ʈ, �۷ι� ������
	virtual GRAPHIC_DLL void RenderSetting(UINT& renderOption) abstract;
	virtual GRAPHIC_DLL void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void Render(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void SSAORender(GlobalData* global) abstract;
	virtual GRAPHIC_DLL void AlphaRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void LightRender(GlobalData* global) abstract;
	virtual GRAPHIC_DLL void DebugRender(std::queue<MeshData*>* meshList, GlobalData* global) abstract;
	virtual GRAPHIC_DLL void EndRender() abstract;
};
