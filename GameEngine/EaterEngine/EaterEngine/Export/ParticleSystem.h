#pragma once
#include "EaterEngineDLL.h"
#include "RandomNumber.h"
#include "Particle.h"

typedef enum PARTICLE_RENDER_TYPE
{
	BILLBOARD,
	QUAD,
	MESH
}PARTICLE_RENDER_TYPE;

class MeshFilter;
class ParticleData;

class ParticleSystem : public Component
{
public:
	EATER_ENGINEDLL ParticleSystem();
	~ParticleSystem();

public:
	void Awake() override;
	void SetUp() override;
	void Start() override;
	void Update() override;

public:
	EATER_ENGINEDLL void SetMeshName(std::string meshName);
	EATER_ENGINEDLL void SetMaxParticles(int maxCount);
	EATER_ENGINEDLL void SetRangeLifeTime(float minTime, float maxTime);
	EATER_ENGINEDLL void SetRangeSpeed(float minSpeed, float maxSpeed);
	EATER_ENGINEDLL void SetRangeSize(float minSize, float maxSize);
	EATER_ENGINEDLL void SetRangeRotate(int minRot, int maxRot);
	EATER_ENGINEDLL void SetForceAxis(float x, float y, float z);

	EATER_ENGINEDLL void SetTextureTiling(float count_x, float count_y);
	EATER_ENGINEDLL void SetPlay(float playTime, bool loop = false);
	EATER_ENGINEDLL void SetRateOverTime(float count);

	EATER_ENGINEDLL void SetDiffuseName(std::string diffuseName);

	void Reset();

private:
	void SetParticleSystem();
	void AddParticle();

	void CreateParticle();

private:
	MeshFilter* m_MeshFilter;

private:
	ParticleData* m_ParticleData;

	std::vector<Particle*> m_Particles;

	PARTICLE_RENDER_TYPE Render_Type;

	std::string m_ParticleMeshName;
	std::string m_DiffuseName;

	bool m_Looping;				// 반복 재생 여부

	int m_MaxParticle;			// 최대 파티클 개수

	float m_PlayTime;			// 실행 지속 시간
	float m_NowTime;			// 현재 진행 시간
	float m_RateOverTime;		// 초당 출력할 파티클 개수

	float m_LifeTime_MinPoint;	// 파티클 유지 최소 시간
	float m_LifeTime_MaxPoint;	// 파티클 유지 최대 시간
	float m_Speed_MinPoint;		// 파티클 최소 속도
	float m_Speed_MaxPoint;		// 파티클 최대 속도
	float m_Size_MinPoint;		// 파티클 최소 크기
	float m_Size_MaxPoint;		// 파티클 최대 크기

	int Rotate_MinPoint;		// 파티클 최소 회전값
	int Rotate_MaxPoint;		// 파티클 최대 회전값

	RandomNumber<float> m_RandomLifeTime;
	RandomNumber<float> m_RandomSpeed;
	RandomNumber<float> m_RandomSize;
	RandomNumber<int> m_RandomRotate;
};