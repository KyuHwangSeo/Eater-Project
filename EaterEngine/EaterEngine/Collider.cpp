#include "Collider.h"

Collider::Collider()
{
	Center = { 0,0,0 };
	Size = { 1,1,1 };
	isTrigger = false;
}

Collider::~Collider()
{

}

void Collider::SetTrigger(bool trigger)
{
	isTrigger = trigger;
}

void Collider::SetMaterial()
{
	
}

void Collider::SetCenter(float x, float y, float z)
{
	Center.x = x;
	Center.y = y;
	Center.z = z;

}

void Collider::SetSize(float x, float y, float z)
{
	Size.x = x;
	Size.y = x;
	Size.x = x;

}
