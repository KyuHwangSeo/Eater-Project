#pragma once

#include <functional>
#include <string>
#include <windows.h>
#include "EaterEngineDLL.h"
#include "TypeOptionHeader.h"

namespace flatbuffers 
{
	class FlatBufferBuilder;
}

class GameObject;
class Scene;
class PhysRayCast;
class NetworkManagerComponent;

extern "C" EATER_ENGINEDLL void EngineInitialize(HWND _g_hWnd);		//GameEngine ����
extern "C" EATER_ENGINEDLL void EndEngine();						//GameEngine ����
extern "C" EATER_ENGINEDLL void UpdateEngine();						//GameEngine ������Ʈ
extern "C" EATER_ENGINEDLL void OnReSize(int X,int Y);				//������ ������ ������	

///���� ������Ʈ ����
extern "C" EATER_ENGINEDLL GameObject*	Instance(std::string ObjName= "GameObject");		//������Ʈ ����
extern "C" EATER_ENGINEDLL GameObject*	InstanceTerrain(std::string ObjName= "GameObject");	//������Ʈ ����
extern "C" EATER_ENGINEDLL GameObject*	InstanceParticle(std::string ObjName= "GameObject");	//������Ʈ ����
extern "C" EATER_ENGINEDLL void			Destroy(GameObject* obj);							//������Ʈ ����

///��ũ�� �����ϱ�
template<typename T>
void CreateScene(std::string Name);
extern "C" EATER_ENGINEDLL void ChoiceScene(std::string name);					//��ũ�� ����
EATER_ENGINEDLL void CreateSceneSub(Scene* mSceneTemp,std::string SceneName);	//��ũ�� ����


///�Ž� �ҷ�����
extern "C" EATER_ENGINEDLL void LoadMesh(std::string mMeshName, UINT parsingMode = 0);	//�Ž� �ε�
extern "C" EATER_ENGINEDLL void LoadTerrainMesh(std::string mMeshName, std::string maskName, UINT parsingMode = 0);	//�Ž� �ε�
extern "C" EATER_ENGINEDLL void LoadMeshPath(std::string mPath);											//�Ž� �ε� ���
extern "C" EATER_ENGINEDLL void LoadTexture(std::string mPath);
extern "C" EATER_ENGINEDLL void LoadTexturePath(std::string mPath);


///Ű�Է�
extern "C" EATER_ENGINEDLL bool  GetKeyDown(byte number);	//Ű�ٿ�
extern "C" EATER_ENGINEDLL bool  GetKeyUp(byte number);		//Ű��
extern "C" EATER_ENGINEDLL bool  GetKey(byte number);		//Ű������������
extern "C" EATER_ENGINEDLL bool  GetTogle(byte number);		//Ű ���
extern "C" EATER_ENGINEDLL float GetMousePosX();			//���콺 ��ġ X
extern "C" EATER_ENGINEDLL float GetMousePosY();			//���콺 ��ġ Y

///ī�޶� ����
//EATER_ENGINEDLL Vector3 GetCameraLocalPosition_UP();
//EATER_ENGINEDLL Vector3 GetCameraLocalPosition_Right();
//EATER_ENGINEDLL Vector3 GetCameraLocalPosition_Look();

///�ð� ����
extern "C" EATER_ENGINEDLL float GetDeltaTime();

///���� ����
extern "C" EATER_ENGINEDLL bool RayCast(PhysRayCast* ray);

///��Ʈ��ũ ����
//�����ʿ��� ����� ��絥���͸� ���⿡�� ���� �Ŵ����� Ŭ��� ����Ѵ�
extern "C" EATER_ENGINEDLL void Network_SetManager(NetworkManagerComponent * Manager);
//Ŭ���̾�Ʈ ���� �޼����� ������ �̰��� ���
extern "C" EATER_ENGINEDLL void Network_Send(flatbuffers::FlatBufferBuilder * Builder,int Type);
//�ε��� �Ϸ�Ǿ����� ������ �޼���
extern "C" EATER_ENGINEDLL void Network_Loading_Send();


template<typename T>
inline void CreateScene(std::string Name)
{
	T* temp = new T();
	CreateSceneSub(temp, Name);
}