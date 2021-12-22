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
	virtual GRAPHIC_DLL void Initialize(HWND _hWnd, int screenWidth, int screenHeight) = 0;	// �ʱ�ȭ �Լ�
	virtual GRAPHIC_DLL void OnReSize(int Change_Width, int Change_Height) = 0;				// ��������
	virtual GRAPHIC_DLL void Delete() = 0;

	virtual GRAPHIC_DLL void CreateMeshBuffer(ParserData::Mesh* mesh, LoadMeshData* meshData) = 0;	// ���۸� ������ش�
	virtual GRAPHIC_DLL TextureBuffer* CreateTextureBuffer(std::string path)  = 0;			// �ؽ��ĸ� ������ش�

	/// �������� �Ѵ� �Ž� ���������� ����Ʈ, �۷ι� ������
	virtual GRAPHIC_DLL void BeginRender(UINT& renderOption) {}
	virtual GRAPHIC_DLL void ShadowRender(std::queue<MeshData*>* meshList, GlobalData* global) { return; }
	virtual GRAPHIC_DLL void Render(std::queue<MeshData*>* meshList, GlobalData* global) = 0;
	virtual GRAPHIC_DLL void SSAORender(GlobalData* global) { return; }
	virtual GRAPHIC_DLL void UIRender(std::queue<MeshData*>* meshList, GlobalData* global) { return; }
	virtual GRAPHIC_DLL void LightRender(GlobalData* global) { return; }
	virtual GRAPHIC_DLL void EndRender() { return; }
};
