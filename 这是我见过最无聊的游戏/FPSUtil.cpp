#include "FPSUtil.h"
#include <chrono>
#include <thread>

FPSUtil::FPSUtil ()
{
}

FPSUtil::~FPSUtil ()
{
}

ULONGLONG FPSUtil::startTime = GetTickCount64();
float FPSUtil::fps = 0;
float FPSUtil::deltaTime = 0;

void FPSUtil::setStartTime () {
	startTime = GetTickCount64();
}

void FPSUtil::setEndTime () {
	auto endTime = GetTickCount64();
	auto cost = endTime - startTime;
	// 计算fps
	fps = 1000.0 / cost;
	// 计算每帧消耗的秒数
	deltaTime = cost / 1000.0;
	startTime = endTime;
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

	std::wstring costStr = L"FPS:";
	costStr += std::to_wstring(fps);

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