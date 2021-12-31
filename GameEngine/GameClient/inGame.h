#pragma once
#include <vector>
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
	GameObject* ParticleObj;
	GameObject* ParticleObj_1;
	GameObject* ParticleObj_2;
	GameObject* ParticleObj_3;

	std::vector<ParticleSystem*> ParticleList;
	std::vector<ParticleSystem*> ParticleList_1;
	std::vector<ParticleSystem*> ParticleList_2;
	std::vector<ParticleSystem*> ParticleList_3;

	ParticleSystem* particles;
};