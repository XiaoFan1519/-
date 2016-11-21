#pragma once
#include <Windows.h>

class Circle{
public:
	//Բ����ʵ��С
	D2D1_ELLIPSE m_ellipse;
	//������С��Բ�Ĵ�С
	D2D1_ELLIPSE m_oldEllipse;
	//Բ����ɫ
	COLORREF m_Color;
	//flag�������4��Բ�е��ĸ�Բ ��0��ʼ
	//0 1
	//2 3
	int m_flag;
	//�Ƿ���������Ϣ
	BOOL m_ok;
	//ÿ����С��ֵ
	float m_len;

	// ���캯��
	// �������꣬��ǰ���꣬��ɫֵ��flag��ʶ���Ƿ���������Ϣ
	Circle(D2D1_ELLIPSE, D2D1_ELLIPSE, COLORREF, int, BOOL);
	
	//�����Ƿ���Բ��
	BOOL InCircle(POINT);

	//�Ƿ������С��
	BOOL IsOk();
	
	//�Ա�����Rect�Ƿ����
	BOOL CompareRect(const D2D1_ELLIPSE&, const D2D1_ELLIPSE&);

	//��СԲ
	void SetSize();

	//��Բ
	void Paint(ID2D1HwndRenderTarget* m_pRenderTarget);
};