#pragma once

// Particle Render Data
typedef struct PARTICLE_SHARE_DESC
{
	PARTICLE_LIFETIME_OPTION SizeType;
	PARTICLE_LIFETIME_OPTION ColorType;

	int Total_Frame = 1;

	int Tile_Width = 1;
	int Tile_Height = 1;

	float LifeTimeMinScale = 1.0f;
	float LifeTimeMaxScale = 1.0f;

	DirectX::SimpleMath::Vector4 LifeTimeMinColor = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::SimpleMath::Vector4 LifeTimeMaxColor = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}PARTICLE_SHARE_DESC;

typedef struct PARTICLE_DESC
{
	float LifeTime;

	float StartScale;
	float StartRot;
	Vector4 StartColor;
	Vector3 StartForce;
	Vector3 StartPos;

	float LifeRot;
	Vector3 LifeForce;
}PARTICLE_DESC;

class Particle : public Component
{
public:
	Particle();
	~Particle();

public:
	friend class ParticleSystem;

public:
	void Awake() override;
	void Start() override;
	void Update() override;

public:
	void SetPlay(const PARTICLE_DESC* particleDesc);
	void Reset();

public:
	void Release();

private:
	Transform* m_Transform;

	OneParticle* m_ParticleData;
	PARTICLE_SHARE_DESC* m_SystemDesc;

	bool m_Playing;

	int m_Index;

	float m_LifeTime;
	float m_OneTickFrame;

	int m_WidthCount;
	int m_HeightCount;

	int m_TotalFrame;
	int m_TurningFrame;
	int m_NowFrame;
	int m_AddFrame;

	float m_OneFrame;
	float m_NowTime;

	float m_MaxScale;
	float m_MinScale;
	float m_OneScale;
	float m_PrevScale;
	float m_NowScale;
	float m_NextScale;

	float m_OneRot;
	float m_PrevRot;
	float m_NowRot;
	float m_NextRot;

	UINT m_AniType;

	DirectX::SimpleMath::Vector3 m_OnePos;
	DirectX::SimpleMath::Vector3 m_PrevPos;
	DirectX::SimpleMath::Vector3 m_NowPos;
	DirectX::SimpleMath::Vector3 m_NextPos;

	DirectX::SimpleMath::Vector4 m_MinColor;
	DirectX::SimpleMath::Vector4 m_MaxColor;
	DirectX::SimpleMath::Vector4 m_OneColor;
	DirectX::SimpleMath::Vector4 m_PrevColor;
	DirectX::SimpleMath::Vector4 m_NowColor;
	DirectX::SimpleMath::Vector4 m_NextColor;

	DirectX::SimpleMath::Matrix m_Tex;
};