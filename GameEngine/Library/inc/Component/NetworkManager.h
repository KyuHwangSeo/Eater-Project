#pragma once
/// <summary>
/// 네트워크를 통신하는 객체들을 관리해주는 매니저
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

	///초기화
	EATER_ENGINEDLL void Initialize();
	
	///플레이어 객체를 등록 한다
	EATER_ENGINEDLL void AddPlayer(GameObject* mClient01, GameObject* mClient02);
	EATER_ENGINEDLL void SetMovePos(Vector3 Pos);

	///플레이어가 정상연결인지 주기적으로 확인
	void C2S_KEEP_ALIVE_CHECK_REQ();	//보내기
	void C2S_KEEP_ALIVE_CHECK_RES();	//받기

	///게임이 시작될때 한번은 꼭받아야하는 데이터
	void S2C_START_GAME();

	///플레이어 움직인 관련 메세지
	void C2S_PLAYER_MOVE_REQ(Vector3 Pos, Vector3 Direction, float Speed);	//보내기
	void C2S_PLAYER_MOVE_RES();	//받기
private:
	UnitNet* Client01;
	UnitNet* Client02;

	GameObject* Obj_Client01;
	GameObject* Obj_Client02;
};

