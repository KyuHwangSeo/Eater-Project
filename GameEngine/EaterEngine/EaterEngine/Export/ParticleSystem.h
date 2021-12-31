#pragma once
#include "EaterEngineDLL.h"

struct PARTICLE_SHARE_DESC;
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
	EATER_ENGINEDLL void SetMeshName(std::string meshName);								// 파티클 출력할 매쉬 타입
	EATER_ENGINEDLL void SetRenderType(PARTICLE_RENDER_OPTION renderType);				// 파티클 출력할 랜더 타입
	EATER_ENGINEDLL void SetMaxParticles(int maxCount);									// 최대 파티클 출력 개수
	EATER_ENGINEDLL void SetDelayTime(float delay);										// 파티클 실행 전 지연 시간
	EATER_ENGINEDLL void SetRateOverTime(float count);									// 1초에 출력할 파티클 개수
	EATER_ENGINEDLL void SetShapeRadius(float radius);									// 파티클 랜덤 생성 범위

	EATER_ENGINEDLL void SetStartForce(Vector3 force);									// 파티클 고정 속력
	EATER_ENGINEDLL void SetStartForce(Vector3 minForce, Vector3 maxForce);				// 파티클 랜덤 속력
	EATER_ENGINEDLL void SetStartColor(Vector4 color);									// 파티클 고정 색상
	EATER_ENGINEDLL void SetStartColor(Vector4 minColor, Vector4 maxColor);				// 파티클 랜덤 색상
	EATER_ENGINEDLL void SetStartLifeTime(float time);									// 파티클 고정 유지시간
	EATER_ENGINEDLL void SetStartLifeTime(float minTime, float maxTime);				// 파티클 랜덤 유지시간
	EATER_ENGINEDLL void SetStartSize(float size);										// 파티클 고정 크기
	EATER_ENGINEDLL void SetStartSize(float minSize, float maxSize);					// 파티클 랜덤 크기
	EATER_ENGINEDLL void SetStartRotation(float rot);									// 파티클 고정 회전
	EATER_ENGINEDLL void SetStartRotation(float minRot, float maxRot);					// 파티클 랜덤 회전

	EATER_ENGINEDLL void SetLifeTimeForce(Vector3 minForce, Vector3 maxForce);			// 파티클 생성 후 랜덤 속력 범위
	EATER_ENGINEDLL void SetLifeTimeColor(Vector4 minColor, Vector4 maxColor, PARTICLE_LIFETIME_OPTION option);			// 파티클 생성 후 색상 범위
	EATER_ENGINEDLL void SetLifeTimeSize(float minSize, float maxSize, PARTICLE_LIFETIME_OPTION option);	// 파티클 생성 후 크기 범위
	EATER_ENGINEDLL void SetLifeTimeRotation(float rot);								// 파티클 생성 후 고정 회전 범위
	EATER_ENGINEDLL void SetLifeTimeRotation(float minRot, float maxRot);				// 파티클 생성 후 랜덤 회전 범위

	EATER_ENGINEDLL void SetTextureTiling(int count_x, int count_y);					// 파티클 텍스쳐 나눌 개수
	EATER_ENGINEDLL void SetPlayTime(float playTime, bool loop = false);					// 파티클 시스템 플레이 시간 및 반복 여부
	
	EATER_ENGINEDLL void SetDiffuseName(std::string diffuseName);

	EATER_ENGINEDLL void Play(bool loop = false);														// 파티클 시스템 플레이 시간 및 반복 여부

	void Reset();

public:
	void Release();

private:
	void StartPlay();
	void SetParticleSystem();
	void AddParticle();

	void CreateParticle();

private:
	MeshFilter* m_MeshFilter;

private:
	ParticleData* m_ParticleData;
	PARTICLE_SHARE_DESC* m_SystemDesc;
	PARTICLE_DESC* m_ParticleDesc;

	std::vector<Particle*> m_Particles;

	std::string m_ParticleMeshName;
	std::string m_DiffuseName;

	bool m_Looping;				// 반복 재생 여부
	bool m_Playing;				// 재생 여부
	int m_MaxParticle;			// 최대 파티클 개수
	int m_PlayCount;			// 이번에 실행할 파티클 개수

	float m_PlayTime;			// 실행 지속 시간
	float m_NowPlayTime;		// 현재 지속 시간
	float m_DelayTime;			// 실행 지연 시간
	float m_NowDelayTime;		// 현재 지연 시간
	float m_TickTime;			// 현재 진행 시간
	float m_RateOverTime;		// 초당 출력할 파티클 개수

	RandomFloat*	m_RandomLifeTime;
	RandomVector4*	m_RandomStartColor;
	RandomVector3*	m_RandomStartPosition;
	RandomVector3*	m_RandomStartForce;
	RandomVector3*	m_RandomLifeTimeForce;

	RandomFloat*	m_RandomStartSize;
	RandomFloat*	m_RandomStartRotation;
	RandomFloat*	m_RandomLifeTimeRotation;
};