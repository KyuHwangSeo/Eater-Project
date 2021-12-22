#include "EngineData.h"
#include "Component.h"
#include "Transform.h"
#include "ParticleSystem.h"

#include "Material.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "LoadManager.h"
#include "ObjectManager.h"
#include "MeshFilter.h"

ParticleSystem::ParticleSystem()
{
	m_ParticleData = new ParticleData();
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Awake()
{
	// Awake에선 GetComponent로 필요한 Component 미리 얻어온다..
	m_MeshFilter = gameobject->GetComponent<MeshFilter>();
}

void ParticleSystem::SetUp()
{
	// Particle DiffuseMap 설정..
	m_MeshFilter->SetTextureName(m_DiffuseName);

	// Particle Mesh 설정..
	m_MeshFilter->SetMeshName(m_ParticleMeshName);
}

void ParticleSystem::Start()
{
	// Particle System Data 설정..
	SetParticleSystem();

	// Particle 생성..
	AddParticle();

	mTimeManager->ResetTime();
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
			return Reset();
		}
	}

	// 현재 파티클 출력할 시간이 지낫다면 실행..
	if (m_RateOverTime <= m_NowTime)
	{
		CreateParticle();
	}
}

void ParticleSystem::SetMeshName(std::string meshName)
{
	m_ParticleMeshName = meshName;
}

void ParticleSystem::SetMaxParticles(int maxCount)
{
	m_MaxParticle = maxCount;
	m_ParticleData->Particle_Count = maxCount;
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
	Particle::g_Tile_WidthCount = count_x;
	Particle::g_Tile_HeightCount = count_y;
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

void ParticleSystem::SetDiffuseName(std::string diffuseName)
{
	m_DiffuseName = diffuseName;
}

void ParticleSystem::Reset()
{
	for (Particle* particle : m_Particles)
	{
		particle->Reset();
	}
}

void ParticleSystem::SetParticleSystem()
{
	// Particle 관련 Data 설정..
	gameobject->OneMeshData->ObjType = OBJECT_TYPE::PARTICLE;
	gameobject->OneMeshData->Particle_Data = m_ParticleData;
}

void ParticleSystem::AddParticle()
{
	// ParticleSystem Transform..
	Transform* systemTransform = gameobject->GetTransform();

	// 최대 Particle 개수만큼 만들어 둔다..
	for (int index = 0; index < m_MaxParticle; index++)
	{
		// 새로운 Particle..
		GameObject* newObject = new GameObject();
		Particle* newParticle = newObject->AddComponent<Particle>();
		Transform* particleTransform = newObject->AddComponent<Transform>();

		newObject->transform = particleTransform;

		// 해당 Particle Index 설정..
		newParticle->m_Index = index;

		// 생성한 Particle을 ParticleSystem 하위 객체로 연결..
		systemTransform->SetChild(particleTransform);
		particleTransform->SetParent(systemTransform);

		// Particle List 삽입..
		m_Particles.push_back(newParticle);
		m_ParticleData->World_List.push_back(particleTransform->GetWorld());
		m_ParticleData->Tex_List.push_back(&newParticle->m_Tex);
	}
}

void ParticleSystem::CreateParticle()
{
	for (Particle* particle : m_Particles)
	{
		// 실행 상태의 파티클이 아닌경우 출력..
		if (particle->m_Playing == false)
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