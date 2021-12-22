#include "Scene.h"

Scene::Scene()
{
	
}
Scene::~Scene()
{

}

void Scene::AddFunction(std::function<void()> mAwake, std::function<void()> mStart, std::function<void()> mUpdate, std::function<void()> mEnd)
{
	AwakeFunction = mAwake;
	StartFunction = mStart;
	UpdateFunction = mUpdate;
	EndFunction = mEnd;
}
