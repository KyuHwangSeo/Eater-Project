#pragma once
class PhysEngine;

class PhysManager
{
public:
	PhysManager();
	~PhysManager();


	void Initialize();	//초기화 
	void Release();		//삭제
	void Update(float m_Time);

private:
	PhysEngine* Phys;
};