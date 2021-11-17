#pragma once

#include <map>
#include <vector>
#include <queue>
#include <windows.h>
#include "EngineData.h"
#include <functional>
#include "Delegate.h"
#include "EaterEngineDLL.h"
#include "BaseManager.h"


class Camera;
class FBXModel;
class GameObject;
class Component;
class MeshFilter;

class ObjectManager : public BaseManager
{
public:
	~ObjectManager();
	ObjectManager();

	//생성한 오브젝트를 넣어줌
	void PushCreateObject(GameObject* obj);

	//삭제할 오브젝트를 넣어줌(이함수를 실행시킬단계에서 오브젝트를 삭제하지않음 삭제는 가장 마지막에)
	void PushDeleteObject(GameObject* obj);

	//현재 오브젝트 리스트에 담겨있는 오브젝트와 컨퍼넌트 리스트를 전부 삭제
	void AllDeleteObject();

	//엔진 초기화
	void Initialize(HWND _g_hWnd);

	/// 시작 단계
	static void PushStart(Component* obj);
	static void PushAwake(Component* obj);

	/// 컨퍼넌트의 업데이트 함수를 넣어준다
	static void PushStartUpdate(Component* obj);
	static void PushTransformUpdate(Component* obj);
	static void PushPhysicsUpdate(Component* obj);
	static void PushEndUpdate(Component* obj);
	static void PushUpdate(Component* obj);

	/// 업데이트 함수 리스트를 실행시킴
	void PlayUpdate();
	void PlayStart();

	///그래픽 엔진쪽으로 보낼 랜더메세지큐를 생성
	void CreateRenderQueue();

	/// 업데이트와 랜더링 함수리스트를 모두 삭제함
	void ClearFunctionList();

	///오브젝트를 삭제한다
	void DeleteObject();

	std::queue<MeshData*>* GetRenderQueue();
	std::queue<MeshData*>* GetShadowQueue();
	
private:
	std::vector<GameObject*> ObjectList;
	std::vector<GameObject*> DontDeleteList;
	
	std::queue<GameObject*> DeleteList;

	///그래픽엔진쪽으로 던저줄 데이터들
	std::queue<MeshData*> RenderData;
	std::queue<MeshData*> ShadowData;
	std::queue<MeshData*> UIData;


	/// 시작단계 한번만 실행됨
	static Delegate_Map<Component> AwakeFunction;		//시작단계 보다 먼저 실행되는 함수
	static Delegate_Map<Component> StartFunction;		//시작단계의 실행되는 함수


	/// 업데이트 단계 프레임마다 실행
	static Delegate_Map<Component> StartUpdate;			//가장먼저 시작되는 업데이트
	static Delegate_Map<Component> TransformUpdate;		//이동 행렬 업데이트
	static Delegate_Map<Component> PhysicsUpdate;		//물리 행동 업데이트
	static Delegate_Map<Component> Update;				//디폴트  중간단계의 시작되는 업데이트
	static Delegate_Map<Component> EndUpdate;			//가장 마지막에 실행되는 업데이트


	void DeleteComponent(Component* cpt);
};