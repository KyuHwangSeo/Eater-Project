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

	// �ش� ��ƼŬ ������Ʈ..
	if (m_LifeTime > 0.0f)
	{
		m_NowTime += dTime;

		if (m_NowTime >= m_OneFrame)
		{
			m_NowFrame++;
			m_LifeTime -= dTime;

			// UV ���� �� ����
			if (m_NowFrame >= m_TotalFrame)
			{
				Reset();
			}
			else
			{
				// Texture ��� ���� ����..
				m_Tex._41 = (m_NowFrame % m_WidthCount) * m_Tex._11;
				m_Tex._42 = (m_NowFrame / m_HeightCount) * m_Tex._22;

				// Rotation ���� ����..
				m_PrevRot += m_OneRot;
				m_NextRot += m_OneRot;

				// Position ���� ����..
				m_PrevPos += m_OnePos;
				m_NextPos += m_OnePos;

				// Scale ���� ����..
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

				// Scale ��ȯ�� ����..
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

		// ��ƼŬ ��ü ȸ�� & ������ ����..
		gameobject->transform->Rotation.z = m_NowRot;
		gameobject->transform->Scale.x = m_NowScale;
		gameobject->transform->Scale.y = m_NowScale;
		gameobject->transform->Position = m_NowPos;
	}
}

void Particle::SetPlay(float lifeTime, Vector4 startColor, Vector3 startPos, float startScale, int startRot, int lifeRot)
{
	m_Playing = true;

	// ���� ��ƼŬ ����ð� ����..
	m_LifeTime = lifeTime;

	// ���� ��ƼŬ Data ����..
	m_Particle_Data->Playing = true;
	m_Particle_Data->Color = startColor;

	// Texture Animation �� ������..
	m_TotalFrame = m_WidthCount * m_HeightCount;

	// Texture ���� ���� ����..
	m_Tex._41 = 0;
	m_Tex._42 = 0;

	// �� ������ ��� �ð�..
	m_OneFrame = lifeTime / (float)m_TotalFrame;
	m_NowFrame = 1;

	// ��ƼŬ �ִ� ������ ����..
	m_ScaleUp = true;
	m_MaxScale = startScale;
	m_OneScale = startScale / (float)m_TotalFrame * 2.0f;
	m_PrevScale = 0.0f;
	m_NextScale = m_OneScale;

	// �� ������ ȸ�� ����..
	m_OneRot = (float)lifeRot / (float)m_TotalFrame;
	m_PrevRot = (float)startRot;
	m_NextRot = m_PrevRot + m_OneRot;

	// ��ƼŬ ��� ���� ���� ����..
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