#include "DirectDefine.h"
#include "BufferData.h"

BufferData::BufferData()
	: ResourceBase(eResourceType::BD), VB(nullptr), IB(nullptr), IndexCount(0), Stride(0), Offset(0)
{

}

BufferData::~BufferData()
{
	Release();
}

void BufferData::Reset()
{
	RESET_COM(VB);
	RESET_COM(IB);
}

void BufferData::Release()
{
	RELEASE_COM(VB);
	RELEASE_COM(IB);
}

