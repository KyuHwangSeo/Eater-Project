#include "GameObject.h"
#include "EngineData.h"
#include "DebugManager.h"
#include "ObjectManager.h"
GameObject::GameObject()
{
	IsDelete = true;	//삭제 여부
	Tag	= 0;			//테그
	IsActive = true;	//모든 컨퍼넌트 사용여부
	Name = "GameObject";
	OneMeshData = new MeshData();
	transform = nullptr;
}

GameObject::~GameObject()
{
	delete OneMeshData;
	OneMeshData = nullptr;
}

void GameObject::SetActive(bool active)
{
	IsActive = active;
}

GameObject* GameObject::GetChild(std::string Name)
{
	return nullptr;
}

GameObject* GameObject::GetChild(int Number)
{
	return nullptr;
}

Transform* GameObject::GetTransform()
{
	if (transform != nullptr)
	{
		return  transform;
	}
	else
	{
		return nullptr;
	}
}

Component* GameObject::GetDeleteComponent(int i)
{
	Component* temp = ComponentList[i];
	return temp;
}

int GameObject::GetComponentCount()
{
	return ComponentList.size();
}

void GameObject::PushChildList(GameObject* obj)
{
	ChildList.push_back(obj);
}

void GameObject::PushComponentFunction(Component* con, unsigned int type)
{
	switch (type)
	{
	case AWAKE:
		ObjectManager::PushAwake(con);
		con->FUNCTION_MASK |= AWAKE;
		break;
	case START:
		ObjectManager::PushStart(con);
		con->FUNCTION_MASK |= START;
		break;
	case START_UPDATE:
		ObjectManager::PushStartUpdate(con);
		con->FUNCTION_MASK |= START_UPDATE;
		break;
	case Transform_UPDATE:
		ObjectManager::PushTransformUpdate(con);
		con->FUNCTION_MASK |= Transform_UPDATE;
		break;
	case Physics_UPDATE:
		ObjectManager::PushPhysicsUpdate(con);
		con->FUNCTION_MASK |= Physics_UPDATE;
		break;
	case UPDATE:
		ObjectManager::PushUpdate(con);
		con->FUNCTION_MASK |= UPDATE;
		break;
	case END_UPDATE:
		ObjectManager::PushEndUpdate(con);
		con->FUNCTION_MASK |= END_UPDATE;
		break;
	}
}
