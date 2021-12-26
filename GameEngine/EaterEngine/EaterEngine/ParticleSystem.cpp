#include "EngineData.h"
#include "Component.h"
#include "Transform.h"
#include "Particle.h"
#include "ParticleSystem.h"

#include "Material.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "LoadManager.h"
#include "ObjectManager.h"
#include "MeshFilter.h"
#include "RandomNumber.h"

ParticleSystem::ParticleSystem()
{
	m_Particle_Desc = new PARTICLE_DESC();

	m_ParticleData = new ParticleData();

	m_RandomLifeTime			= new RandomFloat();
	m_RandomSpeed				= new RandomFloat();
	m_RandomStartSize			= new RandomFloat();
	m_RandomStartPosition		= new RandomVector3();
	m_RandomStartColor			= new RandomVector4();
	m_RandomStartRotation		= new RandomInt();
	m_RandomLifeTimeRotation	= new RandomInt();
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Awake()
{
	// Awake���� GetComponent�� �ʿ��� Component �̸� ���´�..
	m_MeshFilter = gameobject->GetComponent<MeshFilter>();
}

void ParticleSystem::SetUp()
{
	// Particle DiffuseMap ����..
	m_MeshFilter->SetTextureName(m_DiffuseName);

	// Particle Mesh ����..
	m_MeshFilter->SetMeshName(m_ParticleMeshName);
}

void ParticleSystem::Start()
{
	// Particle System Data ����..
	SetParticleSystem();

	// Particle ����..
	AddParticle();

	mTimeManager->ResetTime();
}

void ParticleSystem::Update()
{
	float dTime = mTimeManager->DeltaTime();

	if (m_PlayTime > 0.0f)
	{
		m_NowTime += dTime;
		m_PlayTime -= dTime;

		// ���� ��ƼŬ ����� �ð��� �����ٸ� ����..
		if (m_RateOverTime <= m_NowTime)
		{
			CreateParticle();
		}
	}
	else
	{
		if (m_Looping)
		{
			// ���� ��ƼŬ ����� �ð��� �����ٸ� ����..
			if (m_RateOverTime <= m_NowTime)
			{
				CreateParticle();
			}

			m_PlayTime = 10.0f;
		}
		else
		{
			m_NowTime = 0.0f;
			m_PlayTime = 0.0f;
		}
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

void ParticleSystem::SetStartColor(Vector4 color1, Vector4 color2)
{
	m_RandomStartColor->SetRange(color1, color2);
}

void ParticleSystem::SetStartLifeTime(float minTime, float maxTime)
{
	m_RandomLifeTime->SetRange(minTime, maxTime);
}

void ParticleSystem::SetStartSpeed(float minSpeed, float maxSpeed)
{
	m_RandomSpeed->SetRange(minSpeed, maxSpeed);
}

void ParticleSystem::SetStartSize(float minSize, float maxSize)
{
	m_RandomStartSize->SetRange(minSize, maxSize);
}

void ParticleSystem::SetStartRotation(int minRot, int maxRot)
{
	m_RandomStartRotation->SetRange(minRot, maxRot);
}

void ParticleSystem::SetStartPosition(float radius)
{
	m_ParticleData->Area_Radius = radius;

	m_RandomStartPosition->SetRange(Vector3(-radius, -radius, -radius), Vector3(radius, radius, radius));
}

void ParticleSystem::SetLifeTimeRotation(int minRot, int maxRot)
{
	m_RandomLifeTimeRotation->SetRange(minRot, maxRot);
}

void ParticleSystem::SetForceAxis(float x, float y, float z)
{
	m_Particle_Desc->Force = { x,y,z };
}

void ParticleSystem::SetRateOverTime(float count)
{
	// 1���� ��ƼŬ�� ��µǱ������ �ð�..
	m_RateOverTime = 1.0f / count;
}

void ParticleSystem::SetTextureTiling(int count_x, int count_y)
{
	m_Particle_Desc->Tile_Width = count_x;
	m_Particle_Desc->Tile_Height = count_y;
}

void ParticleSystem::SetPlay(float playTime, bool loop /*= false*/)
{
	m_PlayTime = playTime;
	m_Looping = loop;
}

void ParticleSystem::SetDiffuseName(std::string diffuseName)
{
	m_DiffuseName = diffuseName;
}

void ParticleSystem::Reset()
{
	m_PlayTime = 0.0f;
	m_NowTime = 0.0f;
}

void ParticleSystem::SetParticleSystem()
{
	// Particle ���� Data ����..
	gameobject->OneMeshData->ObjType = OBJECT_TYPE::PARTICLE;
	gameobject->OneMeshData->Particle_Data = m_ParticleData;
}

void ParticleSystem::AddParticle()
{
	// ParticleSystem Transform..
	Transform* systemTransform = gameobject->GetTransform();

	// �ִ� Particle ������ŭ ����� �д�..
	for (int index = 0; index < m_MaxParticle; index++)
	{
		// ���ο� Particle..
		GameObject* newObject = new GameObject();
		Particle* newParticle = newObject->AddComponent<Particle>();
		Transform* particleTransform = newObject->AddComponent<Transform>();

		// �ش� Particle Transform ����..
		newObject->transform = particleTransform;

		// �ش� Particle Data ����..
		newParticle->m_Index = index;
		newParticle->m_Particle_Desc = m_Particle_Desc;

		// ������ Particle�� ParticleSystem ���� ��ü�� ����..
		systemTransform->SetChild(particleTransform);
		particleTransform->SetParent(systemTransform);

		// Particle Data ����..
		OneParticle* newParticleData = new OneParticle();
		newParticle->m_Particle_Data = newParticleData;

		// Particle List ����..
		m_Particles.push_back(newParticle);
		m_ParticleData->m_Particles.push_back(newParticleData);
	}
}

void ParticleSystem::CreateParticle()
{
	for (Particle* particle : m_Particles)
	{
		// ���� ������ ��ƼŬ�� �ƴѰ�� ���..
		if (particle->m_Playing == false)
		{
			particle->SetPlay(m_RandomLifeTime->GetRandomNumber(),
							  m_RandomStartColor->GetRandomNumber(),
							  m_RandomStartPosition->GetRandomNumber(),
							  m_RandomStartSize->GetRandomNumber(),
							  m_RandomStartRotation->GetRandomNumber(),
							  m_RandomLifeTimeRotation->GetRandomNumber());
			break;
		}
	}

	// üũ�� ���� �ð� �ʱ�ȭ..
	m_NowTime = 0.0f;
}