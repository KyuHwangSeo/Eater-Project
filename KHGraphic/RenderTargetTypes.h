#pragma once

/// <summary>
/// 여러가지 타입을 한개의 함수로 반환하기 위해 만든 Class
/// 해당 Class의 Type을 반영하여 원본 Class Operator를 통해 여러가지 타입으로 반환
/// </summary>

interface IGraphicResourceManager;

class RenderTarget;
class BasicRenderTarget;
class ComputeRenderTarget;

///
/// 2021/11/07 23:02
/// SeoKyuHwang
///
/// # OriginalRenderTarget Class
///
/// - OriginalRenderTarget Operator를 통해 해당 RenderTarget Class 반환
/// - operator (BasicRenderTarget, ComputeRenderTarget)
class OriginalRenderTarget
{
public:
	IGraphicResourceManager* pThis;
	size_t hash_code;

public:
	operator BasicRenderTarget* ();
	operator ComputeRenderTarget* ();
};