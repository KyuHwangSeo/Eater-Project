#pragma once
#include "Component.h"
#include "EaterEngineDLL.h"
#include "LightHelper.h"

enum class eLightType
{
	DIRECTION,
	SPOT,
	POINT
};

/// <summary>
/// Light Base Class
/// </summary>

class LightManager;
class Transform;

class Light : public Component
{
public:
	Light(eLightType lightType);
	virtual ~Light() = default;

public:
	static void SetManager(LightManager* light);
	static void Reset();

public:
	eLightType GetType() { return m_LightType; }

protected:
	static LightManager* g_LightManager;

	Transform* m_Transform;
	eLightType m_LightType;
};

/// <summary>
/// DirectionLight Class
/// </summary>

class DirectionLight : public Light
{
public:
	EATER_ENGINEDLL DirectionLight();
	~DirectionLight();

public:
	static DirectionLight* g_DirLight;

public:
	void Awake() override;

public:
	void SetAmbient(float r, float g, float b, float a = 1.0f);
	void SetDiffuse(float r, float g, float b, float a = 1.0f);
	void SetSpecular(float r, float g, float b, float a = 1.0f);
	void SetDirection(float x, float y, float z);

public:
	void SetLight(DirectionalLightData& lightData);
	
public:
	void SetShadowRadius(float radius) { m_ShadowRadius = radius; }
	void SetCenterPos(DirectX::SimpleMath::Vector3 pos) { m_CenterPos = pos; }
	void SetLightViewProj();

public:
	DirectX::SimpleMath::Matrix GetView() { return m_LightView; }
	DirectX::SimpleMath::Matrix GetProj() { return m_LightProj; }

	DirectionalLightData* GetLightData();

private:
	DirectX::SimpleMath::Matrix m_LightView;
	DirectX::SimpleMath::Matrix m_LightProj;

	DirectX::SimpleMath::Vector3 m_CenterPos;
	float m_ShadowRadius;

	DirectionalLightData* m_DirLight;
};

/// <summary>
/// SpotLight Class
/// </summary>

class SpotLight : public Light
{
public:
	EATER_ENGINEDLL SpotLight();
	~SpotLight();

public:
	void Awake() override;
	void Update() override;

public:
	void SetAmbient(float r, float g, float b, float a = 1.0f);
	void SetDiffuse(float r, float g, float b, float a = 1.0f);
	void SetSpecular(float r, float g, float b, float a = 1.0f);
	void SetDirection(float x, float y, float z);

	void SetPosition(float x, float y, float z);
	void SetAtt(float x, float y, float z);

	void SetRange(float range);
	void SetSpot(float spot);

public:
	void SetLight(SpotLightData& lightData);

	SpotLightData* GetLightData();

private:
	SpotLightData* m_SpotLight;
};

/// <summary>
/// PointLight Class
/// </summary>

class PointLight : public Light
{
public:
	EATER_ENGINEDLL PointLight();
	~PointLight();

public:
	void Awake() override;
	void Update() override;

public:
	void SetAmbient(float r, float g, float b, float a = 1.0f);
	void SetDiffuse(float r, float g, float b, float a = 1.0f);
	void SetSpecular(float r, float g, float b, float a = 1.0f);

	void SetPosition(float x, float y, float z);
	void SetAtt(float x, float y, float z);

	void SetRange(float range);
	
public:
	void SetLight(PointLightData& lightData);

	PointLightData* GetLightData();

private:
	PointLightData* m_PointLight;
};
