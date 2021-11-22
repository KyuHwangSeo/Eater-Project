#include "KeyInput.h"
#include "MainHeader.h"
#include "Directxmath.h"
#include "GameObject.h"
#include "Transform.h"

Keyinput::Keyinput()
{
	transform = nullptr;
}

Keyinput::~Keyinput()
{

}

void Keyinput::Start()
{
	transform = gameobject->GetComponent<Transform>();
}

void Keyinput::Update()
{
	if (GetKeyDown(VK_F1))
	{
		speed += 10;
	}

	if (GetKeyDown(VK_F2))
	{
		speed -= 10;
	}

	
	float delta = GetDeltaTime();
	if (GetKey('D'))
	{
		transform->SetLocalPosition((speed * delta),0,0);
	}

	if (GetKey('A'))
	{
		transform->SetLocalPosition(-(speed * delta), 0, 0);
	}

	if (GetKey('W'))
	{
		transform->SetLocalPosition(0, 0, (speed * delta));
	}

	if (GetKey('S'))
	{
		transform->SetLocalPosition(0, 0, -(speed * delta));
	}

	if (GetKeyDown(VK_RBUTTON) == true)
	{
		mLastMousePosX = GetMousePosX();
		mLastMousePosY = GetMousePosY();
	}

	if (GetKeyUp(VK_RBUTTON) == true)
	{
		mLastMousePosX = GetMousePosX();
		mLastMousePosY = GetMousePosY();
	}
	
	float RotSpeed = 5;
	float x = GetMousePosX();
	float y = GetMousePosY();

	if (GetKey(VK_RBUTTON) == true)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = DirectX::XMConvertToRadians(0.5f * static_cast<float>(x - mLastMousePosX));
		float dy = DirectX::XMConvertToRadians(0.5f * static_cast<float>(y - mLastMousePosY));
	
		transform->SetRotate(-dy * RotSpeed, -dx * RotSpeed, 0);
	}
	mLastMousePosX = x;
	mLastMousePosY = y;
}
