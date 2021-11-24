#include "Animator.h"
#include "ParserData.h"
#include "GameObject.h"
#include "Transform.h"
#include "TimeManager.h"
#include "KeyinputManager.h"

Animator::Animator()
{
	NowAnimationData = nullptr;
	transfrom = nullptr;

	XM_ROT = XMMatrixIdentity();
	XM_POS = XMMatrixIdentity();
	XM_SCL = XMMatrixIdentity();

	mTime		= 0;
	PlayTime	= 1;
	AnimeIndex	= 0;

	mLoop = true;
	mStop = false;
}

Animator::~Animator()
{

}

void Animator::Start()
{
	transfrom = gameobject->transform;
	ChoiceAnime(0);
	SetPlayTime(1,true);
}

void Animator::StartUpdate()
{
	if (NowAnimationData != nullptr)
	{
		AnimeFrameIndex();
	
		DirectX::SimpleMath::Vector3	pos = NowAnimationData->m_AniData[AnimeIndex]->m_Pos;
		DirectX::SimpleMath::Quaternion rot = NowAnimationData->m_AniData[AnimeIndex]->m_RotQt;
		DirectX::SimpleMath::Vector3	scl = NowAnimationData->m_AniData[AnimeIndex]->m_Scale;
		
		XM_ROT = DirectX::XMMatrixRotationQuaternion(rot);
		XM_POS = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		XM_SCL = DirectX::XMMatrixScaling(scl.x, scl.y, scl.z);
		
		transfrom->Load_Local = XM_SCL * XM_ROT * XM_POS;
	}
	
	if (mKeyInputManger->GetKeyDown(VK_NUMPAD1))
	{
		Stop();
	}
	
	if (mKeyInputManger->GetKeyDown(VK_NUMPAD2))
	{
		ReStart();
	}
}

void Animator::SetAnimation(ParserData::OneAnimation* data)
{
	AnimeData.push_back(data);
}

void Animator::SetPlayTime(float time, bool Loop)
{
	PlayTime = time;
	mLoop = Loop;
}

void Animator::Stop()
{
	mStop = true;
}

void Animator::ReStart()
{
	mStop = false;
}

void Animator::CreateFrame(int CreateCount)
{
	//보간을 해야하나?






}

void Animator::ChoiceAnime(int index)
{
	NowAnimationData = AnimeData[index];
}

float Animator::GetOnePlayTime(float mPlayTime, int EndFrameCount)
{
	return (mPlayTime / EndFrameCount);
}

void Animator::AnimeFrameIndex()
{
	if (mStop == false) 
	{
		mTime += mTimeManager->DeltaTime();
		if (mTime >= GetOnePlayTime(PlayTime, NowAnimationData->m_EndFrame))
		{
			mTime = 0;
			AnimeIndex++;
		}
	}


	if (AnimeIndex >= NowAnimationData->m_EndFrame)
	{
		if (mLoop == false)
		{
			AnimeIndex = NowAnimationData->m_EndFrame;
		}
		else
		{
			AnimeIndex = 0;
		}
	}
}

