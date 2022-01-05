#include "DirectDefine.h"
#include "Buffer.h"
#include "GraphicView.h"
#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(Buffer* buffer, ShaderResourceView* srv, UnorderedAccessView* uav)
	:ResourceBase(RESOURCE_TYPE::RB), m_Buffer(buffer), m_SRV(srv), m_UAV(uav)
{

}

RenderBuffer::~RenderBuffer()
{
	Release();
}

void RenderBuffer::Reset()
{
	m_Buffer->Reset();
	m_SRV->Reset();
	m_UAV->Reset();
}

void RenderBuffer::Release()
{
	RELEASE_COM(m_Buffer);
	RELEASE_COM(m_SRV);
	RELEASE_COM(m_UAV);
}

void RenderBuffer::OnResize(UINT byteWidth)
{
	m_Buffer->OnResize(byteWidth);
}

void RenderBuffer::SetResize()
{
	m_Buffer->SetResize();
}

Buffer* RenderBuffer::GetBuffer()
{
	return m_Buffer;
}

ShaderResourceView* RenderBuffer::GetSRV()
{
	return m_SRV;
}

UnorderedAccessView* RenderBuffer::GetUAV()
{
	return m_UAV;
}
