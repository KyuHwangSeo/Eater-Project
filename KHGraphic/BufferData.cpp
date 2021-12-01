#include "DirectDefine.h"
#include "BufferData.h"

BufferData::BufferData()
	: ResourceBase(eResourceType::BD), VB(nullptr), IB(nullptr), IndexCount(0), Stride(0), Offset(0)
{

}

BufferData::~BufferData()
{
	RESET_COM(VB); 
	RESET_COM(IB);
}

