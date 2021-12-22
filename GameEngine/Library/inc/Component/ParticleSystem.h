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

	bool m_Looping;				// �ݺ� ��� ����

	int m_MaxParticle;			// �ִ� ��ƼŬ ����

	float m_PlayTime;			// ���� ���� �ð�
	float m_NowTime;			// ���� ���� �ð�
	float m_RateOverTime;		// �ʴ� ����� ��ƼŬ ����

	float m_LifeTime_MinPoint;	// ��ƼŬ ���� �ּ� �ð�
	float m_LifeTime_MaxPoint;	// ��ƼŬ ���� �ִ� �ð�
	float m_Speed_MinPoint;		// ��ƼŬ �ּ� �ӵ�
	float m_Speed_MaxPoint;		// ��ƼŬ �ִ� �ӵ�
	float m_Size_MinPoint;		// ��ƼŬ �ּ� ũ��
	float m_Size_MaxPoint;		// ��ƼŬ �ִ� ũ��

	int Rotate_MinPoint;		// ��ƼŬ �ּ� ȸ����
	int Rotate_MaxPoint;		// ��ƼŬ �ִ� ȸ����

	RandomNumber<float> m_RandomLifeTime;
	RandomNumber<float> m_RandomSpeed;
	RandomNumber<float> m_RandomSize;
	RandomNumber<int> m_RandomRotate;
};