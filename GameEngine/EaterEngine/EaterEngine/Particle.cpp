#include "Component.h"
#include "Transform.h"
#include "EngineData.h"
#include "Particle.h"
#include "TimeManager.h"
#include "LoadManager.h"
#include "GameObject.h"
#include <random>

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

	m_Particle_Data->Tex = &m_Tex;
	m_Particle_Data->World = m_Transform->GetWorld();
}

void Particle::Start()
{
	m_WidthCount = m_Particle_Desc->Tile_Width;
	m_HeightCount = m_Particle_Desc->Tile_Height;

	m_Tex = DirectX::SimpleMath::Matrix::CreateScale(1.0f / m_WidthCount, 1.0f / m_HeightCount, 1.0f);
}

void Particle::Update()
{
	if (m_Playing == false) return;

	float dTime = mTimeManager->DeltaTime();

	// 해당 파티클 업데이트..
	if (m_LifeTime > 0.0f)
	{
		m_NowTime += dTime;

		if (m_NowTime >= m_OneFrame)
		{
			m_NowFrame++;
			m_LifeTime -= dTime;

			// UV 변경 및 설정
			if (m_NowFrame >= m_TotalFrame)
			{
				Reset();
			}
			else
			{
				// Texture 출력 영역 변경..
				m_Tex._41 = (m_NowFrame % m_WidthCount) * m_Tex._11;
				m_Tex._42 = (m_NowFrame / m_HeightCount) * m_Tex._22;

				// Rotation 범위 변경..
				m_PrevRot += m_OneRot;
				m_NextRot += m_OneRot;

				// Position 범위 변경..
				m_PrevPos += m_OnePos;
				m_NextPos += m_OnePos;

				// Scale 범위 변경..
				if (m_ScaleUp)
				{
					m_PrevScale += m_OneScale;
					m_NextScale += m_OneScale;
				}
				else
				{
					m_PrevScale -= m_OneScale;
					m_NextScale -= m_OneScale;
				}

				// Scale 전환점 설정..
				if (m_NextScale >= m_MaxScale)
				{
					m_ScaleUp = false;
					m_NextScale = m_MaxScale - m_OneScale;
				}
				m_NowTime = 0.0f;
			}
		}

		m_OneTickFrame = m_NowTime / m_OneFrame;
		m_NowScale = LERP(m_PrevScale, m_NextScale, m_OneTickFrame);
		m_NowRot = LERP(m_PrevRot, m_NextRot, m_OneTickFrame);
		m_NowPos = Vector3::Lerp(m_PrevPos, m_NextPos, m_OneTickFrame);

		// 파티클 자체 회전 & 스케일 보간..
		gameobject->transform->Rotation.z = m_NowRot;
		gameobject->transform->Scale.x = m_NowScale;
		gameobject->transform->Scale.y = m_NowScale;
		gameobject->transform->Position = m_NowPos;
	}
}

void Particle::SetPlay(float lifeTime, Vector4 startColor, Vector3 startPos, float startScale, int startRot, int lifeRot)
{
	m_Playing = true;

	// 현재 파티클 재생시간 설정..
	m_LifeTime = lifeTime;

	// 현재 파티클 Data 설정..
	m_Particle_Data->Playing = true;
	m_Particle_Data->Color = startColor;

	// Texture Animation 총 프레임..
	m_TotalFrame = m_WidthCount * m_HeightCount;

	// Texture 시작 지점 설정..
	m_Tex._41 = 0;
	m_Tex._42 = 0;

	// 한 프레임 재생 시간..
	m_OneFrame = lifeTime / (float)m_TotalFrame;
	m_NowFrame = 1;

	// 파티클 최대 사이즈 설정..
	m_ScaleUp = true;
	m_MaxScale = startScale;
	m_OneScale = startScale / (float)m_TotalFrame * 2.0f;
	m_PrevScale = 0.0f;
	m_NextScale = m_OneScale;

	// 한 프레임 회전 범위..
	m_OneRot = (float)lifeRot / (float)m_TotalFrame;
	m_PrevRot = (float)startRot;
	m_NextRot = m_PrevRot + m_OneRot;

	// 파티클 출력 시작 지점 설정..
	m_OnePos = (m_Particle_Desc->Force + Vector3(0.0f, 0.0f, 1.0f)) / (float)m_TotalFrame;
	m_PrevPos = startPos;
	m_NextPos = m_PrevPos + m_OnePos;
}

void Particle::Reset()
{
	m_Particle_Data->Playing = false;

	m_Playing = false;
	m_NowTime = 0.0f;
}