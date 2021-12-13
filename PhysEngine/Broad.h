#pragma once
#include "PxPhysicsAPI.h"
#include <vector>

using namespace physx;

class Broad :public physx::PxBroadPhaseCallback
{
	std::vector<PxActor*> outOfBoundsActors;
public:
	virtual void onObjectOutOfBounds(PxShape& /*shape*/, PxActor& actor)
	{
		PxU32 i = 0;
		for (; i < outOfBoundsActors.size(); ++i)
		{
			if (outOfBoundsActors[i] == &actor)
				break;
		}
		if (i == outOfBoundsActors.size())
		{
			outOfBoundsActors.push_back(&actor);
		}
	}

	virtual void onObjectOutOfBounds(PxAggregate& /*aggregate*/)
	{
		//This test does not use aggregates so no need to do anything here
	}


	void purgeOutOfBoundsObjects()
	{
		for (PxU32 i = 0; i < outOfBoundsActors.size(); ++i)
		{
			outOfBoundsActors[i]->release();
		}
		outOfBoundsActors.clear();
	}
};




