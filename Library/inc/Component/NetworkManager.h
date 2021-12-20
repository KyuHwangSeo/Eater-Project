#pragma once
/// <summary>
/// ��Ʈ��ũ�� ����ϴ� ��ü���� �������ִ� �Ŵ���
/// </summary>

#include "Component.h"
#include "EaterEngineDLL.h"

class GameObject;
class UnitNet;
class NetworkManager : public Component
{
public: 
	EATER_ENGINEDLL NetworkManager();
	 ~NetworkManager();
	virtual void Awake();
	virtual void Start();
	virtual void SetUp();

	virtual void Update();

	///�ʱ�ȭ
	EATER_ENGINEDLL void Initialize();
	
	///�÷��̾� ��ü�� ��� �Ѵ�
	EATER_ENGINEDLL void AddPlayer(GameObject* mClient01, GameObject* mClient02);
	EATER_ENGINEDLL void SetMovePos(Vector3 Pos);

	///�÷��̾ ���󿬰����� �ֱ������� Ȯ��
	void C2S_KEEP_ALIVE_CHECK_REQ();	//������
	void C2S_KEEP_ALIVE_CHECK_RES();	//�ޱ�

	///������ ���۵ɶ� �ѹ��� ���޾ƾ��ϴ� ������
	void S2C_START_GAME();

	///�÷��̾� ������ ���� �޼���
	void C2S_PLAYER_MOVE_REQ(Vector3 Pos, Vector3 Direction, float Speed);	//������
	void C2S_PLAYER_MOVE_RES();	//�ޱ�
private:
	UnitNet* Client01;
	UnitNet* Client02;

	GameObject* Obj_Client01;
	GameObject* Obj_Client02;
};

