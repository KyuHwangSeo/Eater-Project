#include "Animation.h"
#include "ParserData.h"
#include "GameObject.h"
#include "Transform.h"
void Animation::Update()
{
	DirectX::SimpleMath::Vector3	pos = AnimationData->m_AniData[0]->m_Pos;
	DirectX::SimpleMath::Quaternion rot = AnimationData->m_AniData[0]->m_RotQt;
	DirectX::SimpleMath::Vector3	scl = AnimationData->m_AniData[0]->m_Scale;;


	gameobject->GetTransform()->Position = { pos.x,pos.y,pos.z};

}

void Animation::SetAnimation(ParserData::OneAnimation* data)
{
	AnimationData = data;
}

