#include "Component.h"
#include "Transform.h"
#include "EngineData.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include "TimeManager.h"
#include "LoadManager.h"
#include "GameObject.h"
#include <random>
#include "DebugManager.h"

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))


Particle::Particle()
{
}

Particle::~Particle()
{

}

void Particle::Awake()
{
	m_Transform = gameobject->transform;

	m_ParticleData->Tex = &m_Tex;
	m_ParticleData->World = m_Transform->GetWorld();
}

void Particle::Start()
{
	m_TexTotalFrame = m_SystemDesc->Total_Frame;

	m_AniTotalFrame = 60;

	// Option에 따른 Scale Turning Frame 지정..
	if (m_SystemDesc->ScaleType == UPDOWN)
		m_ScaleTurningFrame = m_AniTotalFrame / 2;
	else
		m_ScaleTurningFrame = m_AniTotalFrame;

	// Option에 따른 Color Turning Frame 지정..
	if (m_SystemDesc->ColorType == UPDOWN)
		m_ColorTurningFrame = m_AniTotalFrame / 2;
	else
		m_ColorTurningFrame = m_AniTotalFrame;

	m_WidthCount = m_SystemDesc->Tile_Width;
	m_HeightCount = m_SystemDesc->Tile_Height;

	m_Tex = DirectX::SimpleMath::Matrix::CreateScale(1.0f / m_SystemDesc->Tile_Width, 1.0f / m_SystemDesc->Tile_Height, 1.0f);
}

void Particle::Update()
{
	if (m_Playing == false) return;

	float dTime = mTimeManager->DeltaTime();

	// 해당 파티클 업데이트..
	if (m_LifeTime > 0.0f)
	{
		m_TexNowTime += dTime;
		m_AniNowTime += dTime;
		m_LifeTime -= dTime;

		m_NextFrame = (int)(m_TexNowTime / m_TexOneFrame);

		// Texture Animation..
		if (m_TexFrame != m_NextFrame)
		{
			m_TexFrame = m_NextFrame;

			if (m_TexFrame > m_TexTotalFrame)
			{
				m_TexNowFrame = m_TexTotalFrame;
			}
			else
			{
				m_TexNowFrame = m_TexFrame;
			}

			// UV 변경 및 설정..
			if (m_AniType & TEXTURE_ANI)
			{
				// Texture 출력 영역 변경..
				m_Tex._41 = (m_TexNowFrame % m_WidthCount) * m_Tex._11;
				m_Tex._42 = (m_TexNowFrame / m_HeightCount) * m_Tex._22;
			}
		}

		m_NextFrame = (int)(m_AniNowTime / m_AniOneFrame);
		
		// Pos & Rot & Scale & Color Animation..
		if (m_AniFrame != m_NextFrame)
		{
			m_AniFrame = m_NextFrame;
			m_AniPrevFrame = m_AniFrame - 1;
			m_AniNextFrame = m_AniFrame;

			if (m_AniPrevFrame < 0)
			{
				m_AniPrevFrame = 0;
			}
			if (m_AniNextFrame > m_AniTotalFrame)
			{
				m_AniNextFrame = m_AniTotalFrame;
				m_AniPrevFrame = m_AniTotalFrame;
			}

			// Position Data 변경 및 설정..
			if (m_AniType & POSITION_ANI)
			{
				// Position 범위 변경..
				m_PrevPos = m_StartPos + m_OnePos * m_AniPrevFrame;
				m_NextPos = m_StartPos + m_OnePos * m_AniNextFrame;
			}

			// Rotation Data 변경 및 설정..
			if (m_AniType & ROTATION_ANI)
			{
				// Rotation 범위 변경..
				m_PrevRot = m_StartRot + m_OneRot * m_AniPrevFrame;
				m_NextRot = m_StartRot + m_OneRot * m_AniNextFrame;
			}

			// Scale Data 변경 및 설정..
			if (m_AniType & SCALE_ANI)
			{
				if (m_AniNextFrame <= m_ScaleTurningFrame)
				{
					// Scale 범위 변경..
					m_PrevScale = m_StartScale + m_OneScale * m_AniPrevFrame;
					m_NextScale = m_StartScale + m_OneScale * m_AniNextFrame;
				}
				else
				{
					// Scale 범위 변경..
					m_PrevScale = m_StartScale + m_OneScale * (m_AniTotalFrame - m_AniPrevFrame);
					m_NextScale = m_StartScale + m_OneScale * (m_AniTotalFrame - m_AniNextFrame);
				}
			}

			// Color Data 변경 및 설정..
			if (m_AniType & COLOR_ANI)
			{
				if (m_AniNextFrame <= m_ColorTurningFrame)
				{
					// Color 범위 변경..
					m_PrevColor = m_StartColor + m_OneColor * m_AniPrevFrame;
					m_NextColor = m_StartColor + m_OneColor * m_AniNextFrame;
				}
				else
				{
					// Color 범위 변경..
					m_PrevColor = m_StartColor + m_OneColor * (m_AniTotalFrame - m_AniPrevFrame);
					m_NextColor = m_StartColor + m_OneColor * (m_AniTotalFrame - m_AniNextFrame);
				}
			}
		}

		m_OneTickFrame = (m_AniNowTime / m_AniFrame) / m_AniOneFrame;

		// 파티클 데이터 보간..
		if (m_AniType & COLOR_ANI)
		{
			m_NowColor = Vector4::Lerp(m_PrevColor, m_NextColor, m_OneTickFrame);
			m_ParticleData->Color = m_NowColor;
		}
		if (m_AniType & POSITION_ANI)
		{
			m_NowPos = Vector3::Lerp(m_PrevPos, m_NextPos, m_OneTickFrame);
			gameobject->transform->Position = m_NowPos;
		}
		if (m_AniType & ROTATION_ANI)
		{
			m_NowRot = LERP(m_PrevRot, m_NextRot, m_OneTickFrame);
			gameobject->transform->Rotation.z = m_NowRot;
		}
		if (m_AniType & SCALE_ANI)
		{
			m_NowScale = LERP(m_PrevScale, m_NextScale, m_OneTickFrame);
			gameobject->transform->Scale.x = m_NowScale;
			gameobject->transform->Scale.y = m_NowScale;
			gameobject->transform->Scale.z = m_NowScale;
		}
	}
	else
	{
		Reset();
	}
}

void Particle::SetPlay(const PARTICLE_DESC* particleDesc)
{
	m_Playing = true;

	// Animation Type 재설정..
	m_AniType = 0;

	// 현재 파티클 재생시간 설정..
	m_LifeTime = particleDesc->LifeTime;

	// 현재 파티클 Data 설정..
	m_ParticleData->Playing = true;

	// Texture Frame Data 설정..
	m_TexOneFrame = particleDesc->LifeTime / (float)m_TexTotalFrame;
	m_TexFrame = 1;
	m_TexNowFrame = 0;

	// Animation Frame Data 설정..
	m_AniOneFrame = particleDesc->LifeTime / (float)m_AniTotalFrame;
	m_AniFrame = 1;
	m_AniNextFrame = 0;

	// 파티클 색상 설정..
	Vector4 maxColor = particleDesc->StartColor * m_SystemDesc->LifeTimeMaxColor;
	Vector4 minColor = particleDesc->StartColor * m_SystemDesc->LifeTimeMinColor;

	switch (m_SystemDesc->ColorType)
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		m_StartColor = maxColor;
		m_PrevColor = maxColor;
		m_OneColor = Vector4(0, 0, 0, 0);
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		m_StartColor = minColor;
		m_PrevColor = minColor;
		m_OneColor = (maxColor - minColor) / (float)m_AniTotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		m_StartColor = maxColor;
		m_PrevColor = maxColor;
		m_OneColor = (minColor - maxColor) / (float)m_AniTotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		m_StartColor = minColor;
		m_PrevColor = minColor;
		m_OneColor = (maxColor - minColor) / (float)m_ColorTurningFrame;
		break;
	default:
		break;
	}

	m_NextColor = m_PrevColor + m_OneColor;
	m_ParticleData->Color = m_PrevColor;

	// 파티클 사이즈 설정..
	float maxScale = particleDesc->StartScale * m_SystemDesc->LifeTimeMaxScale;
	float minScale = particleDesc->StartScale * m_SystemDesc->LifeTimeMinScale;

	// LifeTime Size Option..
	switch (m_SystemDesc->ScaleType)
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		m_StartScale = maxScale;
		m_PrevScale = maxScale;
		m_OneScale = 0;
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		m_StartScale = minScale;
		m_PrevScale = minScale;
		m_OneScale = (maxScale - minScale) / (float)m_AniTotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		m_StartScale = maxScale;
		m_PrevScale = maxScale;
		m_OneScale = (minScale - maxScale) / (float)m_AniTotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		m_StartScale = minScale;
		m_PrevScale = minScale;
		m_OneScale = (maxScale - minScale) / (float)m_ScaleTurningFrame;
		break;
	default:
		break;
	}

	m_NextScale = m_PrevScale + m_OneScale;
	gameobject->transform->Scale.x = m_PrevScale;
	gameobject->transform->Scale.y = m_PrevScale;

	// 파티클 회전 설정..
	m_StartRot = particleDesc->StartRot;
	m_OneRot = particleDesc->LifeRot / (float)m_AniTotalFrame;
	m_PrevRot = m_StartRot;
	m_NextRot = m_PrevRot + m_OneRot;
	gameobject->transform->Rotation.z = m_PrevRot;

	// 파티클 위치 설정..
	m_StartPos = particleDesc->StartPos;
	m_OnePos = (particleDesc->StartForce + particleDesc->LifeForce) / (float)m_AniTotalFrame;
	m_PrevPos = m_StartPos;
	m_NextPos = m_PrevPos + m_OnePos;
	gameobject->transform->Position = m_PrevPos;

	// Animation Type 설정..
	if (m_OneColor != XMVectorZero())	m_AniType |= COLOR_ANI;
	if (m_OnePos != XMVectorZero())		m_AniType |= POSITION_ANI;
	if (m_OneRot != 0.0f)				m_AniType |= ROTATION_ANI;
	if (m_OneScale != 0.0f)				m_AniType |= SCALE_ANI;
	if (m_TexTotalFrame > 1)			m_AniType |= TEXTURE_ANI;
}

void Particle::Reset()
{
	m_ParticleData->Playing = false;

	m_Playing = false;
	m_LifeTime = 0.0f;
	m_TexNowTime = 0.0f;
	m_AniNowTime = 0.0f;
}

void Particle::Release()
{
	m_Transform = nullptr;

	// 같은 Particle 끼리 공유하는 데이터는 ParticleSystem에서 해제함..
	// 포인터만 초기화..
	m_SystemDesc = nullptr;
	m_ParticleData = nullptr;
}
