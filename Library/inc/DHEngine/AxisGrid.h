#pragma once

#include "DH3DEngineHeader.h"

class AxisGrid
{
private:
	struct AxisGridVertex
	{
		DirectX::SimpleMath::Vector3 Pos;
		DirectX::SimpleMath::Vector4 Color;
	};

	// Gird를 만들때 10 X 10 인지 50 X 50인지.. 크기를 정할 변수.
	int Grid_Num = 0;
	// Axis나 Grid나 World는 기본값으로 동일.
	DirectX::SimpleMath::Matrix _World_Mat;

	ID3D11Device* md3dDevice = nullptr;						// D3D11 디바이스
	ID3D11DeviceContext* md3dImmediateContext = nullptr;	// 디바이스 컨텍스트

	ID3D11Buffer* m_Axis_VB = nullptr;
	ID3D11Buffer* m_Axis_IB = nullptr;

	ID3D11Buffer* m_Grid_VB = nullptr;
	ID3D11Buffer* m_Grid_IB = nullptr;

	ID3DX11Effect* mFX = nullptr;
	ID3DX11EffectTechnique* mTech = nullptr;
	ID3DX11EffectMatrixVariable* mfxWorldViewProj = nullptr;

	ID3D11InputLayout* mInputLayout = nullptr;

	ID3D11RasterizerState* m_pRenderstate = nullptr;

private:
	void BuildGridGeometryBuffers();
	void BuildAxisGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();

public:
	AxisGrid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~AxisGrid();

	void Render(DirectX::SimpleMath::Matrix* _View_Mat, DirectX::SimpleMath::Matrix* _Pro_Mat);
	void DrawGrid(DirectX::SimpleMath::Matrix* _View_Mat, DirectX::SimpleMath::Matrix* _Pro_Mat);
	void DrawAxis(DirectX::SimpleMath::Matrix* _View_Mat, DirectX::SimpleMath::Matrix* _Pro_Mat);
	void Initialize(int _Grid_Num);


};

