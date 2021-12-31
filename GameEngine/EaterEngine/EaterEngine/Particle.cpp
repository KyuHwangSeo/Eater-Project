#include "Component.h"
#include "Transform.h"
#include "EngineData.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include "TimeManager.h"
#include "LoadManager.h"
#include "GameObject.h"
#include <random>

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))

typedef enum PARTICLE_ANIMATION_TYPE : UINT
{
	COLOR_ANI		= 0x00000001,
	POSITION_ANI	= 0x00000010,
	ROTATION_ANI	= 0x00000100,
	SCALE_ANI		= 0x00001000
}PARTICLE_ANIMATION_TYPE;

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
	m_TotalFrame = m_SystemDesc->Total_Frame;

	m_TurningFrame = m_TotalFrame / 2 + 1;

	m_WidthCount = m_SystemDesc->Tile_Width;
	m_HeightCount = m_SystemDesc->Tile_Height;

	m_Tex = DirectX::SimpleMath::Matrix::CreateScale(1.0f / m_SystemDesc->Tile_Width, 1.0f / m_SystemDesc->Tile_Height, 1.0f);
}

void Particle::Update()
{
	if (m_Playing == false) return;

	float dTime = mTimeManager->DeltaTime();

	// �ش� ��ƼŬ ������Ʈ..
	if (m_LifeTime > 0.0f)
	{
		m_NowTime += dTime;

		if (m_NowTime >= m_OneFrame)
		{
			m_AddFrame = (int)(m_NowTime / m_OneFrame);
			m_NowFrame += m_AddFrame;
			m_LifeTime -= dTime;

			// UV ���� �� ����
			if (m_NowFrame > m_TotalFrame)
			{
				Reset();
			}
			else
			{
				// Texture ��� ���� ����..
				m_Tex._41 = (m_NowFrame % m_WidthCount) * m_Tex._11;
				m_Tex._42 = (m_NowFrame / m_HeightCount) * m_Tex._22;

				if (m_AniType & COLOR_ANI)
				{
					// Color ���� ����..
					m_PrevColor += m_OneColor * m_AddFrame;
					m_NextColor += m_OneColor * m_AddFrame;

					// Color ��ȯ�� ����..
					if (m_NowFrame == m_TurningFrame)
					{
						m_OneColor = -m_OneColor;
						m_NextColor = m_MaxColor + m_OneColor;
					}
				}

				if (m_AniType & POSITION_ANI)
				{
					// Position ���� ����..
					m_PrevPos += m_OnePos * m_AddFrame;
					m_NextPos += m_OnePos * m_AddFrame;
				}

				if (m_AniType & ROTATION_ANI)
				{
					// Rotation ���� ����..
					m_PrevRot += m_OneRot * m_AddFrame;
					m_NextRot += m_OneRot * m_AddFrame;
				}

				if (m_AniType & SCALE_ANI)
				{
					// Scale ���� ����..
					m_PrevScale += m_OneScale * m_AddFrame;
					m_NextScale += m_OneScale * m_AddFrame;

					// Scale ��ȯ�� ����..
					if (m_NowFrame == m_TurningFrame)
					{
						m_OneScale = -m_OneScale;
						m_NextScale = m_MaxScale + m_OneScale;
					}
				}

				m_NowTime = 0.0f;
			}
		}

		m_OneTickFrame = m_NowTime / m_OneFrame;

		// ��ƼŬ ������ ����..
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

	// Animation Type �缳��..
	m_AniType = 0;

	// ���� ��ƼŬ ����ð� ����..
	m_LifeTime = particleDesc->LifeTime;

	// ���� ��ƼŬ Data ����..
	m_ParticleData->Playing = true;

	// Texture ���� ���� ����..
	m_Tex._41 = 0;
	m_Tex._42 = 0;

	// �� ������ ��� �ð�..
	m_OneFrame = particleDesc->LifeTime / (float)m_TotalFrame;
	m_NowFrame = 1;

	// ��ƼŬ ���� ����..
	m_MaxColor = particleDesc->StartColor * m_SystemDesc->LifeTimeMaxColor;
	m_MinColor = particleDesc->StartColor * m_SystemDesc->LifeTimeMinColor;

	switch (m_SystemDesc->ColorType)
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		m_PrevColor = particleDesc->StartColor;
		m_OneColor = Vector4(0, 0, 0, 0);
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		m_PrevColor = m_MinColor;
		m_OneColor = (m_MaxColor - m_MinColor) / (float)m_TotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		m_PrevColor = m_MaxColor;
		m_OneColor = (m_MinColor - m_MaxColor) / (float)m_TotalFrame;
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		m_PrevColor = m_MinColor;
		m_OneColor = (m_MaxColor - m_MinColor) / (float)(m_TotalFrame / 2);
		break;
	default:
		break;
	}

	m_NextColor = m_PrevColor + m_OneColor;
	m_ParticleData->Color = m_PrevColor;

	// ��ƼŬ ������ ����..
	m_MaxScale = particleDesc->StartScale * m_SystemDesc->LifeTimeMaxScale;
	m_MinScale = particleDesc->StartScale * m_SystemDesc->LifeTimeMinScale;

	// LifeTime Size Option..
	switch (m_SystemDesc->SizeType)
	{
	case PARTICLE_LIFETIME_OPTION::NONE:
		m_PrevScale = m_MaxScale;
		m_OneScale = 0;
		break;
	case PARTICLE_LIFETIME_OPTION::UP:
		m_PrevScale = m_MinScale;
		m_OneScale = (m_MaxScale - m_MinScale) / (float)(m_TotalFrame);
		break;
	case PARTICLE_LIFETIME_OPTION::DOWN:
		m_PrevScale = m_MaxScale;
		m_OneScale = (m_MinScale - m_MaxScale) / (float)(m_TotalFrame);
		break;
	case PARTICLE_LIFETIME_OPTION::UPDOWN:
		m_PrevScale = m_MinScale;
		m_OneScale = (m_MaxScale - m_MinScale) / (float)(m_TotalFrame / 2);
		break;
	default:
		break;
	}

	m_NextScale = m_PrevScale + m_OneScale;
	gameobject->transform->Scale.x = m_PrevScale;
	gameobject->transform->Scale.y = m_PrevScale;

	// ��ƼŬ ȸ�� ����..
	m_OneRot = particleDesc->LifeRot / (float)m_TotalFrame;
	m_PrevRot = particleDesc->StartRot;
	m_NextRot = m_PrevRot + m_OneRot;
	gameobject->transform->Rotation.z = m_PrevRot;

	// ��ƼŬ ��ġ ����..
	m_OnePos = (particleDesc->StartForce + particleDesc->LifeForce) / (float)m_TotalFrame;
	m_PrevPos = particleDesc->StartPos;
	m_NextPos = m_PrevPos + m_OnePos;
	gameobject->transform->Position = m_PrevPos;

	// Animation Type ����..
	if (m_OneColor != XMVectorZero())	m_AniType |= COLOR_ANI;
	if (m_OnePos != XMVectorZero())		m_AniType |= POSITION_ANI;
	if (m_OneRot != 0.0f)				m_AniType |= ROTATION_ANI;
	if (m_OneScale != 0.0f)				m_AniType |= SCALE_ANI;
}

void Particle::Reset()
{
	m_ParticleData->Playing = false;

	m_Playing = false;
	m_NowTime = 0.0f;
}

void Particle::Release()
{
	m_Transform = nullptr;

	// ���� Particle ���� �����ϴ� �����ʹ� ParticleSystem���� ������..
	// �����͸� �ʱ�ȭ..
	m_SystemDesc = nullptr;
	m_ParticleData = nullptr;
}
