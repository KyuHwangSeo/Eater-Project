#include "Component.h"
#include "GameObject.h"
#include "DebugManager.h"
#include "ObjectManager.h"
#include "LoadManager.h"
#include "Camera.h"
#include "EngineData.h"
#include "KeyinputManager.h"
#include "MeshFilter.h"
#include "Light.h"

//함수포인터 리스트들
Delegate_Map ObjectManager::AwakeFunction;
Delegate_Map ObjectManager::StartFunction;
Delegate_Map ObjectManager::StartUpdate;
Delegate_Map ObjectManager::TransformUpdate;
Delegate_Map ObjectManager::PhysicsUpdate;
Delegate_Map ObjectManager::Update;
Delegate_Map ObjectManager::EndUpdate;

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::PushCreateObject(GameObject* obj)
{
	//오브젝트를 넣어줄때 빈곳이 있는지부터 확인
	//std::vector<GameObject*>::iterator it = ObjectList.begin();
	//for (it; it != ObjectList.end(); it++)
	//{
	//	if ( (*it) == nullptr)
	//	{
	//		//빈곳을 찾았다면 빈곳에 넣어주고 함수 종료
	//		(*it) = obj;
	//		return;
	//	}
	//}

	//빈곳이없다면 그냥 넣어줌
	ObjectList.push_back(obj);
}

void ObjectManager::PushDontDeleteObject(GameObject* obj)
{
	std::vector<GameObject*>::iterator it = ObjectList.begin();
	for (it; it != ObjectList.end(); it++)
	{
		//오브젝트를 찾았다면 옮겨준다
		if ((*it) == obj)
		{
			GameObject* temp = (*it);
			(*it) = nullptr;

			DontDeleteObjectList.push_back(temp);
			return;
		}
	}

	//오브젝트 리스트에서 찾지못했다면 그냥 넣어줌
	DontDeleteObjectList.push_back(obj);
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
	int count = (int)ObjectList.size();
	for (int i = 0; i < count; i++)
	{
		int comSize =  ObjectList[i]->GetComponentCount();
		for (int j = 0; j < comSize; j++)
		{
			DeleteComponent(ObjectList[i]->GetDeleteComponent(j));
		}
	}
	
	ObjectList.clear();
}

void ObjectManager::Initialize(HWND _g_hWnd)
{

}

void ObjectManager::PushStartUpdate(Component* mComponent)
{
	//컨퍼넌트들의 업데이트 함수만 모아놓은 리스트에 들어온 컨퍼넌트 업데이트 함수를 넣어줌

	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::StartUpdate, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPoiner = mComponent;

	StartUpdate.Push(data);
}

void ObjectManager::PushTransformUpdate(Component* mComponent)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::TransformUpdate, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPoiner = mComponent;

	TransformUpdate.Push(data);
}

void ObjectManager::PushPhysicsUpdate(Component* mComponent)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::PhysicsUpdate, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPoiner = mComponent;

	PhysicsUpdate.Push(data);
}

void ObjectManager::PushUpdate(Component* mComponent)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::Update, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPoiner = mComponent;


	Update.Push(data);
}

void ObjectManager::PushEndUpdate(Component* mComponent)
{
	//가장 마지막에 실행되는 업데이트 함수리스트에 넣는다
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::EndUpdate, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPoiner = mComponent;

	EndUpdate.Push(data);
}

void ObjectManager::PushStart(Component* mComponent)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::Start, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPoiner = mComponent;

	StartFunction.Push(data);
}

void ObjectManager::PushAwake(Component* mComponent)
{
	ComponentFunctionData data;
	//활성화 여부
	data.Enabled = &mComponent->Enabled;
	//함수 포인터
	data.FunctionPointer = std::bind(&Component::Awake, mComponent);
	//컨퍼넌트 포인터
	data.ComponentPoiner = mComponent;

	AwakeFunction.Push(data);
}

void ObjectManager::PlayUpdate()
{
	//한번만 실행되는 함수포인터 리스트
	AwakeFunction.Play(true);
	StartFunction.Play(true);

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





	//글로벌 데이터
	Global->mProj	= Camera::GetProj();
	Global->mViewMX = Camera::GetMainView();

	//라이트 데이터
	Global->mLightViewMX	= DirectionLight::g_DirLight->GetView();
	Global->mLightProj		= DirectionLight::g_DirLight->GetProj();
	Global->mShadowTrans	= DirectionLight::g_DirLight->GetShadowTranspose();

	///모든오브젝트의 데이터를 랜더큐에 담는다
	CreateRenderQueue();

	///모든 오브젝트 업데이트 완료
}

void ObjectManager::CreateRenderQueue()
{
	//오브젝트의 사이즈 만큼 돌면서 랜더큐에 MeshData를 전달해준다
	for (int i = 0; i < ShadowData.size(); i++)
	{
		ShadowData.pop();
	}
	
	int count = (int)ObjectList.size();
	
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
		//게임오브젝트도 삭제
		delete temp;
		temp = nullptr;

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

