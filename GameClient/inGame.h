#pragma once

#include "Scene.h"
class InGame : public Scene
{
public:
	InGame() {};
	~InGame() {};

	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void End();
};