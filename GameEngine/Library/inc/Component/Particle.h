#pragma once

class Transform;

class Particle : public Component
{
public:
	Particle();
	~Particle();

public:
	friend class ParticleSystem;

public:
	void Start() override;
	void Update() override;

public:
	void SetPlay(float lifeTime, float speed, float size, int rot);
	void Reset();

private:
	bool m_Playing;

	int m_Index;

	int m_Rotate;
	float m_LifeTime;
	float m_Speed;
	float m_Size;

	float m_NowTime;
	float m_OneTick;

	int m_TexIndex;

	DirectX::SimpleMath::Matrix m_Tex;

public:
	static int g_Tile_WidthCount;
	static int g_Tile_HeightCount;

	static DirectX::SimpleMath::Vector3 g_Force;	// 사용자 지정 방향에 대한 속력
};
