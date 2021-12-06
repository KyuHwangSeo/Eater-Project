#pragma once

#include "Grahpics2D_Define.h"
#include "Graphics2D_Sturcter.h"
#include "Non_ThreadObjectPool.h"





EXPIMP_TEMPLATE template class Graphics2D_DLL std::queue<Text_Queue_Data*>;
EXPIMP_TEMPLATE template class Graphics2D_DLL std::queue<Image_Queue_Data*>;
EXPIMP_TEMPLATE template class Graphics2D_DLL std::queue<Image_Queue_Data*>;
EXPIMP_TEMPLATE template class Graphics2D_DLL std::map<std::wstring, ID2D1Bitmap*>;



class Graphics2D_DLL Grahpics2D
{
public:
	Grahpics2D();
	~Grahpics2D();

public:

	IDXGISwapChain* m_3D_SwapChain = nullptr;
private:
	// 메인 핸들
	HWND g_hWnd;
	// DX11에서 생성된 스왑체인.
	// 백버퍼
	IDXGISurface* m_3D_BackBuffer = nullptr;
	// Direct2D Factory
	ID2D1Factory* m_2D_Factory = nullptr;
	// 3D로 부터 생성된 2D RenderTarget
	ID2D1RenderTarget* m_2D_RenderTarget;
	// 커스텀 폰드 로드를 위한 팩토리
	IDWriteFactory5* m_WriteFactory = nullptr;
	// 커스텀 폰트 정보.
	IDWriteFontCollection1* m_FontCollection = nullptr;
	// 텍스트 포멧.
	IDWriteTextFormat* m_TextFormat = nullptr;

	/// 불필요한 Release 방지..
	// 텍스트 레이아웃
	IDWriteTextLayout* m_TextLayOut = nullptr;
	// 텍스트 칼러
	ID2D1SolidColorBrush* m_TextColor = nullptr;

	/// 이미지 로딩
	// 이미지 팩토리.
	IWICImagingFactory* pWICFactory = nullptr;
	// 포맷 변환.
	IWICFormatConverter* pConvertedSrcBmp = nullptr;
	// 디코더 생성.
	IWICBitmapDecoder* pDecodePtr = nullptr;
	// 디코더의 프레임 획득을 위함.
	IWICBitmapFrameDecode* pFramePtr = nullptr;


	/// 폰트 셋팅 관련.

	float m_Font_Size = 24.f;

	/// 그려야할 텍스트를 미리 넣어둔다. ( 오브젝트 풀 생성 )

	Non_ThreadObjectPool<Text_Queue_Data> Text_Data_Pool;
	Non_ThreadObjectPool<Image_Queue_Data> Image_Data_Pool;
	Non_ThreadObjectPool<Image_Queue_Data> Sprite_Data_Pool;

	///	이미지가 그려진후에 텍스쳐가 그려짐
	std::queue<Text_Queue_Data*> Msg_Queue;		// 텍스트 데이터 큐
	std::queue<Image_Queue_Data*> Image_Queue;	// 이미지 데이터 큐	
	std::queue<Image_Queue_Data*> Sprite_Queue;	// 스프라이트 이미지 데이터 큐	
	std::map<std::wstring, ID2D1Bitmap*> Image_Resource;
public:
	void initialize(HWND g_hWnd, IDXGISwapChain* _3D_SwapChain, float _Font_Size = 24.f);

	void LoadBitMap(std::wstring _Image_Name, std::wstring _File_Path);

	void thDrawBitmap(std::wstring _Image_Name, POINTF _Position, float _Alpha, float _Scale_X = 1.f, float _Scale_Y = 1.f, float _Angle = 0.f);
	//스프라이트용 임지ㅣ 그리기
	void thDrawSprite(std::wstring _Image_Name, POINTF _Position, float _Alpha, float _Scale_X, float _Scale_Y, int _Angle,
		int _Accross, int _maxindex, float _StartOneSpace, float _EndOneSpace);

	void Push_DrawImage(std::wstring _Image_Name, POINTF _Pos, POINTF _Scale, float _Angle, float _Alpha,
		POINTF UI_Pos, POINTF UI_Scale, float UI_Angle, float UI_Alpha);

	void Push_DrawSprite(std::wstring _Image_Name,
		POINTF _Pos, POINTF _Scale, float _Angle, float _Alpha,
		POINTF UI_Pos, POINTF UI_Scale, float UI_Angle, float UI_Alpha,
		int IMG_Index, float _StartOneSpace, float _EndOneSpace);
	/// <summary>
	/// 텍스트 출력 함수 - 가변인자를 받을 수 있다.색깔도 지정가능.
	/// </summary>
	/// <param name="_Pos"> 출력할 좌표 (시작 지점) </param>
	/// <param name="_Width"> 글씨 폭 </param>
	/// <param name="r"> Color 의 R </param>
	/// <param name="g"> Color 의 G </param>
	/// <param name="b"> Color 의 B </param>
	/// <param name="a"> Color 의 Alpha </param>
	/// <param name="_Size"> 폰트 사이즈 </param>
	/// <param name="_Input_String"> 입력 스트링 </param>
	/// <param name=""> 가변인자 </param>
	void Push_DrawText(POINT _Pos, float _Width,
		float r, float g, float b, float a,
		float _Size, const wchar_t* _Input_String, ...);

	void Draw_AllText();
	void Draw_AllImage();
	void Draw_AllSprite();
};

