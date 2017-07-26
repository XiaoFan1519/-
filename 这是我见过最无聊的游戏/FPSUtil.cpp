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
	// 暂时不知道如何获取微妙，只能+0.001
	deltaTime = endTime - startTime + 0.001;
	// 计算fps
	fps = 1000 / deltaTime;
}

float FPSUtil::getFPS () {
	return fps;
}

float FPSUtil::getDeltaTime () {
	return deltaTime;
}

void fps (ID2D1HwndRenderTarget* m_pRenderTarget, IDWriteTextFormat* m_pTextFormat)
{
	float fps = FPSUtil::getFPS ();

	std::wstringstream ss;
	ss << "FPS:";
	ss << fps;
	std::wstring costStr;
	ss >> costStr;

	D2D1_RECT_F textLayoutRect = D2D1::RectF (
		static_cast<FLOAT>(800.f),
		static_cast<FLOAT>(0.f),
		static_cast<FLOAT>(960.f),
		static_cast<FLOAT>(16.f)
	);

	ID2D1SolidColorBrush* brush = nullptr;
	m_pRenderTarget->CreateSolidColorBrush (
		D2D1::ColorF (255, 140, 0),
		&brush);

	m_pRenderTarget->DrawText (costStr.c_str (), costStr.size (),
		m_pTextFormat,
		&textLayoutRect,
		brush);

	SafeRelease (&brush);
}