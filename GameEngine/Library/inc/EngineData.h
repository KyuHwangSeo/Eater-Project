#pragma once
#include <map>
#include <functional>
#include "ResourcesData.h"
#include "ParserData.h"
#include "LightHelper.h"
#include "TypeOptionHeader.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace ParserData;

class Component;

typedef enum OBJECT_TYPE
{
	///현재 기본형으로 넣어둔 오브젝트 종류
	///나중에 필요한건 넣고 필요없는건 빼자

	DEFALT,			//값을 넣지않았을때 기본형
	GAMEOBJECT,		//다른 오브젝트들을 묶어놓는 용도
	BASE,			//상속구조로 되어있는 오브젝트
	SKINNING,		//스키닝이 추가된 오브젝트
	BONE,			//본만 있는 오브젝트
	TERRAIN,		//터레인 오브젝트
	CAMERA,			//카메라 기능만 하는 오브젝트
	LIGHT,			//라이트 객체
	SKYBOX,			//스카이 박스
	TEXTURE,		//한개의 텍스쳐로된 오브젝트(페이스두개가 합쳐진 사각형 텍스쳐)
	DEBUGOBJECT,	//디버깅 오브젝트
	PARTICLE,		//파티클 오브젝트
	EFFECT			//이펙트 오브젝트
}OBJECT_TYPE;

class MaterialData
{
public:
	UINT Material_Index = 0;				// Material Index

	MaterialOption* Material_Option = nullptr;	// Material Data

	TextureBuffer* Albedo = nullptr;		// DiffuseMap Texture
	TextureBuffer* Normal = nullptr;		// NormalMap Texture
	TextureBuffer* Roughness = nullptr;		// RoughnessMap Texture
	TextureBuffer* Metallic = nullptr;		// MetallicMap Texture

	Vector4 Color_Base;			// Base Color
	Vector4 Color_Add;			// Add Color
};

class TerrainData
{
public:
	std::vector<MaterialData*> Material_List;		// Material List
};

class OneParticle
{
public:
	bool Playing;
	Vector4 Color;

	Matrix* Tex;
	Matrix* World;
};

class ParticleData
{
public:
	PARTICLE_RENDER_OPTION RenderType;

	int Particle_Count;
	Vector3 Area_Radius;

	std::vector<OneParticle*> m_Particles;
};


/// <summary>
/// 게임엔진에서 그래픽엔진으로 던저줄 글로벌 데이터
/// </summary>
class GlobalData
{
public:
	//카메라 정보들
	Matrix mCamInvView;	// Camera Inverse XY View Matrix
	Matrix mCamView;	// Camera View Matrix
	Matrix mCamProj;	// Camera Proj Matrix
	Vector3 mCamPos;	// Camera Pos

	Matrix mCamVP;		// Camera Proj * Proj Matrix
	Matrix mCamPT;		// Camera Proj * TexSpace Matrix
	Matrix mCamVPT;		// Camera View * Proj * TexSpace Matrix

	Matrix mLightView;	// Light View Matrix
	Matrix mLightProj;	// Light Proj Matrix

	Matrix mLightVP;	// Light View * Proj Matrix
	Matrix mLightVPT;	// Light View * Proj * TexSpace Matrix

	LightData* mLightData;
	std::vector<MaterialOption*> mMatData;
};

/// <summary>
/// 게임엔진에서 그래픽엔진으로 던저줄 한개의 메쉬 데이터
/// </summary>
class MeshData
{
public:
	~MeshData()
	{
		IB = nullptr;
		VB = nullptr;
	}

public:
	OBJECT_TYPE ObjType = OBJECT_TYPE::DEFALT;		//오브젝트 타입

	IndexBuffer* IB = nullptr;						//인덱스 버퍼
	VertexBuffer* VB = nullptr;						//버텍스 버퍼

	std::vector<Matrix> BoneOffsetTM;				//본 오프셋 TM

	Matrix mWorld = XMMatrixIdentity();			//매쉬의 월드 행렬
	Matrix mLocal = XMMatrixIdentity();			//매쉬의 로컬행렬
	Matrix mTexTM = XMMatrixIdentity();

	// 추가 데이터
	MaterialData* Material_Data;		// Material Data
	TerrainData* Terrain_Data;			// Terrain Data
	ParticleData* Particle_Data;		// Particle Data
};

/// <summary>
/// 규황이 파서에서 버텍스와 인덱스를 버퍼로 변경해주고
/// 애니메이션과, 변경되지않는값들은 그냥 가져와서 합쳐놓는 클래스
/// </summary>
class LoadMeshData
{
public:
	~LoadMeshData()
	{
		IB = nullptr;
		VB = nullptr;

		Material = nullptr;
		Animation = nullptr;

		BoneTMList = nullptr;
		BoneList = nullptr;

		Parent = nullptr;
	};

	MESH_TYPE MeshType;				// 매쉬 타입

	bool Top_Object = false;		//가장 최상위 오브젝트인지 여부

	int BoneIndex = -1;				//본일경우 자신의 인덱스

	std::string ParentName = "";	//부모의 이름
	std::string	Name = "";			//자기자신의 이름

	Matrix WorldTM;					//월드 매트릭스
	Matrix LocalTM;					//로컬 매트릭스

	IndexBuffer* IB = nullptr;			//인덱스 버퍼
	VertexBuffer* VB = nullptr;			//버텍스 버퍼

	TextureBuffer* Albedo = nullptr;	// DiffuseMap Texture
	TextureBuffer* Normal = nullptr;	// NormalMap Texture
	TextureBuffer* Roughness = nullptr;	// RoughnessMap Texture
	TextureBuffer* Metallic = nullptr;	// MetallicMap Texture

	std::vector<UINT> Index_List;		//	
	std::vector<Vector3> Vertex_List;	//

	ParserData::CMaterial* Material = nullptr;	//메테리얼 정보
	ParserData::OneAnimation* Animation = nullptr;	//애니메이션 정보

	std::vector<Matrix>* BoneTMList = nullptr;	//본 매트릭스
	std::vector<Mesh*>* BoneList = nullptr;		//본 매쉬

	Vector3*	m_OriginVertexList		= nullptr;
	UINT*		m_OriginIndexList		= nullptr;
	int			m_OriginVertexListCount = 0;
	int			m_OriginIndexListCount	= 0;
	
	std::string Mask_Name;					// Terrain 전용 Mask Name

	LoadMeshData* Parent = nullptr;			//부모 매쉬
	std::vector<LoadMeshData*> Child;		//자식 매쉬 리스트

	Matrix* BoneOffset = nullptr;			//본 매트릭스
};

/// <summary>
/// 한개의 모델을 저장하는 단위
/// 매쉬의 가장 최상위 오브젝트들만 보관하는 저장데이터
/// </summary>
class ModelData
{
public:
	~ModelData()
	{
		//최상위의 오브젝트를 재귀로 돌면서 포인터로 생성된것들 모두삭제
		//
	}
	std::vector<LoadMeshData*> TopMeshList;
	std::vector<LoadMeshData*> TopBoneList;

	std::vector<Matrix>* BoneOffsetList = nullptr;	//본 매트릭스
	std::vector<Mesh*>* BoneList = nullptr;	//본 매쉬
};

class ModelAnimationData
{
public:
	~ModelAnimationData()
	{

	}
	std::map<std::string, std::vector<OneAnimation*>* > AnimList;
};

//컨퍼넌트들의 함수포인터를 저장할 구조체
class ComponentFunctionData
{
public:
	//함수 활성화 여부
	bool* Enabled = nullptr;

	//함수 포인터 
	std::function<void()> FunctionPointer;

	//컨퍼넌트 포인터
	Component* ComponentPointer;

	//컨퍼넌트 순서
	int OrderCount = 0;
};