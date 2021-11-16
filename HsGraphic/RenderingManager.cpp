#include "RenderingManager.h"
#include "ShaderManager.h"
#include "BufferData.h"
#include "EngineData.h"
#include "TextureBase.h"


RenderingManager::RenderingManager()
{
	Device			= nullptr;
	DeviceContext	= nullptr;
}

RenderingManager::~RenderingManager()
{
	Device = nullptr;
	DeviceContext = nullptr;
}

void RenderingManager::Initialize(ID3D11Device* mDeviece, ID3D11DeviceContext* mDeviceContext, ShaderManager* SM)
{
	Device			= mDeviece;
	DeviceContext	= mDeviceContext;
	
	mShaderManager	= SM;
	CreateTexture();
	CreateRenderState();
	GetShader();
}

void RenderingManager::CreateTexture()
{
	//포스트 프로세싱 전용 텍스쳐 생성
	//Post = new TextureBase();
	//Post->Create();
}

void RenderingManager::CameraUpdate(GlobalData* data)
{
	//카메라 버퍼 업데이트
	ID3D11Buffer* buffer = mShaderManager->GetConstantBuffer("CameraBuffer");
	CameraBuffer temp;
	temp.view = DirectX::XMMatrixTranspose(*data->mViewMX);
	temp.proj = DirectX::XMMatrixTranspose(*data->mProj);

	DeviceContext->UpdateSubresource(buffer, 0, nullptr, &temp, 0, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &buffer);
	DeviceContext->PSSetConstantBuffers(0, 1, &buffer);
}

void RenderingManager::SkinningUpdate(MeshData* data)
{
	///스키닝 오브젝트 버퍼 업데이트
	ID3D11Buffer* buffer = mShaderManager->GetConstantBuffer("SkinningBuffer");
	//버퍼 업데이트
	SkinningBuffer mbuffer;
	mbuffer.world = DirectX::XMMatrixTranspose(data->mWorld); //HLSL에서만

	int OffsetSize = (data->BoneOffsetTM).size();
	for (int i = 0; i < OffsetSize; i++)
	{
		mbuffer.BoneOffset[i] = (data->BoneOffsetTM)[i];
	}
	

	DeviceContext->UpdateSubresource(buffer, 0, nullptr, &mbuffer, 0, 0);
	DeviceContext->VSSetConstantBuffers(2, 1, &buffer);
	DeviceContext->PSSetConstantBuffers(2, 1, &buffer);
}

void RenderingManager::MeshUpdate(MeshData* data)
{
	///오브젝트 버퍼 업데이트
	ID3D11Buffer* buffer = mShaderManager->GetConstantBuffer("ObjectBuffer");
	//버퍼 업데이트
	ObjectBuffer mbuffer;
	mbuffer.world = DirectX::XMMatrixTranspose(data->mWorld); //HLSL에서만

	DeviceContext->UpdateSubresource(buffer, 0, nullptr, &mbuffer, 0, 0);
	DeviceContext->VSSetConstantBuffers(1, 1, &buffer);
	DeviceContext->PSSetConstantBuffers(1, 1, &buffer);
}

void RenderingManager::Rendering(MeshData* data, ShaderType type)
{
	///버텍스 버퍼를 만든 자료형 사이즈
	UINT stride = data->VB->VertexDataSize;
	UINT offset = 0;

	///void* 를 버퍼로 타입변환
	ID3D11Buffer* IBuffer = reinterpret_cast<ID3D11Buffer*>(data->IB->IndexBufferPointer);
	ID3D11Buffer* VBuffer = reinterpret_cast<ID3D11Buffer*>(data->VB->VertexbufferPointer);

	///데이터 삽입
	DeviceContext->IASetVertexBuffers(0, 1, &VBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT, 0);


	///그리는 방식 설정
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->RSSetState(mWireframe);

	
	ShaderData Shaderdata;
	switch (type)
	{
	case RenderingManager::ShaderType::BASIC:
		Shaderdata = BasicMesh_SH;
		break;
	case RenderingManager::ShaderType::SKINNING:
		Shaderdata = SknningMesh_SH;
		break;
	}

	///쉐이더 삽입
	DeviceContext->IASetInputLayout(Shaderdata.Layout);
	DeviceContext->VSSetShader(Shaderdata.VertexShader, NULL, 0);
	DeviceContext->PSSetShader(Shaderdata.PixelShader, NULL, 0);

	///샘플러 삽입
	DeviceContext->PSSetSamplers(0, 1, &BasicSampler);
	DeviceContext->VSSetSamplers(0, 1, &BasicSampler);

	///그리기
	DeviceContext->DrawIndexed(data->IB->Count, 0, 0);
}

void RenderingManager::CreateRenderState()
{
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	Device->CreateRasterizerState(&solidDesc, &mSolid);


	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	Device->CreateRasterizerState(&wireframeDesc, &mWireframe);
}

void RenderingManager::GetShader()
{
	///쉐이더 미리 가져오기
	BasicMesh_SH	= mShaderManager->GetShader("texture");
	SknningMesh_SH	= mShaderManager->GetShader("Skinning");


	///샘플러 가져오기
	BasicSampler = mShaderManager->GetSampler("Basic");

}

