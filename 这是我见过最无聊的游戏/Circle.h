#include <Windows.h>

class Circle{
public:
	//圆的真实大小
	RECT m_location;
	//正在缩小的圆的大小
	RECT m_old_location;
	//圆的颜色
	COLORREF m_Color;
	//flag标记了是4个圆中的哪个圆 从0开始
	//0 1
	//2 3
	int flag;
	//是否接收鼠标消息
	BOOL ok;
	//每次缩小的值
	double m_len;

	Circle(RECT, RECT, COLORREF, int, BOOL, double);
	
	//坐标是否在圆内
	BOOL InCircle(POINT);
	//是否完成缩小了
	BOOL IsOk();
	
	//对比两个Rect是否相等
	BOOL CompareRect(RECT, RECT);

	//缩小圆
	void SetSize();
	//画园
	void Paint(HDC);
};