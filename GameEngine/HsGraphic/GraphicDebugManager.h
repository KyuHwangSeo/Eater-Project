#pragma once
#include "InputLayout.h"
#include <vector>
#include <DirectXMath.h>
#include <string>
#include <map>
#include "d3d11.h"
using namespace DirectX;

/// <summary>
/// 더쉽게 게임을 제작하기위해 디버깅을 한다
/// </summary>

class BufferData;
class GraphicDebugManager
{
public:
	GraphicDebugManager();
	~GraphicDebugManager();

	//초기화
	void Initialize(ID3D11Device* Device, ID3D11DeviceContext* mContext);

public:
	//2D원모형으로 디버깅 버텍스 생성한다
	void Create_2D_Y_Circle_Buffers(XMFLOAT3 _Centor, float Radius, XMFLOAT3 Color = { 1,0,0 });
	void Create_2D_X_Circle_Buffers(XMFLOAT3 _Centor, float Radius, XMFLOAT3 Color = { 1,0,0 });
	void Create_2D_Z_Circle_Buffers(XMFLOAT3 _Centor, float Radius, XMFLOAT3 Color = { 1,0,0 });

	//2D사각형모형으로 디버깅버택스 생성
	void Create_2D_X_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT3 Color = { 1,0,0});
	void Create_2D_Y_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT3 Color = { 1,0,0});
	void Create_2D_Z_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT3 Color = { 1,0,0});

	//3D 박스 모형으로 버퍼를 생성한다
	void Create_3D_Box_Buffers(XMFLOAT3 _Centor, XMFLOAT3 _Extents, XMFLOAT3 Color = { 1,0,0});

	//3D 원형 모형으로 버퍼를 생성한다
	void Create_3D_Circle_Buffers(XMFLOAT3 _Centor, float Radius, XMFLOAT3 Color = { 1,0,0});

	//라인 모형으로 디버깅 버택스를 생성함
	void Create_2D_Line_Buffers(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT3 Color = { 1,0,0});

	//버퍼를 생성한다
	BufferData* CreateBuffer(std::string BufferName);

	//그리드버퍼를 가져온다
	BufferData* GetGridBuffer();
	//본버퍼를 가져온다
	BufferData* GetBoneBuffer();
private:
	//그리드를 그린다
	void Create_Grid(int CountX,int CountY);
	void Create_Bone();
private:
	ID3D11Device*		 m_Device;				//디바이스
	ID3D11DeviceContext* m_DeviceContext;		//디바이스 컨텍스트

	std::vector<Debug32>	vertices;
	std::vector<UINT>		indices;

	int vcount;
	int icount;

	BufferData* GridBuffer;
	BufferData* BoneBuffer;

	std::map<std::string, BufferData*> BufferList;
};