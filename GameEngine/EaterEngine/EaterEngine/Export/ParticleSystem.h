#pragma once
#include "EaterEngineDLL.h"

struct PARTICLE_DESC;

class MeshFilter;
class Particle;
class ParticleData;
class RandomInt;
class RandomFloat;
class RandomVector3;
class RandomVector4;

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
	EATER_ENGINEDLL void SetMeshName(std::string meshName);					// 파티클 출력할 매쉬 타입
	EATER_ENGINEDLL void SetMaxParticles(int maxCount);						// 최대 파티클 출력 개수
	EATER_ENGINEDLL void SetStartColor(Vector4 color1, Vector4 color2);		// 파티클 색상
	EATER_ENGINEDLL void SetStartLifeTime(float minTime, float maxTime);	// 파티클 랜덤 유지시간
	EATER_ENGINEDLL void SetStartSpeed(float minSpeed, float maxSpeed);		// 파티클 랜덤 속도
	EATER_ENGINEDLL void SetStartSize(float minSize, float maxSize);		// 파티클 랜덤 크기
	EATER_ENGINEDLL void SetStartRotation(int minRot, int maxRot);			// 파티클 랜덤 방향
	EATER_ENGINEDLL void SetStartPosition(float radius);					// 파티클 랜덤 생성 범위

	EATER_ENGINEDLL void SetLifeTimeRotation(int minRot, int maxRot);		// 파티클 고정 회전 범위
	EATER_ENGINEDLL void SetForceAxis(float x, float y, float z);			// 파티클 고정 방향
	EATER_ENGINEDLL void SetRateOverTime(float count);						// 1초에 출력할 파티클 개수

	EATER_ENGINEDLL void SetTextureTiling(int count_x, int count_y);		// 파티클 텍스쳐 나눌 개수
	EATER_ENGINEDLL void SetPlay(float playTime, bool loop = false);		// 파티클 시스템 플레이 시간 및 반복 여부

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
	PARTICLE_DESC* m_Particle_Desc;

	std::vector<Particle*> m_Particles;

	std::string m_ParticleMeshName;
	std::string m_DiffuseName;

	bool m_Looping;				// 반복 재생 여부
	int m_MaxParticle;			// 최대 파티클 개수

	float m_PlayTime;			// 실행 지속 시간
	float m_NowTime;			// 현재 진행 시간
	float m_RateOverTime;		// 초당 출력할 파티클 개수

	RandomFloat*	m_RandomLifeTime;
	RandomFloat*	m_RandomSpeed;
	RandomFloat*	m_RandomStartSize;
	RandomVector3*	m_RandomStartPosition;
	RandomVector4*	m_RandomStartColor;
	RandomInt*		m_RandomStartRotation;
	RandomInt*		m_RandomLifeTimeRotation;
};