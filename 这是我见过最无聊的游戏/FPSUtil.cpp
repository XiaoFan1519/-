#include "FPSUtil.h"

FPSUtil::FPSUtil ()
{
}

FPSUtil::~FPSUtil ()
{
}

ULONGLONG FPSUtil::startTime = GetTickCount64 ();
float FPSUtil::fps = 0;
float FPSUtil::deltaTime = 0;

void FPSUtil::setStartTime () {
	startTime = GetTickCount64 ();
}

void FPSUtil::setEndTime () {
	ULONGLONG endTime = GetTickCount64 ();
	float tmp = endTime - startTime;
	float sleepTime = 1000 / 60 - tmp;
	if (sleepTime > 0) {
		Sleep ((DWORD)sleepTime);
	}

	endTime = GetTickCount64 ();
	tmp = endTime - startTime;
	// 计算fps
	fps = 1000 / tmp;
	// 计算每帧消耗的秒数
	deltaTime = tmp / 1000;
}

float FPSUtil::getFPS () {
	return fps;
}

float FPSUtil::getDeltaTime () {
	return deltaTime;
}

void fps (ID2D1HwndRenderTarget* m_pRenderTarget, IDWriteTextFormat* m_pTextFormat)
{
	static float count;
	
	count += FPSUtil::getDeltaTime ();
	if (count < 1) {
		return;
	}
	count = 0;

	float fps = FPSUtil::getFPS ();

	std::wstringstream ss;
	ss << "FPS:";
	ss << fps;
	std::wstring costStr;
	ss >> costStr;

	D2D1_RECT_F clearRect = D2D1::RectF (
		static_cast<FLOAT>(800.f),
		static_cast<FLOAT>(0.f),
		static_cast<FLOAT>(960.f),
		static_cast<FLOAT>(32.f)
	);

	ID2D1SolidColorBrush* brush = nullptr;
	m_pRenderTarget->CreateSolidColorBrush (
		D2D1::ColorF (D2D1::ColorF::Black),
		&brush);
	// Clear
	m_pRenderTarget->FillRectangle (clearRect, brush);
	SafeRelease (&brush);

	D2D1_RECT_F textLayoutRect = D2D1::RectF (
		static_cast<FLOAT>(800.f),
		static_cast<FLOAT>(0.f),
		static_cast<FLOAT>(960.f),
		static_cast<FLOAT>(0.f)
	);
	// 用于文字的画刷
	m_pRenderTarget->CreateSolidColorBrush (
			D2D1::ColorF (255, 140, 0),
			&brush);
	m_pRenderTarget->DrawText (costStr.c_str (), costStr.size (),
		m_pTextFormat,
		&textLayoutRect,
		brush);

	SafeRelease (&brush);
}