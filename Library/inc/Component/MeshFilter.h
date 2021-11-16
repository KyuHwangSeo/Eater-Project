#pragma once

#include "EaterEngineDLL.h"
#include <vector>
#include <string>
#include "Component.h"
/// <summary>
/// 매쉬정보를 파서에서 받아오고 저장해두는 컨퍼넌트
/// </summary>

namespace DirectX
{
	namespace SimpleMath
	{
		struct Matrix;
	}
}

class ModelData;
class LoadMeshData;
class Transform;
class ObjectManager;
class MaterialManager;

class  MeshFilter : public Component
{
public:
	EATER_ENGINEDLL MeshFilter();
	virtual ~MeshFilter();

	virtual void Awake() override;

public:
	//메쉬의 이름을 넣으면 데이터 로드
	EATER_ENGINEDLL void SetMeshName(std::string mMeshName);

	static void SetManager(ObjectManager* obj, MaterialManager* mat);
private:
	//Transform을 연결한다
	void LinkHierarchy(Transform* my,Transform*parent);

	//LoadMeshData를 게임에 사용할 오브젝트로 생성하면서 값을 넣어준다
	void CreateChild_Mesh(LoadMeshData* data,Transform* parent, ModelData* modeldata);

	//LoadMeshData를 게임에 사용할 오브젝트로 생성하면서 값을 넣어준다
	void CreateChild_Bone(LoadMeshData* data, Transform* parent, std::vector<Transform*>* mBoneList, std::vector<DirectX::SimpleMath::Matrix>* BoneOffsetList);

	//게임오브젝트의 LoadData를 합쳐준다
	void PushModelData(LoadMeshData* mModel);

	static ObjectManager* OBJ_Manager;
	static MaterialManager* MAT_Manager;
private:
	bool isLoad;			//매쉬 로드여부
	std::string MeshName;	//로드한 매쉬 이름

	std::vector<Transform*> BoneList;
	std::vector<DirectX::SimpleMath::Matrix> BoneOffsetList;
};

