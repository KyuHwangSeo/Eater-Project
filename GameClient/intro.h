#pragma once
#include "Scene.h"


class GameObject;
class  intro :public Scene
{
public:
	intro() {};
	~intro() {};
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void End();

private:
	GameObject* testobj;
};

