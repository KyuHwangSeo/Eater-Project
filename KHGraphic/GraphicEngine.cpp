#include "GraphicsEngine.h"
#include "KHGraphic.h"
#include "../HsGraphic/HsGraphic.h"

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
