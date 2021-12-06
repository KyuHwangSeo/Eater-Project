#include "GraphicsEngine.h"
#include "KHGraphic.h"
#include "./HSEngine/HsGraphic.h"

GraphicEngine* GraphicEngine::Create()
{
	GraphicEngine* graphicEngine = new KHGraphic();

	return graphicEngine;
}

GraphicEngine* GraphicEngine::CreateHS()
{
	GraphicEngine* graphicEngine = new HsGraphic();

	return graphicEngine;
}
