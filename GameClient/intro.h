#pragma once
#include "Scene.h"

class GameObject;
class Transform;
class MeshFilter;
class Rigidbody;
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
	Transform* Tr;
	Rigidbody* Rig;
	MeshFilter* Mf;
};

