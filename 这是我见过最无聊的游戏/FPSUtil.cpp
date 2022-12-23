#include "FPSUtil.h"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

FPSUtil::FPSUtil ()
{
}

FPSUtil::~FPSUtil ()
{
}

std::chrono::steady_clock::time_point FPSUtil::startTime = std::chrono::high_resolution_clock::now();
float FPSUtil::fps = 0;
float FPSUtil::deltaTime = 0;

void FPSUtil::setStartTime () {
	startTime = std::chrono::high_resolution_clock::now();
}

void FPSUtil::setEndTime () {
	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
	double sleepTime = 1000 / 60 - elapsed.count();
	if (sleepTime > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds((long)sleepTime));
	}
	endTime = std::chrono::high_resolution_clock::now();
	elapsed = endTime - startTime;
	// 计算fps
	fps = 1000 / elapsed.count();
	// 计算每帧消耗的秒数
	deltaTime = elapsed.count() / 1000;
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