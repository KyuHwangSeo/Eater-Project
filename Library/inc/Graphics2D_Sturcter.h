#pragma once
#include <string>

// 그려질 텍스트의 내용들.
struct Text_Queue_Data
{
	Text_Queue_Data()
	{
		Position_X = 0.f;
		Position_Y = 0.f;
		Text_Width = 0.f;
		Color_R = 0.f;
		Color_G = 0.f;
		Color_B = 0.f;
		Color_Alpha = 0.f;
		Text_Size = 0.f;
		Text_String = nullptr;
	}
	~Text_Queue_Data()
	{
		delete[] Text_String;
	}


	float Position_X;		// 텍스트 그리기 시작점 X
	float Position_Y;		// 텍스트 그리기 시작점 Y

	float Text_Width;		// 텍스트 박스 크기

	float Color_R;			// Color
	float Color_G;
	float Color_B;
	float Color_Alpha;

	float Text_Size;		// 텍스트 폰트 크기

	wchar_t* Text_String = nullptr;	// 텍스트 내용
};

struct Image_Queue_Data
{
	std::wstring Img_Name = L"";	// 그릴 이미지 이름.

	float Position_X = 0.f;		// 이미지 그리기 시작점 X
	float Position_Y = 0.f;		// 이미지 그리기 시작점 Y

	float Scale_X = 1.f;		// 이미지 스케일 배율 X
	float Scale_Y = 1.f;		// 이미지 스케일 배율 Y

	float Rotate_Angle = 0.f;	// 이미지 회전 각도

	float Alpha = 1.f;			// 이미지 알파값.


	//스프라이트용 이미지에 대한 자료
	int    Index = 0;				//이미지 인덱스
	float  WidthDraw = 0.0f;			//스프라이트 이미지중 한칸의 대한 이미지.
	float  HeightDraw = 0.0f;
};

// 포인터 해제.
template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
}