#include "GraphicDebugManager.h"
#include "HsDefine.h"
#include "Data.h"

GraphicDebugManager::GraphicDebugManager()
{
	m_Device		= nullptr;
	m_DeviceContext = nullptr;

	vcount = 0;
	icount = 0;

	GridBuffer = nullptr;
}

GraphicDebugManager::~GraphicDebugManager()
{
	std::map<std::string, BufferData*>::iterator it = BufferList.begin();
	for (it; it != BufferList.end(); it++)
	{
		if (it->second == nullptr){continue;}

		delete it->second;
	}

	BufferList.clear();
}

void GraphicDebugManager::Initialize(ID3D11Device* Device, ID3D11DeviceContext* mContext)
{
	m_Device		= Device;
	m_DeviceContext = mContext;


	Create_Grid(20, 20);
	Create_Bone();
}

void GraphicDebugManager::Create_2D_Y_Circle_Buffers(XMFLOAT3 _Centor, float Radius, XMFLOAT3 Color)
{
	int verCount = 180;
	int inCount = verCount * 2;
	int inSize = (int)vertices.size();

	Debug32 temp00[180];
	UINT in_temp00[360];
	for (int i = 0; i < verCount; i++)
	{
		float angle = (float)(i * 2);
		float x =(float)(Radius * cos(angle * 3.14f / 180));
		float z = (float)(Radius * -sin(angle * 3.14f / 180));
		float y = 0;

		temp00[i].Pos = { _Centor.x + x,_Centor.y + y,_Centor.z + z };
		temp00[i].Color = Color;
	}
	for (int i = 0; i < verCount; i++)
	{
		vertices.push_back(temp00[i]);
	}

	int num = 1;
	for (int i = 1; i < inCount; i += 2)
	{
		in_temp00[i] = num + inSize;
		in_temp00[i + 1] = num + inSize;
		num++;
	}
	in_temp00[0] = 0 + inSize;
	in_temp00[inCount - 1] = 0 + inSize;
	for (int i = 0; i < inCount; i++)
	{
		indices.push_back(in_temp00[i]);
	}
}

void GraphicDebugManager::Create_2D_X_Circle_Buffers(XMFLOAT3 _center, float Radius, XMFLOAT3 Color)
{
	int verCount = 180;
	int inCount = verCount * 2;


	Debug32 temp[180];
	UINT in_temp[360];

	int inSize = (int)vertices.size();

	for (int i = 0; i < verCount; i++)
	{
		float angle = (float)(i * 2);
		float x = (float)(Radius * cos(angle * 3.14f / 180));
		float y = (float)(Radius * -sin(angle * 3.14f / 180));
		float z = 0;

		temp[i].Pos = { _center.x + x,_center.y + y,_center.z + z };
		temp[i].Color = Color;
	}
	for (int i = 0; i < verCount; i++)
	{
		vertices.push_back(temp[i]);
	}

	int num = 1;
	for (int i = 1; i < inCount; i += 2)
	{
		in_temp[i] = num + inSize;
		in_temp[i + 1] = num + inSize;
		num++;
	}
	in_temp[0] = 0 + inSize;
	in_temp[inCount - 1] = 0 + inSize;
	for (int i = 0; i < inCount; i++)
	{
		indices.push_back(in_temp[i]);
	}
}

void GraphicDebugManager::Create_2D_Z_Circle_Buffers(XMFLOAT3 _center, float Radius, XMFLOAT3 Color)
{
	int verCount = 180;
	int inCount = verCount * 2;
	int inSize = (int)vertices.size();

	Debug32 temp00[180];
	UINT in_temp00[360];
	for (int i = 0; i < verCount; i++)
	{
		float angle = (float)(i * 2);
		float y = (float)(Radius * cos(angle * 3.14f / 180));
		float z = (float)(Radius * -sin(angle * 3.14f / 180));
		float x = 0;

		temp00[i].Pos = { _center.x + x,_center.y + y,_center.z + z };
		temp00[i].Color = Color;
	}
	for (int i = 0; i < verCount; i++)
	{
		vertices.push_back(temp00[i]);
	}

	int num = 1;
	for (int i = 1; i < inCount; i += 2)
	{
		in_temp00[i] = num + inSize;
		in_temp00[i + 1] = num + inSize;
		num++;
	}
	in_temp00[0] = 0 + inSize;
	in_temp00[inCount - 1] = 0 + inSize;
	for (int i = 0; i < inCount; i++)
	{
		indices.push_back(in_temp00[i]);
	}
}

void GraphicDebugManager::Create_2D_X_Rect_Buffer(XMFLOAT3 _center, XMFLOAT2 _Extents, XMFLOAT3 Color)
{
	Debug32 temp[4];
	temp[0].Pos = { _Extents.x + _center.x,_Extents.y + _center.y,_center.z };
	temp[0].Color = Color;

	temp[1].Pos = { -_Extents.x + _center.x, _Extents.y + _center.y,_center.z };
	temp[1].Color = Color;


	temp[2].Pos = { _Extents.x + _center.x,-_Extents.y + _center.y, _center.z };
	temp[2].Color = Color;

	temp[3].Pos = { -_Extents.x + _center.x,-_Extents.y + _center.y, _center.z };
	temp[3].Color = Color;


	for (int i = 0; i < 4; i++)
	{
		vertices.push_back(temp[i]);
	}

	int inSize = (int)indices.size();
	UINT in_temp[8];
	in_temp[0] = 0 + inSize;		in_temp[1] = 1 + inSize;
	in_temp[2] = 2 + inSize;		in_temp[3] = 3 + inSize;
	in_temp[4] = 0 + inSize;		in_temp[5] = 2 + inSize;
	in_temp[6] = 1 + inSize;		in_temp[7] = 3 + inSize;

	for (int i = 0; i < 8; i++)
	{
		indices.push_back(in_temp[i]);
	}
}

void GraphicDebugManager::Create_2D_Y_Rect_Buffer(XMFLOAT3 _center, XMFLOAT2 _Extents, XMFLOAT3 Color)
{
	Create_2D_Line_Buffers(XMFLOAT3(_center.x + _Extents.x, _center.y, _center.z + _Extents.y), XMFLOAT3(_center.x - _Extents.x, _center.y, _center.z + _Extents.y), Color);
	Create_2D_Line_Buffers(XMFLOAT3(_center.x + _Extents.x, _center.y, _center.z - _Extents.y), XMFLOAT3(_center.x - _Extents.x, _center.y, _center.z - _Extents.y), Color);

	Create_2D_Line_Buffers(XMFLOAT3(_center.x + _Extents.x, _center.y, _center.z + _Extents.y), XMFLOAT3(_center.x + _Extents.x, _center.y, _center.z - _Extents.y), Color);
	Create_2D_Line_Buffers(XMFLOAT3(_center.x - _Extents.x, _center.y, _center.z - _Extents.y), XMFLOAT3(_center.x - _Extents.x, _center.y, _center.z + _Extents.y), Color);
}

void GraphicDebugManager::Create_2D_Z_Rect_Buffer(XMFLOAT3 _center, XMFLOAT2 _Extents, XMFLOAT3 Color)
{
	Create_2D_Line_Buffers(XMFLOAT3(_center.x, _center.y + _Extents.x, _center.z + _Extents.y), XMFLOAT3(_center.x, _center.y + _Extents.x, _center.z - _Extents.y), Color);
	Create_2D_Line_Buffers(XMFLOAT3(_center.x, _center.y - _Extents.x, _center.z + _Extents.y), XMFLOAT3(_center.x, _center.y - _Extents.x, _center.z - _Extents.y), Color);

	Create_2D_Line_Buffers(XMFLOAT3(_center.x, _center.y + _Extents.x, _center.z - _Extents.y), XMFLOAT3(_center.x, _center.y - _Extents.x, _center.z - _Extents.y), Color);
	Create_2D_Line_Buffers(XMFLOAT3(_center.x, _center.y + _Extents.x, _center.z + _Extents.y), XMFLOAT3(_center.x, _center.y - _Extents.x, _center.z + _Extents.y), Color);
}

void GraphicDebugManager::Create_3D_Box_Buffers(XMFLOAT3 _center, XMFLOAT3 _Extents, XMFLOAT3 Color)
{
	Debug32 temp[8];
	temp[0].Pos = { _Extents.x + _center.x,_Extents.y + _center.y,-_Extents.z + _center.z };
	temp[0].Color = Color;

	temp[1].Pos = { -_Extents.x + _center.x, _Extents.y + _center.y,-_Extents.z + _center.z };
	temp[1].Color = Color;


	temp[2].Pos = { _Extents.x + _center.x,-_Extents.y + _center.y,-_Extents.z + _center.z };
	temp[2].Color = Color;

	temp[3].Pos = { -_Extents.x + _center.x,-_Extents.y + _center.y,-_Extents.z + _center.z };
	temp[3].Color = Color;


	temp[4].Pos = { _Extents.x + _center.x, _Extents.y + _center.y,_Extents.z + _center.z };
	temp[4].Color = Color;

	temp[5].Pos = { -_Extents.x + _center.x, _Extents.y + _center.y,_Extents.z + _center.z };
	temp[5].Color = Color;

	temp[6].Pos = { _Extents.x + _center.x,-_Extents.y + _center.y,_Extents.z + _center.z };
	temp[6].Color = Color;


	temp[7].Pos = { -_Extents.x + _center.x,-_Extents.y + _center.y,_Extents.z + _center.z };
	temp[7].Color = Color;

	for (int i = 0; i < 8; i++)
	{
		vertices.push_back(temp[i]);
	}

	int inSize = (int)indices.size();

	UINT in_temp[24];
	in_temp[0] = 0 + inSize;		in_temp[1] = 1 + inSize;
	in_temp[2] = 2 + inSize;		in_temp[3] = 3 + inSize;
	in_temp[4] = 0 + inSize;		in_temp[5] = 2 + inSize;
	in_temp[6] = 1 + inSize;		in_temp[7] = 3 + inSize;
	in_temp[8] = 4 + inSize;		in_temp[9] = 5 + inSize;
	in_temp[10] = 6 + inSize;		in_temp[11] = 7 + inSize;
	in_temp[12] = 4 + inSize;		in_temp[13] = 6 + inSize;
	in_temp[14] = 5 + inSize;		in_temp[15] = 7 + inSize;
	in_temp[16] = 0 + inSize;		in_temp[17] = 4 + inSize;
	in_temp[18] = 1 + inSize;		in_temp[19] = 5 + inSize;
	in_temp[20] = 2 + inSize;		in_temp[21] = 6 + inSize;
	in_temp[22] = 3 + inSize;		in_temp[23] = 7 + inSize;


	for (int i = 0; i < 24; i++)
	{
		indices.push_back(in_temp[i]);
	}
}

void GraphicDebugManager::Create_3D_Circle_Buffers(XMFLOAT3 _center, float Radius, XMFLOAT3 Color)
{
	Create_2D_Y_Circle_Buffers(_center, Radius, Color);
	Create_2D_X_Circle_Buffers(_center, Radius, Color);
	Create_2D_Z_Circle_Buffers(_center, Radius, Color);
}

void GraphicDebugManager::Create_2D_Line_Buffers(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT3 Color)
{
	//라인을 생성해준다

	int inSize = (int)indices.size();
	int vsize = (int)vertices.size();

	Debug32 temp[2];
	temp[0].Pos		= start;
	temp[0].Color	= Color;

	temp[1].Pos		= end;
	temp[1].Color	= Color;


	for (int i = 0; i < 2; i++)
	{
		vertices.push_back(temp[i]);
	}

	UINT in_temp[2];
	in_temp[0] = 0 + vsize;
	in_temp[1] = 1 + vsize;

	for (int i = 0; i < 2; i++)
	{
		indices.push_back(in_temp[i]);
	}
}

BufferData* GraphicDebugManager::CreateBuffer(std::string BufferName)
{
	//현재 만들어진 인덱스와 버텍스들로 버퍼를 생성한다

	BufferData* Buffer = new BufferData();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Debug32) * (UINT)(vertices.size());
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	m_Device->CreateBuffer(&vbd, &vinitData, &Buffer->VB);


	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * (UINT)indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	m_Device->CreateBuffer(&ibd, &iinitData, &Buffer->IB);

	Buffer->IndexCount = (int)indices.size();
	Buffer->VertexCount = (int)vertices.size();
	Buffer->VertexDataSize = sizeof(Debug32);

	//다쓴 인덱스와 버텍스들은 삭제 시킨다
	vertices.clear();
	indices.clear();

	//버퍼리스트에 넣어준다
	BufferList.insert({BufferName,Buffer});

	return Buffer;
}

BufferData* GraphicDebugManager::GetGridBuffer()
{
	return GridBuffer;
}

BufferData* GraphicDebugManager::GetBoneBuffer()
{
	return BoneBuffer;
}

void GraphicDebugManager::Create_Grid(int CountX, int CountY)
{
	XMFLOAT3 m_Color = { 1,1,1 };
	for (int i = -CountX; i <= CountX; i++)
	{
		Create_2D_Line_Buffers(XMFLOAT3((float)i, 0, (float)(-CountX)), XMFLOAT3((float)i, 0, (float)CountX), m_Color);
	}

	for (int j = -CountY; j <= CountY; j++)
	{
		Create_2D_Line_Buffers(XMFLOAT3((float)(-CountY), 0, (float)j), XMFLOAT3((float)CountY, 0, (float)j), m_Color);
	}
	
	////축도 그린다
	Create_2D_Line_Buffers(XMFLOAT3(0, 0.1f, 0), XMFLOAT3(0, 0.1f, -50), XMFLOAT3(1, 0, 0));
	Create_2D_Line_Buffers(XMFLOAT3(0, 0.1f, 0), XMFLOAT3(0, 50, 0), XMFLOAT3(0, 1, 0));
	Create_2D_Line_Buffers(XMFLOAT3(0, 0.1f, 0), XMFLOAT3(50, 0.1f, 0), XMFLOAT3(0, 0, 1));

	////이대로 버퍼생성
	GridBuffer = CreateBuffer("Grid");
	int num = 0;
}

void GraphicDebugManager::Create_Bone()
{
	Create_3D_Box_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0.02f, 0.02f, 0.02f));
	Create_2D_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, -0.04f), XMFLOAT3(1, 0, 0));
	Create_2D_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0.04f, 0), XMFLOAT3(0, 1, 0));
	Create_2D_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0.04f, 0, 0), XMFLOAT3(0, 0, 1));
	BoneBuffer = CreateBuffer("Bone");
}


