#include <Windows.h>

class Circle{
public:
	//Բ����ʵ��С
	RECT m_location;
	//������С��Բ�Ĵ�С
	RECT m_old_location;
	//Բ����ɫ
	COLORREF m_Color;
	//flag�������4��Բ�е��ĸ�Բ ��0��ʼ
	//0 1
	//2 3
	int flag;
	//�Ƿ���������Ϣ
	BOOL ok;
	//ÿ����С��ֵ
	double m_len;

	Circle(RECT, RECT, COLORREF, int, BOOL, double);
	
	//�����Ƿ���Բ��
	BOOL InCircle(POINT);
	//�Ƿ������С��
	BOOL IsOk();
	
	//�Ա�����Rect�Ƿ����
	BOOL CompareRect(RECT, RECT);

	//��СԲ
	void SetSize();
	//��԰
	void Paint(HDC);
};