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

	void CreatePlayer();
	void CreateLignt();
	void CreateCam();
private:
	GameObject* testobj;
};

