#pragma once
/// <summary>
/// 랜더링 관리 매니저
/// </summary>
#include "Data.h" 

struct ID3D11Device;
struct ID3D11DeviceContext;

class ShaderManager;
class TextureBase;
class HsGraphic;
class GlobalData;
class MeshData;

struct ID3D11RasterizerState;

class RenderingManager
{
public:
	enum class ShaderType
	{
		BASIC,
		SKINNING,
	};








	RenderingManager();
	~RenderingManager();
	void Initialize(ID3D11Device* mDeviece, ID3D11DeviceContext* mDeviceContext,ShaderManager* SM);
	void CreateTexture();

	//카메라 버퍼를 업데이트 
	void CameraUpdate(GlobalData* data);
	//스키닝 버퍼를 업데이트
	void SkinningUpdate(MeshData* data);
	//매쉬 버퍼를 업데이트
	void MeshUpdate(MeshData* data);

	void Rendering(MeshData* data, ShaderType type);
	void CreateRenderState();
	void GetShader();
private:
	ID3D11Device*		 Device;
	ID3D11DeviceContext* DeviceContext;


	TextureBase* Post;
	ShaderManager* mShaderManager;


	ID3D11RasterizerState* mWireframe;
	ID3D11RasterizerState* mSolid;


	ShaderData BasicMesh_SH;
	ShaderData SknningMesh_SH;

	ID3D11SamplerState* BasicSampler;
};