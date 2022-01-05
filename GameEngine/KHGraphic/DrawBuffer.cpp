#include "DirectDefine.h"
#include "ResourceBase.h"
#include "Buffer.h"
#include "DrawBuffer.h"

DrawBuffer::DrawBuffer(Buffer* vb, Buffer* ib)
	: ResourceBase(RESOURCE_TYPE::DB), VB(vb), IB(ib), IndexCount(0), Stride(0), Offset(0)
{

}

DrawBuffer::~DrawBuffer()
{
	Release();
}

void DrawBuffer::Reset()
{
	VB->Reset();
	IB->Reset();
}

void DrawBuffer::Release()
{
	RELEASE_COM(VB);
	RELEASE_COM(IB);
}

