#pragma once
#include <Windows.h>

class Circle{
public:
	//圆的真实大小
	D2D1_ELLIPSE m_ellipse;
	//正在缩小的圆的大小
	D2D1_ELLIPSE m_oldEllipse;
	//圆的颜色
	COLORREF m_Color;
	//flag标记了是4个圆中的哪个圆 从0开始
	//0 1
	//2 3
	int m_flag;
	//是否接收鼠标消息
	BOOL m_ok;
	//每次缩小的值
	float m_len;

	// 构造函数
	// 最终坐标，当前坐标，颜色值，flag标识，是否接收鼠标消息
	Circle(D2D1_ELLIPSE, D2D1_ELLIPSE, COLORREF, int, BOOL);
	
	//坐标是否在圆内
	BOOL InCircle(POINT);

	//是否完成缩小了
	BOOL IsOk();
	
	//对比两个Rect是否相等
	BOOL CompareRect(const D2D1_ELLIPSE&, const D2D1_ELLIPSE&);

	//缩小圆
	void SetSize();

	//画圆
	void Paint(ID2D1HwndRenderTarget* m_pRenderTarget);
};