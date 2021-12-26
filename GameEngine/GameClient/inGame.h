#pragma once

#include "Scene.h"
class ParticleSystem;
class InGame : public Scene
{
public:
	InGame() {};
	~InGame() {};

	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void End();

public:
	GameObject* testobj;
	ParticleSystem* particles;
};