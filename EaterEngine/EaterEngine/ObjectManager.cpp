#include "Component.h"
#include "GameObject.h"
#include "DebugManager.h"
#include "ObjectManager.h"
#include "LoadManager.h"
#include "Camera.h"
#include "EngineData.h"
#include "KeyinputManager.h"
#include "MeshFilter.h"

//함수포인터 리스트들
Delegate_Map<Component> ObjectManager::AwakeFunction;
Delegate_Map<Component> ObjectManager::StartFunction;
Delegate_Map<Component> ObjectManager::StartUpdate;
Delegate_Map<Component> ObjectManager::TransformUpdate;
Delegate_Map<Component> ObjectManager::PhysicsUpdate;
Delegate_Map<Component> ObjectManager::Update;
Delegate_Map<Component> ObjectManager::EndUpdate;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::PushCreateObject(GameObject* obj)
{
	//오브젝트를 넣어줄때 빈곳이 있는지부터 확인
	std::vector<GameObject*>::iterator it = ObjectList.begin();
	for (it; it != ObjectList.end(); it++)
	{
		if ( (*it) == nullptr)
		{
			//빈곳을 찾았다면 빈곳에 넣어주고 함수 종료
			(*it) = obj;
			return;
		}
	}

	//빈곳이없다면 그냥 넣어줌
	ObjectList.push_back(obj);
}

void ObjectManager::PushDeleteObject(GameObject* obj)
{
	//오브젝트를 넣어줄때 빈곳이 있는지부터 확인
	std::vector<GameObject*>::iterator it = ObjectList.begin();
	for (it; it != ObjectList.end(); it++)
	{
		//삭제할 오브젝트를 찾았다면 그오브젝트를 일단 리스트에서 뺀다
		if ((*it) == obj)
		{
			GameObject* temp = (*it);
			(*it) = nullptr;

			//삭제할 리스트에 넣어준다
			DeleteList.push(temp);
		}
	}
}

void ObjectManager::AllDeleteObject()
{
	int count = ObjectList.size();
	for (int i = 0; i < count; i++)
	{
		int comSize =  ObjectList[i]->GetComponentCount();
		for (int j = 0; j < comSize; j++)
		{
			DeleteComponent(ObjectList[i]->GetDeleteComponent(j));
		}
	}


	ObjectList.clear();
	//DHRenderData.clear();
}

void ObjectManager::Initialize(HWND _g_hWnd)
{
}

void ObjectManager::PushStartUpdate(Component* mComponent)
{
	//컨퍼넌트들의 업데이트 함수만 모아놓은 리스트에 들어온 컨퍼넌트 업데이트 함수를 넣어줌
	StartUpdate.Push(mComponent, std::bind(&Component::StartUpdate, mComponent));
}

void ObjectManager::PushTransformUpdate(Component* obj)
{
	TransformUpdate.Push(obj, std::bind(&Component::TransformUpdate, obj));
}

void ObjectManager::PushPhysicsUpdate(Component* obj)
{
	PhysicsUpdate.Push(obj, std::bind(&Component::PhysicsUpdate, obj));
}

void ObjectManager::PushUpdate(Component* mComponent)
{
	Update.Push(mComponent, std::bind(&Component::Update, mComponent));
}

void ObjectManager::PushEndUpdate(Component* mComponent)
{
	//가장 마지막에 실행되는 업데이트 함수리스트에 넣는다
	EndUpdate.Push(mComponent, std::bind(&Component::EndUpdate, mComponent));
}

void ObjectManager::PushStart(Component* mComponent)
{
	StartFunction.Push(mComponent, std::bind(&Component::Start, mComponent));
}

void ObjectManager::PushAwake(Component* mComponent)
{
	AwakeFunction.Push(mComponent, std::bind(&Component::Awake, mComponent));
}

void ObjectManager::PlayUpdate()
{
	///가장 먼저실행되는 StartUpdate 함수 리스트(각 컨퍼넌트들의 초기화작업을 해줄때)
	StartUpdate.Play();

	///이동행렬 실행되는 Update 함수 리스트
	TransformUpdate.Play();

	///물리 충돌관련 Update 함수 리스트 (물리관련 컨퍼넌트들을 업데이트)
	PhysicsUpdate.Play();

	///중간 단계에 실행되는 Update 함수 리스트 (클라이언트쪽에서 만든 컨퍼넌트들이 업데이트될곳)
	Update.Play();

	///가장 마지막에 실행되는 Update 함수 리스트
	EndUpdate.Play();

	///업데이트 작업끝 그래픽엔진으로 넘겨줄 데이터 정리



	//글로벌 데이터
	Global->mProj = Camera::GetProj();
	Global->mViewMX = Camera::GetMainView();
	Global->mPos = Camera::GetMainPos();

	///모든오브젝트의 데이터를 랜더큐에 담는다
	CreateRenderQueue();

	///모든 오브젝트 업데이트 완료
}

void ObjectManager::PlayStart()
{
	AwakeFunction.Play();
	StartFunction.Play();

	//Test();
}

void ObjectManager::CreateRenderQueue()
{
	//오브젝트의 사이즈 만큼 돌면서 랜더큐에 MeshData를 전달해준다
	for (int i = 0; i < ShadowData.size(); i++)
	{
		ShadowData.pop();
	}
	

	int count = ObjectList.size();
	
	std::vector<GameObject*>::iterator it = ObjectList.begin();
	for (it; it != ObjectList.end(); it++)
	{
		RenderData.push((*it)->OneMeshData);
		ShadowData.push((*it)->OneMeshData);
	}
}

void ObjectManager::ClearFunctionList()
{
	AwakeFunction.Clear();
	StartFunction.Clear();

	StartUpdate.Clear();
	TransformUpdate.Clear();
	PhysicsUpdate.Clear();
	Update.Clear();
	EndUpdate.Clear();
}

void ObjectManager::DeleteObject()
{
	//삭제할 오브젝트 만큼 반복문을 돈다
	for (unsigned i = 0; i < DeleteList.size(); i++)
	{
		//큐에서 가장먼저들어온 오브젝트를 꺼낸다
		GameObject* temp = DeleteList.front();
		int count = temp->GetComponentCount();
		for (int j = 0; j < count; j++)
		{
			Component* cpt = temp->GetDeleteComponent(j);
			//컨퍼넌트 삭제 함수포인터 리스트에서 그컨퍼넌트의 함수포인터를찾아서 삭제
			DeleteComponent(cpt);
		}

		//삭제 했으니 삭제할 리스트에서도 빼준다
		DeleteList.pop();
	}
}

std::queue<MeshData*>* ObjectManager::GetRenderQueue()
{
	return &RenderData;
}

std::queue<MeshData*>* ObjectManager::GetShadowQueue()
{
	return &ShadowData;
}


void ObjectManager::DeleteComponent(Component* cpt)
{
	//업데이트 함수포인터에 넣었던 것들 삭제
	if (cpt->FUNCTION_MASK & AWAKE)
	{
		AwakeFunction.Pop(cpt);
	}

	if (cpt->FUNCTION_MASK & START)
	{
		StartFunction.Pop(cpt);
	}

	if (cpt->FUNCTION_MASK & START_UPDATE)
	{
		StartUpdate.Pop(cpt);
	}

	if (cpt->FUNCTION_MASK & Transform_UPDATE)
	{
		TransformUpdate.Pop(cpt);
	}

	if (cpt->FUNCTION_MASK & Physics_UPDATE)
	{
		PhysicsUpdate.Pop(cpt);
	}

	if (cpt->FUNCTION_MASK & UPDATE)
	{
		Update.Pop(cpt);
	}

	if (cpt->FUNCTION_MASK & END_UPDATE)
	{
		EndUpdate.Pop(cpt);
	}


	///진짜 컨퍼넌트 삭제
	delete cpt;
	cpt = nullptr;
}

