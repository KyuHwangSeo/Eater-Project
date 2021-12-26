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
	EATER_ENGINEDLL void SetMeshName(std::string meshName);					// ��ƼŬ ����� �Ž� Ÿ��
	EATER_ENGINEDLL void SetMaxParticles(int maxCount);						// �ִ� ��ƼŬ ��� ����
	EATER_ENGINEDLL void SetStartColor(Vector4 color1, Vector4 color2);		// ��ƼŬ ����
	EATER_ENGINEDLL void SetStartLifeTime(float minTime, float maxTime);	// ��ƼŬ ���� �����ð�
	EATER_ENGINEDLL void SetStartSpeed(float minSpeed, float maxSpeed);		// ��ƼŬ ���� �ӵ�
	EATER_ENGINEDLL void SetStartSize(float minSize, float maxSize);		// ��ƼŬ ���� ũ��
	EATER_ENGINEDLL void SetStartRotation(int minRot, int maxRot);			// ��ƼŬ ���� ����
	EATER_ENGINEDLL void SetStartPosition(float radius);					// ��ƼŬ ���� ���� ����

	EATER_ENGINEDLL void SetLifeTimeRotation(int minRot, int maxRot);		// ��ƼŬ ���� ȸ�� ����
	EATER_ENGINEDLL void SetForceAxis(float x, float y, float z);			// ��ƼŬ ���� ����
	EATER_ENGINEDLL void SetRateOverTime(float count);						// 1�ʿ� ����� ��ƼŬ ����

	EATER_ENGINEDLL void SetTextureTiling(int count_x, int count_y);		// ��ƼŬ �ؽ��� ���� ����
	EATER_ENGINEDLL void SetPlay(float playTime, bool loop = false);		// ��ƼŬ �ý��� �÷��� �ð� �� �ݺ� ����

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

	bool m_Looping;				// �ݺ� ��� ����
	int m_MaxParticle;			// �ִ� ��ƼŬ ����

	float m_PlayTime;			// ���� ���� �ð�
	float m_NowTime;			// ���� ���� �ð�
	float m_RateOverTime;		// �ʴ� ����� ��ƼŬ ����

	RandomFloat*	m_RandomLifeTime;
	RandomFloat*	m_RandomSpeed;
	RandomFloat*	m_RandomStartSize;
	RandomVector3*	m_RandomStartPosition;
	RandomVector4*	m_RandomStartColor;
	RandomInt*		m_RandomStartRotation;
	RandomInt*		m_RandomLifeTimeRotation;
};