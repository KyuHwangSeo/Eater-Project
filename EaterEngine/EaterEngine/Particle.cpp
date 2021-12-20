#include "Component.h"
#include "Transform.h"
#include "EngineData.h"
#include "Material.h"
#include "Particle.h"
#include "TimeManager.h"
#include <random>

DirectX::SimpleMath::Vector3 Particle::g_Force;

Particle::Particle()
{
	m_Transform = new Transform();
}

Particle::~Particle()
{

}

void Particle::Update(float dTime)
{
	if (Playing == false) return;

	// 해당 파티클 업데이트..
	if (LifeTime > 0.0f)
	{

	}
}

void Particle::TextureAnimationUpdate()
{

}

void Particle::SetPlay(float lifeTime, float speed, float size, int rot)
{
	Playing = true;

	LifeTime = lifeTime;
	Speed = speed;

	m_Transform->SetScale(size, size, size);
}

void Particle::Reset()
{
	Playing = false;
	LifeTime = 0.0f;
	Speed = 0.0f;
	Size = 1.0f;
}

ParticleSystem::ParticleSystem()
{
	m_ParticleData = new ParticleData();
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Awake()
{


}

void ParticleSystem::Start()
{
	// Particle 생성..
	AddParticle();
}

void ParticleSystem::Update()
{
	float dTime = mTimeManager->DeltaTime();
	m_NowTime += dTime;

	if (m_Looping == false)
	{
		if (m_PlayTime > 0.0f)
		{
			m_PlayTime -= dTime;
		}
		else
		{
			Reset();
			return;
		}
	}

	// 현재 파티클 출력할 시간이 지낫다면 실행..
	if (m_RateOverTime >= m_NowTime)
	{
		CreateParticle();
	}

	// 파티클 업데이트..
	UpdataeParticle(dTime);
}

void ParticleSystem::SetRangeLifeTime(float minTime, float maxTime)
{
	m_LifeTime_MinPoint = minTime;
	m_LifeTime_MaxPoint = maxTime;

	m_RandomLifeTime.SetRange(minTime, maxTime);
}

void ParticleSystem::SetRangeSpeed(float minSpeed, float maxSpeed)
{
	m_Speed_MinPoint = minSpeed;
	m_Speed_MaxPoint = maxSpeed;

	m_RandomSpeed.SetRange(minSpeed, maxSpeed);
}

void ParticleSystem::SetRangeSize(float minSize, float maxSize)
{
	m_Size_MinPoint = minSize;
	m_Size_MaxPoint = maxSize;

	m_RandomSize.SetRange(minSize, maxSize);
}

void ParticleSystem::SetRangeRotate(int minRot, int maxRot)
{
	Rotate_MinPoint = minRot;
	Rotate_MaxPoint = maxRot;

	m_RandomRotate.SetRange(minRot, maxRot);
}

void ParticleSystem::SetForceAxis(float x, float y, float z)
{
	Particle::g_Force = { x,y,z };
}

void ParticleSystem::SetTextureTiling(float count_x, float count_y)
{
	m_Tile_WidthCount = count_x;
	m_Tile_HeightCount = count_y;
}

void ParticleSystem::SetPlay(float playTime, bool loop /*= false*/)
{
	m_PlayTime = playTime;
	m_Looping = loop;
}

void ParticleSystem::SetRateOverTime(float count)
{
	// 1개의 파티클이 출력되기까지의 시간..
	m_RateOverTime = 1.0f / count;
}

void ParticleSystem::Reset()
{
	for (Particle* particle : m_Particles)
	{
		particle->Reset();
	}
}

void ParticleSystem::AddParticle()
{
	// 최대 Particle 개수만큼 만들어 둔다..
	for (int i = 0; i < m_MaxParticle; i++)
	{
		// 새로운 Particle..
		Particle* newParticle = new Particle();
		Transform* particleTransform = newParticle->m_Transform;

		// Particle List 삽입..
		m_Particles.push_back(newParticle);

		// Renderer에 넘길 Particle World Matrix 삽입..
		m_ParticleData->m_Particles.push_back(particleTransform->GetWorld());
	}
}

void ParticleSystem::CreateParticle()
{
	for (Particle* particle : m_Particles)
	{
		// 실행 상태의 파티클이 아닌경우 출력..
		if (particle->Playing == false)
		{
			particle->SetPlay(m_RandomLifeTime.GetRandomNumber(),
							  m_RandomSpeed.GetRandomNumber(), 
							  m_RandomSize.GetRandomNumber(), 
							  m_RandomRotate.GetRandomNumber());
			break;
		}
	}

	// 체크용 현재 시간 초기화..
	m_NowTime = 0.0f;
}

void ParticleSystem::UpdataeParticle(float dTime)
{
	for (Particle* particle : m_Particles)
	{
		particle->Update(dTime);
	}
}
