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

	//������ ������Ʈ�� �־���
	static void PushCreateObject(GameObject* obj);

	//�����Ǹ� �ȵǴ� ������Ʈ ����Ʈ
	void PushDontDeleteObject(GameObject* obj);

	//������ ������Ʈ�� �־���(���Լ��� �����ų�ܰ迡�� ������Ʈ�� ������������ ������ ���� ��������)
	void PushDeleteObject(GameObject* obj);

	//���� ������Ʈ ����Ʈ�� ����ִ� ������Ʈ�� ���۳�Ʈ ����Ʈ�� ���� ����
	void AllDeleteObject();

	//���� �ʱ�ȭ
	void Initialize(HWND _g_hWnd);

	///�ʱ�ȭ �ܰ�
	static void PushStart(Component* obj,int Order);
	static void PushStartPlay(Component* obj, int Order);
	static void PushAwake(Component* obj, int Order);

	/// ���۳�Ʈ�� ������Ʈ �Լ��� �־��ش�
	static void PushStartUpdate(Component* obj, int Order);
	static void PushTransformUpdate(Component* obj, int Order);
	static void PushPhysicsUpdate(Component* obj, int Order);
	static void PushEndUpdate(Component* obj, int Order);
	static void PushUpdate(Component* obj, int Order);

	/// ������Ʈ �Լ� ����Ʈ�� �����Ŵ
	void PlayUpdate();

	///�׷��� ���������� ���� �����޼���ť�� ����
	void CreateRenderQueue();

	/// ������Ʈ�� ������ �Լ�����Ʈ�� ��� ������
	void ClearFunctionList();

	///������Ʈ�� �����Ѵ�
	void DeleteObject();

	std::queue<MeshData*>* GetRenderQueue();
	std::queue<MeshData*>* GetShadowQueue();

private:
	//���۳�Ʈ �����͸� �Ұ��ش�
	static ComponentFunctionData PushComponentData(Component*);

	///������Ʈ ����Ʈ
	static std::vector<GameObject*> ObjectList;
	///�����Ǹ� �ȵǴ� ������Ʈ ����Ʈ
	std::vector<GameObject*> DontDeleteObjectList;
	///���������� ��γ����� �������� ������ ������Ʈ����Ʈ
	std::queue<GameObject*> DeleteList;


	///�׷��ȿ��������� ������ �����͵�
	std::queue<MeshData*> RenderData;
	std::queue<MeshData*> ShadowData;
	std::queue<MeshData*> UIData;

	/// ���۴ܰ� �ѹ��� �����
	static Delegate_Map AwakeFunction;		//���۴ܰ� ���۳�Ʈ �������� �Լ�
	static Delegate_Map StartFunction;		//���۴ܰ��� �ʱ�ȭ �Լ�
	static Delegate_Map SetUpFunction;		//���۴ܰ��� ����Ǵ� �Լ�


	/// ������Ʈ �ܰ� �����Ӹ��� ����
	static Delegate_Map StartUpdate;		//������� ���۵Ǵ� ������Ʈ
	static Delegate_Map TransformUpdate;	//�̵� ��� ������Ʈ
	static Delegate_Map PhysicsUpdate;		//���� �ൿ ������Ʈ
	static Delegate_Map Update;				//����Ʈ  �߰��ܰ��� ���۵Ǵ� ������Ʈ
	static Delegate_Map EndUpdate;			//���� �������� ����Ǵ� ������Ʈ

	//���۳�Ʈ �������� �ش� �Լ������Ϳ� �־��� �����͸� ����������
	void DeleteComponent(Component* cpt);
	
};