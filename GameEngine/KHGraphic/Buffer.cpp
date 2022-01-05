#include "DirectDefine.h"
#include "Buffer.h"

Buffer::Buffer(ID3D11Buffer* buffer)
	:m_Buffer(buffer), m_IsResize(false)
{

}

Buffer::~Buffer()
{
	Release();
}

void Buffer::OnResize(UINT byteWidth)
{
	m_ByteWidth = byteWidth;
}

void Buffer::SetResize()
{
	m_IsResize = true;
}

void Buffer::Reset()
{
	m_Buffer.Reset();
}

void Buffer::Release()
{
	RELEASE_COM(m_Buffer);
}

ID3D11Buffer* Buffer::Get()
{
	return m_Buffer.Get();
}

ID3D11Buffer** Buffer::GetAddress()
{
	return m_Buffer.GetAddressOf();
}

ID3D11Buffer** Buffer::ReleaseGetAddress()
{
	return m_Buffer.ReleaseAndGetAddressOf();
}

void Buffer::GetDesc(D3D11_BUFFER_DESC* desc)
{
	assert(m_Buffer);

	m_Buffer->GetDesc(desc);

	desc->ByteWidth = m_ByteWidth;
}

bool Buffer::GetResize()
{
	return m_IsResize;
}
