#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"


Rigidbody::Rigidbody()
{
	Component::TransformUpdate_Order	= Component::FUNCTION_ORDER_FIRST;
	Component::Start_Order				= Component::FUNCTION_ORDER_LAST;
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::Start()
{
	Tr = gameobject->GetTransform();

	PhysData data;
	data.WorldPosition	= { Tr->Position.x,Tr->Position.y,Tr->Position.z };
	data.Rotation		= { Tr->Rotation.x,Tr->Rotation.y,Tr->Rotation.z };
	data.Scale			= { Tr->Scale.x,Tr->Scale.y,Tr->Scale.z };

	data.MT_DynamicFriction = 0.5f;
	data.MT_StaticFriction	= 0.5f;
	data.MT_Restitution		= 0.6f;

	data.Collider_length(COLLIDER_TYPE::BOX, Tr->Scale.x, Tr->Scale.y, Tr->Scale.z);
	
	if (typeIndex == 0)
	{
		MyIndex = PhysX_Create_DinamicActor(data);
		data.Actor_TYPE	= ACTOR_TYPE::DINAMIC;
	}
	else
	{
		data.Actor_TYPE = ACTOR_TYPE::STATIC;
		MyIndex = PhysX_Create_StaticActor(data);
	}
}

void Rigidbody::TransformUpdate()
{
	PhysData temp;

	switch (typeIndex)
	{
	case (int)ACTOR_TYPE::DINAMIC:		temp =	Get_DinamicActors(MyIndex);		break;
	case (int)ACTOR_TYPE::STATIC:		temp =	Get_StaticActors(MyIndex);		break;
	}

	Tr->Position.x = temp.WorldPosition.x;
	Tr->Position.y = temp.WorldPosition.y;
	Tr->Position.z = temp.WorldPosition.z;

	Tr->Q_Rotation.x = temp.Rotation.x;
	Tr->Q_Rotation.y = temp.Rotation.y;
	Tr->Q_Rotation.z = temp.Rotation.z;
	Tr->Q_Rotation.w = temp.Rotation.w;
}

void Rigidbody::SetType(int Type)
{
	typeIndex = Type;
}


