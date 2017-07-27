#include "init.h"
#include "Circle.h"
#include "FPSUtil.h"

Circle::Circle (D2D1_ELLIPSE ellipse, D2D1_ELLIPSE oldEllipse, COLORREF color, int f, BOOL ok) : 
	m_ellipse (ellipse), m_oldEllipse (oldEllipse), m_Color (color), m_flag (f), m_ok (ok) {
	
	len = oldEllipse.radiusX - ellipse.radiusX;
}

float InvSqrt (float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*) &x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*) &i;
	x = x * (1.5f - xhalf * x * x);
	return 1 / x;
}

long myabs (long num)
{
	return num > 0 ? num : -num;
}

BOOL Circle::InCircle (POINT point) {
	if (!(this->m_ok)) {
		return FALSE;
	}

	if (this->m_ellipse.radiusX <= 4.0f) {
		//圆太小了
		return FALSE;
	}

	// 勾股定理
	long a = myabs (point.x - this->m_ellipse.point.x);
	long b = myabs (point.y - this->m_ellipse.point.y);
	long c = a * a + b * b;

	// 溢出判断
	if (c <= 0)
	{
		return FALSE;
	}

	c = InvSqrt (c);

	// 溢出判断
	if (c <= 0)
	{
		return FALSE;
	}

	// 判断是否在圆的范围内
	if (c > this->m_ellipse.radiusX)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL Circle::IsOk () {
	if (this->m_ok)
	{
		return TRUE;
	}
	
	this->SetSize ();
	return FALSE;
}

void Circle::SetSize () 
{
	//每次缩小的值
	float m_len = 0;
	// 每1秒需要减去的值
	float tmp = len / 1;
	// 计算每帧要减去的值
	m_len = FPSUtil::getDeltaTime () * tmp;

	if (this->CompareRect (m_ellipse, m_oldEllipse)) 
	{
		this->m_ok = TRUE;
		//完成缩小后,改变颜色
		this->m_Color = RGB (rand () % 255 + 1, rand () % 255 + 1, rand () % 255 + 1);
		return;
	}
	
	// 缩小半径
	this->m_oldEllipse.radiusX =
		this->m_oldEllipse.radiusY -= m_len;

	D2D1_POINT_2F& point = this->m_oldEllipse.point;

	switch (this->m_flag)
	{
	case 0:
		point.x -= m_len;
		point.y -= m_len;
		break;
	case 1:
		point.x += m_len;
		point.y -= m_len;
		break;
	case 2:
		point.x -= m_len;
		point.y += m_len;
		break;
	case 3:
		point.x += m_len;
		point.y += m_len;
		break;
	default:
		break;
	}
}


BOOL Circle::CompareRect (const D2D1_ELLIPSE& r1, const D2D1_ELLIPSE& r2) 
{	
	if (r1.radiusX < r2.radiusX)
	{
		return false;
	}

	if (r1.radiusY < r2.radiusY)
	{
		return false;
	}

	/*if (r1.point.x != r2.point.x)
	{
		return false;
	}

	if (r1.point.y != r2.point.y)
	{
		return false;
	}*/

	return true;
}

void Circle::Paint (ID2D1HwndRenderTarget* m_pRenderTarget) 
{
	// ID2D1SolidColorBrush** 
	ID2D1SolidColorBrush* brush = nullptr;
	m_pRenderTarget->CreateSolidColorBrush (
		D2D1::ColorF (this->m_Color),
		&brush);
	
	D2D1_ELLIPSE ellipse = this->m_ellipse;

	// 如果没画完，就画正在缩小的圆
	if (!this->m_ok)
	{
		ellipse = this->m_oldEllipse;
	}

	m_pRenderTarget->FillEllipse (ellipse,
		brush);

	SafeRelease (&brush);
}