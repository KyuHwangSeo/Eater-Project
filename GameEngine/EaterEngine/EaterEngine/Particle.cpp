#include "Component.h"
#include "Transform.h"
#include "EngineData.h"
#include "Particle.h"
#include "TimeManager.h"
#include "LoadManager.h"
#include "GameObject.h"
#include <random>

int Particle::g_Tile_WidthCount;
int Particle::g_Tile_HeightCount;
DirectX::SimpleMath::Vector3 Particle::g_Force;

Particle::Particle()
{
}

Particle::~Particle()
{

}

void Particle::Start()
{
	m_Tex = DirectX::SimpleMath::Matrix::CreateScale( 1.0f / g_Tile_WidthCount, 1.0f / g_Tile_HeightCount, 1.0f);
}

void Particle::Update()
{
	if (m_Playing == false) return;

	float dTime = mTimeManager->DeltaTime();

	// 해당 파티클 업데이트..
	if (m_LifeTime > 0.0f)
	{
		m_NowTime += dTime;

		if (m_NowTime >= m_OneTick)
		{
			m_TexIndex++;
			m_LifeTime -= dTime;

			// UV 변경 및 설정
			if (m_TexIndex >= g_Tile_WidthCount * g_Tile_HeightCount)
			{
				Reset();
			}
			else
			{
				m_Tex._41 = (m_TexIndex % g_Tile_WidthCount) * m_Tex._11;
				m_Tex._42 = (m_TexIndex / g_Tile_HeightCount) * m_Tex._22;
			}

			m_NowTime = 0.0f;
		}
	}
	gameobject->transform->Position.y += 1.0f * dTime;
	gameobject->OneMeshData->mTexTM = m_Tex;
}

void Particle::SetPlay(float lifeTime, float speed, float size, int rot)
{
	m_Playing = true;

	m_LifeTime = lifeTime;
	m_OneTick = lifeTime / (g_Tile_WidthCount * g_Tile_HeightCount);
	m_Speed = speed;
	m_TexIndex = 1;

	//gameobject->transform->SetScale(size, size, 1.0f);
}

void Particle::Reset()
{
	m_Playing = false;
	m_NowTime = 0.0f;
}