#pragma once
#include <d3d11shader.h>
#include <string>
#include "d3dcommon.h"
#include "Data.h"
#include <map>


struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderReflection;
class ShaderManager 
{
public:
	ShaderManager();
	~ShaderManager();

	//초기화
	void Initialize(ID3D11Device* mDevice,ID3D11DeviceContext* DeviceContext);
	//쉐이더 로드
	bool LoadShader(std::string vsFileName, std::string psFileName, std::string ShaderName);

	//쉐이더 가져오기
	ShaderData GetShader(std::string name);
	//셈플러 가져오기
	ID3D11SamplerState* GetSampler(std::string name);
	//상수버퍼 가져오기
	ID3D11Buffer* GetConstantBuffer(std::string name);
private:
	//로드 버택스 쉐이더
	bool LoadVertexShader(std::string vsFileName, std::string ShaderName);
	//로드 픽셀 쉐이더
	bool LoadPixelShader(std::string psFileName, std::string ShaderName);
	//샘플러 생성
	bool CreateSampler();
	//인풋 레이아웃을 만든다
	bool CreateInputLayout(std::string shaderName, ID3D10Blob* vertexBuffer);
	//상수버퍼를 만든다
	bool CreateConstantBuffer();
	//Format 찾기
	DXGI_FORMAT FindFormat(int number, BYTE size, D3D11_SIGNATURE_PARAMETER_DESC desc);
private:
	ID3D11Device*			Device;
	ID3D11DeviceContext*	DeviceContext;

	ID3D11ShaderReflection* ReFlector;
	std::string ShaderPath;


	//쉐이더 정보
	std::map<std::string, ShaderData> ShaderList;
	//샘플러 정보
	std::map<std::string, ID3D11SamplerState*>  SamplerList;
	//컨텍스트 버퍼
	std::map<std::string, ID3D11Buffer*> CbufferList;
};