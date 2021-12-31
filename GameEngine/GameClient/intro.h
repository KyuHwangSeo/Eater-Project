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

	
	virtual void Awake();
	virtual void Start() override;
	virtual void Update() override;
	virtual void End() override;
	

	void CreatePlayer();
	void CreatePlayerNetwork();
	void CreateTerrain();
	void CreateObject(float x,float y,float z,int type);
	void CreateMap();
private:
	GameObject* testobj;

	GameObject* BossA;
	GameObject* BossB;


	//Transform* Tr;
	//Rigidbody* Rig;
	//MeshFilter* Mf;
	//AnimationController* Ac;
};

