#include "GameObject.h"
#include "EngineData.h"
#include "DebugManager.h"
#include "ObjectManager.h"
GameObject::GameObject()
{
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
	std::vector<Component*>::iterator it = ComponentList.begin();
	for (it; it != ComponentList.end(); it++)
	{
		(*it)->Enabled = active;
	}
}

GameObject* GameObject::GetChild(std::string Name)
{
	return nullptr;
}

GameObject* GameObject::GetChild(int Number)
{
	if (Number >= (int)ChildList.size())
	{
		return nullptr;
	}
	return ChildList[Number];
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
	return (int)ComponentList.size();
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
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_PUSH, "ComponentFunction", "1.Awake",false);
		ObjectManager::PushAwake(con);
		con->FUNCTION_MASK |= AWAKE;
		break;
	case START:
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_PUSH, "ComponentFunction", "2.Start", false);
		ObjectManager::PushStart(con);
		con->FUNCTION_MASK |= START;
		break;
	case START_UPDATE:
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_PUSH, "ComponentFunction", "3.StartUpdate", false);
		ObjectManager::PushStartUpdate(con);
		con->FUNCTION_MASK |= START_UPDATE;
		break;
	case Transform_UPDATE:
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_PUSH, "ComponentFunction","4.TransformUpdate", false);
		ObjectManager::PushTransformUpdate(con);
		con->FUNCTION_MASK |= Transform_UPDATE;
		break;
	case Physics_UPDATE:
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_PUSH, "ComponentFunction", "5.PhysicsUpdate", false);
		ObjectManager::PushPhysicsUpdate(con);
		con->FUNCTION_MASK |= Physics_UPDATE;
		break;
	case UPDATE:
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_PUSH, "ComponentFunction", "6,Update", false);
		ObjectManager::PushUpdate(con);
		con->FUNCTION_MASK |= UPDATE;
		break;
	case END_UPDATE:
		DebugManager::Print(DebugManager::MSG_TYPE::MSG_PUSH, "ComponentFunction", "7.EndUpdate", false);
		ObjectManager::PushEndUpdate(con);
		con->FUNCTION_MASK |= END_UPDATE;
		break;
	}
}
