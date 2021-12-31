#pragma once

#include "EaterEngineDLL.h"
#include "Component.h"
#include <string>
class Transform;
class PhysData;
class Rigidbody : public Component
{
public:
	EATER_ENGINEDLL Rigidbody();
	EATER_ENGINEDLL ~Rigidbody();

	
	virtual void Start();
	virtual void SetUp();
	virtual void PhysicsUpdate();

	//Ÿ���� ���� �Ѵ�(static , Dinamic)
	EATER_ENGINEDLL void SetType(bool Dinamic);
	//�߷� ���� ����
	EATER_ENGINEDLL void SetGrvity(bool grvity);
	//
	EATER_ENGINEDLL void SetKinematic(bool kinematic);
	//���Ը� �����Ѵ�
	EATER_ENGINEDLL void SetMass(float mass);
	//������
	EATER_ENGINEDLL void SetRestitution(float mRestitution);


	//���ϴ� ���� �����̵��� ���� ��Ų��
	EATER_ENGINEDLL void SetFreezePosition(bool x,bool y,bool z);
	//���ϴ� ���� ����ȸ���� ���� ��Ų�� 
	EATER_ENGINEDLL void SetFreezeRotation(bool x,bool y,bool z);
	//rigdbody�� �̿��ؼ� ������
	EATER_ENGINEDLL void SetVelocity(float x, float y, float z);
	
	EATER_ENGINEDLL Vector3 GetVelocity();

	EATER_ENGINEDLL void SetAddForce(float x, float y, float z);

	EATER_ENGINEDLL void SetCenterPoint(float x, float y, float z);
		
	//�ݶ��̴� ����
	//���� ���� ����
	EATER_ENGINEDLL void CreateBoxCollider(float x, float y, float z);
	//������
	EATER_ENGINEDLL void CreateBoxCollider(float Radius);
	//������
	EATER_ENGINEDLL void CreateSphereCollider(float Radius);
	//������,����
	EATER_ENGINEDLL void CreateCapsuleCollider(float Radius, float Height);

	EATER_ENGINEDLL void CreateTriangleCollider(std::string MeshName);


	static const int BOX		= 0;
	static const int SPHERE		= 1;
	static const int CAPSULE	= 2;
private:
	Transform* Tr;
	PhysData* RigidbodyData;
	std::string ColliderName;
};

