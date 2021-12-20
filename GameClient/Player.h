#pragma once
#include "Component.h"

class AnimationController;
class Transform;
class MeshFilter;

class Player :public Component
{
public:
	Player();
	virtual ~Player();
	void Awake();
	void SetUp();
	void Start();
	void Update();
private:
	AnimationController* AC;
	Transform*	Tr;
	MeshFilter* Mf;

	GameObject* RightHand;
	GameObject* LeftHand;

	GameObject* Gun;
	GameObject* Weapon;

	bool Move;
};