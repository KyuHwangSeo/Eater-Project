#pragma once

#include "MainHeader.h"
#include "Component.h"

namespace flatbuffers
{
	class FlatBufferBuilder;
}


class NetworkManagerComponent: public Component
{
public:
	EATER_ENGINEDLL  NetworkManagerComponent();
	EATER_ENGINEDLL virtual ~NetworkManagerComponent();

	EATER_ENGINEDLL virtual void RECV(const uint8_t* Data, int type) {};
	
	flatbuffers::FlatBufferBuilder* _Builder;
};