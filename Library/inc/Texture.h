#pragma once
#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>
/// <summary>
/// 텍스쳐 컴포넌트이다.
/// 3D매핑용, 보여지기위한 2D용의 이미지를 나타낼 정보클래스이다.
/// 우리가 쓰일 이미지의 정보만 들고있는 인터페이스이다.
/// </summary>

#include "Component.h"
class D2DSupport;


class  Texture : public Component
{
private:
	D2DSupport* m_pD2DSupport;
	ATL::CString m_str;
public:
	 Texture();
	 Texture(D2DSupport* _pD2DSupport = nullptr);
	 Texture(ATL::CString _str, D2DSupport* _pD2DSupport = nullptr );
	 virtual ~Texture();

public:
	void LoadTexture(ATL::CString _str);

public:

	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update()override;
};

