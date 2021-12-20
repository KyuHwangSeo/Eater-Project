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

	static DirectX::SimpleMath::Vector3 g_Force;	// 사용자 지정 방향에 대한 속력
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

	bool m_Looping;				// 반복 재생 여부

	int m_MaxParticle;			// 최대 파티클 개수

	float m_PlayTime;			// 실행 지속 시간
	float m_NowTime;			// 현재 진행 시간
	float m_RateOverTime;		// 초당 출력할 파티클 개수

	int m_Tile_WidthCount;
	int m_Tile_HeightCount;

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