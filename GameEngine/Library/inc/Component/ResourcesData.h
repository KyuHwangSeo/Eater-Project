#pragma once
#include <DirectXMath.h>
#include <string>
//���ҽ� �ֻ��� Ŭ����
class  Resources
{
public:
	Resources() {};
	virtual ~Resources() {};
};

//�ε��� ���۸� ���� Ŭ����
class IndexBuffer : Resources
{
public:
	virtual ~IndexBuffer()
	{
		delete IndexBufferPointer;
	};
	
	void* IndexBufferPointer = nullptr;
	unsigned int Count	= 0;			//�ε��� ����
};


//���ؽ� ���۸� ���� Ŭ����
class VertexBuffer : Resources
{
public:
	virtual ~VertexBuffer()
	{
		delete VertexBufferPointer;
	};

	void* VertexBufferPointer = nullptr;

	unsigned int VectexDataSize = 0;	//���ؽ� 
	unsigned int Count	= 0;			//���ؽ� ����
};

//�ؽ��ĸ� ���� Ŭ����
class TextureBuffer : Resources
{
public:
	virtual ~TextureBuffer()
	{
		delete TextureBufferPointer;
	};

	void* TextureBufferPointer = nullptr;
};







