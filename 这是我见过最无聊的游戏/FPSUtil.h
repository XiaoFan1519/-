#pragma once
#include "init.h"
#include <chrono>

class FPSUtil
{
private:
	// ��ǰFPS
	static float fps;
	// ÿ֡���ѵ�����
	static float deltaTime;
	static std::chrono::steady_clock::time_point startTime;
public:
	FPSUtil ();
	~FPSUtil ();

	// ���ÿ�ʼʱ��
	static void setStartTime ();
	
	// ���ý���ʱ��
	static void setEndTime ();

	// ��ȡ��ǰFPS
	static float getFPS ();

	// ��ȡÿ֡���ѵ�����
	static float getDeltaTime ();
};

// ����Ļ�ϻ���FPS
void fps (ID2D1HwndRenderTarget* m_pRenderTarget, IDWriteTextFormat* m_pTextFormat);