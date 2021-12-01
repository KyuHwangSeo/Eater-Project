#pragma once

/// <summary>
/// �������� Ÿ���� �Ѱ��� �Լ��� ��ȯ�ϱ� ���� ���� Class
/// �ش� Class�� Type�� �ݿ��Ͽ� ���� Class Operator�� ���� �������� Ÿ������ ��ȯ
/// </summary>

interface IGraphicResourceManager;

class RenderTarget;
class BasicRenderTarget;
class ComputeRenderTarget;

class OriginalRenderTarget
{
public:
	IGraphicResourceManager* pThis;
	size_t hash_code;

public:
	operator BasicRenderTarget* ();
	operator ComputeRenderTarget* ();
};