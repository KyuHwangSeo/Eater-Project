#pragma once

// Particle Render Type
typedef enum PARTICLE_RENDER_TYPE
{
	BILLBOARD,
	QUAD,
	MESH
}PARTICLE_RENDER_TYPE;

// Particle Render Data
typedef struct PARTICLE_DESC
{
	PARTICLE_RENDER_TYPE RenderType;

	int Tile_Width;
	int Tile_Height;

	DirectX::SimpleMath::Vector3 Force;
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
	void SetPlay(float lifeTime, Vector4 startColor, Vector3 startPos, float startScale, int startRot, int lifeRot);
	void Reset();

private:
	Transform* m_Transform;

	OneParticle* m_Particle_Data;
	PARTICLE_DESC* m_Particle_Desc;

	bool m_Playing;

	int m_Index;

	float m_LifeTime;
	float m_OneTickFrame;

	int m_WidthCount;
	int m_HeightCount;

	int m_TotalFrame;
	int m_NowFrame;

	float m_OneFrame;
	float m_NowTime;

	bool m_ScaleUp;
	float m_MaxScale;
	float m_OneScale;
	float m_PrevScale;
	float m_NowScale;
	float m_NextScale;

	float m_OneRot;
	float m_PrevRot;
	float m_NowRot;
	float m_NextRot;

	DirectX::SimpleMath::Vector3 m_OnePos;
	DirectX::SimpleMath::Vector3 m_PrevPos;
	DirectX::SimpleMath::Vector3 m_NowPos;
	DirectX::SimpleMath::Vector3 m_NextPos;

	DirectX::SimpleMath::Matrix m_Tex;
	DirectX::SimpleMath::Vector4 m_Color;
};