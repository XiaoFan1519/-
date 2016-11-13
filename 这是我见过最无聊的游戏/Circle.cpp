#include "Circle.h"
#include <cmath>

Circle::Circle(RECT rect, RECT oldrect, COLORREF color, int f, BOOL o, double len) :m_location(rect), m_old_location(oldrect), m_Color(color), flag(f), ok(o){
	//每次缩小1%;
	m_len = len*0.01;

	int length = (this->m_location.right - this->m_location.left) / 2;
	origin.x = this->m_location.left + length;
	length = (this->m_location.bottom - this->m_location.top) / 2;
	origin.y = this->m_location.top + length;

	this->radius = length;
}

BOOL Circle::InCircle(POINT point){
	if (!(this->ok)){
		return FALSE;
	}
	
	if (this->radius <= 4){
		//圆太小了
		return FALSE;
	}

	// 勾股定理
	int a = abs (point.x - this->origin.x);
	int b = abs (point.y - this->origin.y);
	int c = pow (a, 2) + pow(b, 2);
	c = sqrt (c);

	if (c > this->radius)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL Circle::IsOk(){
	if (this->ok)
	{
		return TRUE;
	}
	else
	{
		this->SetSize();
	}
	return FALSE;
}

void Circle::SetSize(){
	if (this->CompareRect(this->m_location, this->m_old_location)){
		this->ok = TRUE;
		//完成缩小后,改变颜色
		this->m_Color = RGB(rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1);
	}
	else
	{
		switch(this->flag)
		{
		case 0:
			this->m_old_location.right -= ceil(m_len);
			this->m_old_location.bottom -= ceil(m_len);
			break;
		case 1:
			this->m_old_location.left += ceil(m_len);
			this->m_old_location.bottom -= ceil(m_len);
			break;
		case 2:
			this->m_old_location.top += ceil(m_len);
			this->m_old_location.right -= ceil(m_len);
			break;
		case 3:
			this->m_old_location.left += ceil(m_len);
			this->m_old_location.top += ceil(m_len);
			break;
		default:
			break;
		}
	}
}


BOOL Circle::CompareRect(RECT r1, RECT r2){
	if (r1.right - r1.left <=r2.right - r2.left)
	{
		return FALSE;
	}
	return TRUE;
}

void Circle::Paint(HDC hdc){
	HBRUSH hBrush;

	hBrush = CreateSolidBrush(this->m_Color);
	auto oldBrush = SelectObject(hdc, hBrush);
	
	//如果缩小完成了,就画缩小好的圆
	if (this->ok)
	{
		Ellipse(hdc, this->m_location.left, this->m_location.top, this->m_location.right, this->m_location.bottom);
	}
	else
	{
		Ellipse(hdc, this->m_old_location.left, this->m_old_location.top, this->m_old_location.right, this->m_old_location.bottom);
	}
	
	SelectObject(hdc, oldBrush);

	DeleteObject(hBrush);
}