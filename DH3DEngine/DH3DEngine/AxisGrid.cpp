#include "AxisGrid.h"

void AxisGrid::BuildGridGeometryBuffers()
{
	int Half_Grid = Grid_Num / 2;
	int Pow2_Grid = Grid_Num * Grid_Num;

	// 버텍스버퍼 생성
	std::vector<AxisGridVertex> Grid_Vertices;
	Grid_Vertices.resize(Pow2_Grid);

	for (int i = 0; i < Pow2_Grid; i++)
	{
		Grid_Vertices[i].Pos = XMFLOAT3((float)(i % Grid_Num) - Half_Grid, 0.0f, (float)(i / Grid_Num) - Half_Grid);
		Grid_Vertices[i].Color = XMFLOAT4((const float*)&Colors::FloralWhite);	// (어두운 회색)
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(AxisGridVertex) * Pow2_Grid;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &Grid_Vertices[0];
	md3dDevice->CreateBuffer(&vbd, &vinitData, &m_Grid_VB);


	// 인덱스 버퍼를 생성한다.
	std::vector<UINT> Grid_Indices;
	Grid_Indices.resize(Grid_Num * 4);

	// 열
	for (int i = 0; i < Grid_Num; i++)
	{
		Grid_Indices[i * 2] = i;
		Grid_Indices[i * 2 + 1] = i + Pow2_Grid - Grid_Num;
	}

	// 행
	for (int i = 0; i < Grid_Num; i++)
	{
		Grid_Indices[Grid_Num*2 + (i * 2)] = i * Grid_Num;
		Grid_Indices[Grid_Num*2 + (i * 2) + 1] = (i+1) * Grid_Num - 1;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * Grid_Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &Grid_Indices[0];
	md3dDevice->CreateBuffer(&ibd, &iinitData, &m_Grid_IB);
}

void AxisGrid::BuildAxisGeometryBuffers()
{
	// 정점 버퍼를 생성한다. 
	// 각 축에 맞도록 6개의 정점을 만들었다.
	AxisGridVertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x축 (빨강)
		{ XMFLOAT3(100.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y축 (초록)
		{ XMFLOAT3(0.0f, 100.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z축 (파랑)
		{ XMFLOAT3(0.0f, 0.0f, 100.0f), XMFLOAT4((const float*)&Colors::Blue) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(AxisGridVertex) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	md3dDevice->CreateBuffer(&vbd, &vinitData, &m_Axis_VB);


	// 인덱스 버퍼를 생성한다.
	// 역시 3개의 축을 나타내도록 했다.

	UINT indices[] = {
		// x축
		0, 1,

		// y축
		2, 3,

		// z축
		4, 5,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	md3dDevice->CreateBuffer(&ibd, &iinitData, &m_Axis_IB);
}

void AxisGrid::BuildFX()
{
	std::ifstream fin("../Resources/Shader/CDH/color.cso", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, md3dDevice, &mFX);

	mTech = mFX->GetTechniqueByName("ColorTech");
	mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void AxisGrid::BuildVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	mTech->GetPassByIndex(0)->GetDesc(&passDesc);
	md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout);
}

AxisGrid::AxisGrid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
{
	md3dDevice = pDevice;
	md3dImmediateContext = pDeviceContext;
	m_pRenderstate = pRS;
}

AxisGrid::~AxisGrid()
{

}

void AxisGrid::Render(DirectX::SimpleMath::Matrix* _View_Mat, DirectX::SimpleMath::Matrix* _Pro_Mat)
{
	DrawGrid(_View_Mat, _Pro_Mat);
	DrawAxis(_View_Mat, _Pro_Mat);
}

void AxisGrid::DrawGrid(DirectX::SimpleMath::Matrix* _View_Mat, DirectX::SimpleMath::Matrix* _Pro_Mat)
{
	// 입력 배치 객체 셋팅
	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(AxisGridVertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &m_Grid_VB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(m_Grid_IB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX worldViewProj = _World_Mat * (*_View_Mat) * (*_Pro_Mat);
	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	// 렌더스테이트
	md3dImmediateContext->RSSetState(m_pRenderstate);

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	// 렌더패스는...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		md3dImmediateContext->DrawIndexed(Grid_Num*4, 0, 0);
	}
}

void AxisGrid::DrawAxis( DirectX::SimpleMath::Matrix* _View_Mat, DirectX::SimpleMath::Matrix* _Pro_Mat)
{
	// 입력 배치 객체 셋팅
	md3dImmediateContext->IASetInputLayout(mInputLayout);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(AxisGridVertex);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &m_Axis_VB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(m_Axis_IB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX worldViewProj = _World_Mat *  (*_View_Mat) * (*_Pro_Mat);
	mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	// 렌더스테이트
	md3dImmediateContext->RSSetState(m_pRenderstate);

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	// 렌더패스는...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

		// 6개의 인덱스로 축을 그린다.
		md3dImmediateContext->DrawIndexed(6, 0, 0);
	}
}

void AxisGrid::Initialize(int _Grid_Num)
{
	// 처음 한번 지정한값으로 설정.
	if (m_Axis_VB == nullptr)
	{
		Grid_Num = _Grid_Num;
		BuildGridGeometryBuffers();
		BuildAxisGeometryBuffers();
		BuildFX();
		BuildVertexLayout();
	}
}