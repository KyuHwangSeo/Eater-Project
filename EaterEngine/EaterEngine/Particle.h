#pragma once
#include "RandomNumber.h"

typedef enum PARTICLE_RENDER_TYPE
{
	BILLBOARD,
	QUAD,
	MESH
}PARTICLE_RENDER_TYPE;

class Particle
{
public:
	Particle();
	~Particle();

public:
	friend class ParticleSystem;

public:
	Transform* m_Transform;

public:
	void Update(float dTime);
	void TextureAnimationUpdate();
	void SetPlay(float lifeTime, float speed, float size, int rot);
	void Reset();

private:
	bool Playing;

	float LifeTime;
	float Speed;
	float Size;
	int Rotate;

	static DirectX::SimpleMath::Vector3 g_Force;	// ����� ���� ���⿡ ���� �ӷ�
};

class ParticleSystem : public Component
{
public:
	ParticleSystem();
	~ParticleSystem();

public:
	void Awake() override;
	void Start() override;
	void Update() override;

public:
	void SetRangeLifeTime(float minTime, float maxTime);
	void SetRangeSpeed(float minSpeed, float maxSpeed);
	void SetRangeSize(float minSize, float maxSize);
	void SetRangeRotate(int minRot, int maxRot);
	void SetForceAxis(float x, float y, float z);

	void SetTextureTiling(float count_x, float count_y);
	void SetPlay(float playTime, bool loop = false);
	void SetRateOverTime(float count);

	void Reset();
private:
	void AddParticle();
	void CreateParticle();
	void UpdataeParticle(float dTime);

private:
	ParticleData* m_ParticleData;
	
	std::vector<Particle*> m_Particles;

	PARTICLE_RENDER_TYPE Render_Type;

	bool m_Looping;				// �ݺ� ��� ����

	int m_MaxParticle;			// �ִ� ��ƼŬ ����

	float m_PlayTime;			// ���� ���� �ð�
	float m_NowTime;			// ���� ���� �ð�
	float m_RateOverTime;		// �ʴ� ����� ��ƼŬ ����

	int m_Tile_WidthCount;
	int m_Tile_HeightCount;

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