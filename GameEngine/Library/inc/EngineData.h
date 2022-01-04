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
	///���� �⺻������ �־�� ������Ʈ ����
	///���߿� �ʿ��Ѱ� �ְ� �ʿ���°� ����

	DEFALT,			//���� �����ʾ����� �⺻��
	GAMEOBJECT,		//�ٸ� ������Ʈ���� ������� �뵵
	BASE,			//��ӱ����� �Ǿ��ִ� ������Ʈ
	SKINNING,		//��Ű���� �߰��� ������Ʈ
	BONE,			//���� �ִ� ������Ʈ
	TERRAIN,		//�ͷ��� ������Ʈ
	CAMERA,			//ī�޶� ��ɸ� �ϴ� ������Ʈ
	LIGHT,			//����Ʈ ��ü
	SKYBOX,			//��ī�� �ڽ�
	TEXTURE,		//�Ѱ��� �ؽ��ķε� ������Ʈ(���̽��ΰ��� ������ �簢�� �ؽ���)
	DEBUGOBJECT,	//����� ������Ʈ
	PARTICLE,		//��ƼŬ ������Ʈ
	EFFECT			//����Ʈ ������Ʈ
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
/// ���ӿ������� �׷��ȿ������� ������ �۷ι� ������
/// </summary>
class GlobalData
{
public:
	//ī�޶� ������
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
/// ���ӿ������� �׷��ȿ������� ������ �Ѱ��� �޽� ������
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
	OBJECT_TYPE ObjType = OBJECT_TYPE::DEFALT;		//������Ʈ Ÿ��

	IndexBuffer* IB = nullptr;						//�ε��� ����
	VertexBuffer* VB = nullptr;						//���ؽ� ����

	std::vector<Matrix> BoneOffsetTM;				//�� ������ TM

	Matrix mWorld = XMMatrixIdentity();			//�Ž��� ���� ���
	Matrix mLocal = XMMatrixIdentity();			//�Ž��� �������
	Matrix mTexTM = XMMatrixIdentity();

	// �߰� ������
	MaterialData* Material_Data;		// Material Data
	TerrainData* Terrain_Data;			// Terrain Data
	ParticleData* Particle_Data;		// Particle Data
};

/// <summary>
/// ��Ȳ�� �ļ����� ���ؽ��� �ε����� ���۷� �������ְ�
/// �ִϸ��̼ǰ�, ��������ʴ°����� �׳� �����ͼ� ���ĳ��� Ŭ����
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

	MESH_TYPE MeshType;				// �Ž� Ÿ��

	bool Top_Object = false;		//���� �ֻ��� ������Ʈ���� ����

	int BoneIndex = -1;				//���ϰ�� �ڽ��� �ε���

	std::string ParentName = "";	//�θ��� �̸�
	std::string	Name = "";			//�ڱ��ڽ��� �̸�

	Matrix WorldTM;					//���� ��Ʈ����
	Matrix LocalTM;					//���� ��Ʈ����

	IndexBuffer* IB = nullptr;			//�ε��� ����
	VertexBuffer* VB = nullptr;			//���ؽ� ����

	TextureBuffer* Albedo = nullptr;	// DiffuseMap Texture
	TextureBuffer* Normal = nullptr;	// NormalMap Texture
	TextureBuffer* Roughness = nullptr;	// RoughnessMap Texture
	TextureBuffer* Metallic = nullptr;	// MetallicMap Texture

	std::vector<UINT> Index_List;		//	
	std::vector<Vector3> Vertex_List;	//

	ParserData::CMaterial* Material = nullptr;	//���׸��� ����
	ParserData::OneAnimation* Animation = nullptr;	//�ִϸ��̼� ����

	std::vector<Matrix>* BoneTMList = nullptr;	//�� ��Ʈ����
	std::vector<Mesh*>* BoneList = nullptr;		//�� �Ž�

	Vector3*	m_OriginVertexList		= nullptr;
	UINT*		m_OriginIndexList		= nullptr;
	int			m_OriginVertexListCount = 0;
	int			m_OriginIndexListCount	= 0;
	
	std::string Mask_Name;					// Terrain ���� Mask Name

	LoadMeshData* Parent = nullptr;			//�θ� �Ž�
	std::vector<LoadMeshData*> Child;		//�ڽ� �Ž� ����Ʈ

	Matrix* BoneOffset = nullptr;			//�� ��Ʈ����
};

/// <summary>
/// �Ѱ��� ���� �����ϴ� ����
/// �Ž��� ���� �ֻ��� ������Ʈ�鸸 �����ϴ� ���嵥����
/// </summary>
class ModelData
{
public:
	~ModelData()
	{
		//�ֻ����� ������Ʈ�� ��ͷ� ���鼭 �����ͷ� �����Ȱ͵� ��λ���
		//
	}
	std::vector<LoadMeshData*> TopMeshList;
	std::vector<LoadMeshData*> TopBoneList;

	std::vector<Matrix>* BoneOffsetList = nullptr;	//�� ��Ʈ����
	std::vector<Mesh*>* BoneList = nullptr;	//�� �Ž�
};

class ModelAnimationData
{
public:
	~ModelAnimationData()
	{

	}
	std::map<std::string, std::vector<OneAnimation*>* > AnimList;
};

//���۳�Ʈ���� �Լ������͸� ������ ����ü
class ComponentFunctionData
{
public:
	//�Լ� Ȱ��ȭ ����
	bool* Enabled = nullptr;

	//�Լ� ������ 
	std::function<void()> FunctionPointer;

	//���۳�Ʈ ������
	Component* ComponentPointer;

	//���۳�Ʈ ����
	int OrderCount = 0;
};