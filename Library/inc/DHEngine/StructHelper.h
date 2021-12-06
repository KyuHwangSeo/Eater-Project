
#include "DH3DEngineDefine.h"
#include "DH3DEngineHeader.h"
#include "Effects.h"

class InputLayoutDesc
{
public:
	// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC PosNormal[2];
	static const D3D11_INPUT_ELEMENT_DESC PosNormalTexBiNormalTangent[5];	
	/// 스카이박스에는 포지션만 ㅎㅎ
	static const D3D11_INPUT_ELEMENT_DESC Pos[1];
};

class InputLayouts
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11InputLayout* Pos;
	static ID3D11InputLayout* PosNormal;
	static ID3D11InputLayout* PosNormalTexBiNormalTangent;
};