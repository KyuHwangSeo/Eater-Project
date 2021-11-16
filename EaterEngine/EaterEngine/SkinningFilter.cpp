#include "SkinningFilter.h"
#include "GameObject.h"
#include "LoadManager.h"
#include "Transform.h"
#include "EngineData.h"
#include "Animation.h"
#include "ObjectManager.h"

SkinningFilter::SkinningFilter()
{

}

SkinningFilter::~SkinningFilter()
{

}

void SkinningFilter::Update()
{
	//데이터를 넣기전 이전 값들은 삭제
	MeshData* data = gameobject->OneMeshData;
	data->BoneOffsetTM.clear();

	//사이즈 잡기
	int BoneSize =  (int)BoneList->size();
	//data->BoneOffsetTM.resize(BoneSize);
	(*BoneList)[1]->SetRotate(0, 0, 0.01f);
	//(*BoneList)[27]->SetRotate(0, 0, 0.01f);

	//데이터 넣어주기
	for (int i = 0; i < BoneList->size(); i++)
	{
		//본의 월드
		DirectX::XMMATRIX BoneWorld = *(*BoneList)[i]->GetWorld();

		//본의 오프셋
		DirectX::XMFLOAT4X4 temp = (*BoneOffsetTM)[i];
		DirectX::XMMATRIX Offset = DirectX::XMLoadFloat4x4(&temp);

		//그래픽 랜더링쪽으로 넘겨줄수있도록 값을 넣어줌
		(data->BoneOffsetTM).push_back(Offset * BoneWorld);
	}
	int num = 0;
}

void SkinningFilter::PushBoneList(std::vector<Transform*>* mBoneList)
{
	// BoneList 와 BoneOffsetTM 은 1:1 매칭이 되어야한다
	BoneList = mBoneList;
}

void SkinningFilter::PushBone_OffsetList(std::vector<DirectX::SimpleMath::Matrix>* mBoneOffsetTM)
{
	BoneOffsetTM = mBoneOffsetTM;
}
