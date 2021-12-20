#pragma once
#include "Scene.h"

class GameObject;
class Transform;
class MeshFilter;
class Rigidbody;
class AnimationController;

class  intro :public Scene
{
public:
	intro() {};
	~intro() {};

	// SceneA��(��|) AeC�� ��o��O��E
	virtual void Awake();
	virtual void Start() override;
	virtual void Update() override;
	virtual void End() override;
	

	GameObject* CreatePlayer(float x, float y, float z);
	void CreateTerrain();
	void CreateBox(float x,float y,float z);
private:
	GameObject* testobj;
	Transform* Tr;
	Rigidbody* Rig;
	MeshFilter* Mf;
	AnimationController* Ac;
};

