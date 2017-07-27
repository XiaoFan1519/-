#pragma once
#include "init.h"

class FPSUtil
{
private:
	// 当前FPS
	static float fps;
	// 每帧消费的秒数
	static float deltaTime;
	static ULONGLONG startTime;
public:
	FPSUtil ();
	~FPSUtil ();

	// 设置开始时间
	static void setStartTime ();
	
	// 设置结束时间
	static void setEndTime ();

	// 获取当前FPS
	static float getFPS ();

	// 获取每帧消费的秒数
	static float getDeltaTime ();
};

// 在屏幕上画出FPS
void fps (ID2D1HwndRenderTarget* m_pRenderTarget, IDWriteTextFormat* m_pTextFormat);