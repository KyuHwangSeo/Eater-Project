#pragma once

#include <functional>
#include <string>

/// <summary>
/// 생성은 엔진쪽에서 생성할때 이름과 함수포인터들만 넘겨주고 생성과삭제는 스크린매니저에서 해준다
/// </summary>

#include "EaterEngineDLL.h"


class Scene
{
public:
	EATER_ENGINEDLL Scene();
	EATER_ENGINEDLL virtual ~Scene();

	void AddFunction
	(
		std::function<void()> mAwake,
		std::function<void()> mStart,
		std::function<void()> mUpdate,
		std::function<void()> mEnd
	);


	//씬이 생성될떄 호출
	virtual void Awake()=0;
	virtual void Start()=0;
	//매프레임 호출
	virtual void Update()=0;
	//씬이 종료되거나 다음씬으로 넘어갈떄 실행
	virtual void End()=0;


	std::string Name;
	std::function<void()> AwakeFunction;
	std::function<void()> StartFunction;
	std::function<void()> UpdateFunction;
	std::function<void()> EndFunction;
private:
};


