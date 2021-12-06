#include "RenderingManager.h"
#include "ShaderManager.h"
#include "BufferData.h"
#include "GraphicDebugManager.h"
#include "EngineData.h"
#include "TextureBase.h"


RenderingManager::RenderingManager()
{
	Device			= nullptr;
	DeviceContext	= nullptr;

	mShaderManager = nullptr;

	mWireframe	= nullptr;
	mSolid		= nullptr;

	BasicSampler = nullptr;
}

RenderingManager::~RenderingManager()
{
	Device = nullptr;
	DeviceContext = nullptr;
}

void RenderingManager::Initialize(ID3D11Device* mDeviece, ID3D11DeviceContext* mDeviceContext,
	ShaderManager* SM, GraphicDebugManager* DM)
{
	Device			= mDeviece;
	DeviceContext	= mDeviceContext;
	
	mShaderManager	= SM;
	mDebugManager	= DM;
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
	///오브젝트 버퍼 업데이트
	if (data->Albedo != nullptr)
	{
		ID3D11ShaderResourceView* SRV = reinterpret_cast<ID3D11ShaderResourceView*>(data->Albedo->TextureBufferPointer);
		DeviceContext->PSSetShaderResources(0, 1, &SRV);
		//DeviceContext->VSSetShaderResources(0, 1, &SRV);
	}


	XMFLOAT4X4 offet =
	{
		1			,0			,0			,0,
		0			,1			,0			,0,
		0			,0			,1			,0,
		0			,0			,0			,1
	};
	XMMATRIX offetMT = XMLoadFloat4x4(&offet);


	///스키닝 오브젝트 버퍼 업데이트
	ID3D11Buffer* buffer = mShaderManager->GetConstantBuffer("SkinningBuffer");
	//버퍼 업데이트
	SkinningBuffer mbuffer;
	mbuffer.world = DirectX::XMMatrixTranspose(data->mWorld); //HLSL에서만
	mbuffer.TexMatrix = DirectX::XMMatrixTranspose(offetMT);


	int OffsetSize = (int)data->BoneOffsetTM.size();
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
	if (data->Albedo != nullptr)
	{
		ID3D11ShaderResourceView* SRV = reinterpret_cast<ID3D11ShaderResourceView*>(data->Albedo->TextureBufferPointer);
		DeviceContext->PSSetShaderResources(0, 1, &SRV);
		DeviceContext->VSSetShaderResources(0, 1, &SRV);
	}

	XMFLOAT4X4 offet =
	{
		1			,0			,0			,0,
		0			,1			,0			,0,
		0			,0			,1			,0,
		0			,0			,0			,1
	};
	XMMATRIX offetMT = XMLoadFloat4x4(&offet);



	ID3D11Buffer* buffer = mShaderManager->GetConstantBuffer("ObjectBuffer");
	//버퍼 업데이트
	ObjectBuffer mbuffer;
	mbuffer.world		= DirectX::XMMatrixTranspose(data->mWorld); //HLSL에서만
	mbuffer.TexMatrix	= DirectX::XMMatrixTranspose(offetMT);
	DeviceContext->UpdateSubresource(buffer, 0, nullptr, &mbuffer, 0, 0);
	DeviceContext->VSSetConstantBuffers(1, 1, &buffer);
	DeviceContext->PSSetConstantBuffers(1, 1, &buffer);
}

void RenderingManager::BoneUpdate(MeshData* Meshdata)
{
	XMVECTOR pos;
	XMVECTOR rot;
	XMVECTOR scl;
	DirectX::XMMatrixDecompose(&scl, &rot, &pos, Meshdata->mWorld);

	XMFLOAT3 _pos;
	XMStoreFloat3(&_pos, pos);


	ID3D11Buffer* buffer = mShaderManager->GetConstantBuffer("ObjectBuffer");
	//버퍼 업데이트
	ObjectBuffer mbuffer;
	mbuffer.world = DirectX::XMMatrixTranspose(Meshdata->mWorld); //HLSL에서만
	mbuffer.TexMatrix = DirectX::XMMatrixTranspose(XMMatrixIdentity());
	DeviceContext->UpdateSubresource(buffer, 0, nullptr, &mbuffer, 0, 0);
	DeviceContext->VSSetConstantBuffers(1, 1, &buffer);
	DeviceContext->PSSetConstantBuffers(1, 1, &buffer);



	BufferData* data = mDebugManager->GetBoneBuffer();

	UINT stride = data->VertexDataSize;
	UINT offset = 0;

	///void* 를 버퍼로 타입변환
	ID3D11Buffer* IBuffer = data->IB;
	ID3D11Buffer* VBuffer = data->VB;

	///데이터 삽입
	DeviceContext->IASetVertexBuffers(0, 1, &VBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT, 0);


	///그리는 방식 설정
	DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	DeviceContext->RSSetState(mWireframe);


	///쉐이더 삽입
	DeviceContext->IASetInputLayout(DebugMesh_SH.Layout);
	DeviceContext->VSSetShader(DebugMesh_SH.VertexShader, NULL, 0);
	DeviceContext->PSSetShader(DebugMesh_SH.PixelShader, NULL, 0);

	///샘플러 삽입
	DeviceContext->PSSetSamplers(0, 1, &BasicSampler);
	DeviceContext->VSSetSamplers(0, 1, &BasicSampler);

	///그리기
	DeviceContext->DrawIndexed(data->IndexCount, 0, 0);
}


void RenderingManager::DebugUpdate()
{
	ID3D11Buffer* buffer = mShaderManager->GetConstantBuffer("ObjectBuffer");
	//버퍼 업데이트
	ObjectBuffer mbuffer;
	mbuffer.world = DirectX::XMMatrixTranspose(XMMatrixIdentity()); //HLSL에서만
	mbuffer.TexMatrix = DirectX::XMMatrixTranspose(XMMatrixIdentity());
	DeviceContext->UpdateSubresource(buffer, 0, nullptr, &mbuffer, 0, 0);
	DeviceContext->VSSetConstantBuffers(1, 1, &buffer);
	DeviceContext->PSSetConstantBuffers(1, 1, &buffer);



	BufferData* data = mDebugManager->GetGridBuffer();

	UINT stride = data->VertexDataSize;
	UINT offset = 0;

	///void* 를 버퍼로 타입변환
	ID3D11Buffer* IBuffer = data->IB;
	ID3D11Buffer* VBuffer = data->VB;

	///데이터 삽입
	DeviceContext->IASetVertexBuffers(0, 1, &VBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT, 0);


	///그리는 방식 설정
	DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	DeviceContext->RSSetState(mWireframe);


	///쉐이더 삽입
	DeviceContext->IASetInputLayout(DebugMesh_SH.Layout);
	DeviceContext->VSSetShader(DebugMesh_SH.VertexShader, NULL, 0);
	DeviceContext->PSSetShader(DebugMesh_SH.PixelShader, NULL, 0);

	///샘플러 삽입
	DeviceContext->PSSetSamplers(0, 1, &BasicSampler);
	DeviceContext->VSSetSamplers(0, 1, &BasicSampler);

	///그리기
	DeviceContext->DrawIndexed(data->IndexCount, 0, 0);
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
	DeviceContext->RSSetState(mSolid);

	
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
	DeviceContext->VSSetSamplers(0, 1, &BasicSampler);
	DeviceContext->PSSetSamplers(0, 1, &BasicSampler);

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
	DebugMesh_SH	= mShaderManager->GetShader("Debug");


	///샘플러 가져오기
	BasicSampler = mShaderManager->GetSampler("Basic");

}

