#include <windows.h>
#include <list>
#include<vector>
#include "Circle.h"


using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawCircle(HWND);

// ����Բ�ļ���
// list<Circle*> circles;
vector<Circle*> circles;
// ���汻ɾ����Բ����û�б�delete���ڴ�
list<Circle*> memory;
// vector<Circle*> circles;
RECT rect;

// �����������
POINT cursorPos;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	// ��ʼ������
	circles.reserve (8000);

	// ��ʼ����Դ
	

	//��ʼ����Բ
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 800;
	rect.bottom = 800;
	COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);
	Circle* circle = new Circle(rect,RECT() ,color, 0, TRUE,400);
	circles.push_back(circle);

	//���涼�Ǵ������ڵĴ���
	static TCHAR szAppName[] = TEXT("CircleGame");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName, TEXT("�����Ҽ����������ĵ���Ϸ"),WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,0, 0,800+16, 800+38,NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);

	//��ȡ�ͻ�����С
	GetClientRect(hwnd, &rect);

	UpdateWindow(hwnd);

	do{
		//���ÿ���ʱ��������
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		DrawCircle(hwnd);
	
	}while (msg.message != WM_QUIT);

	//ɾ�����е�Բ��ָ��
	for (auto i = circles.begin(); i != circles.end(); ++i)
	{
		if (nullptr == *i)
		{
			continue;
		}
		delete *i;
		*i = NULL;
	}
	circles.clear();

	for (auto i = memory.begin (); i != memory.end (); ++i)
	{
		delete *i;
		*i = NULL;
	}
	memory.clear();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
		cursorPos.x = LOWORD (lParam);
		cursorPos.y = HIWORD (lParam);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void AddCircle( Circle* c){
	RECT rTmp;
	Circle *cTmp;
	int len;
	//Բ�İ뾶
	len = (c->m_location.right - c->m_location.left) / 2;

	//�����½�4����Բ
	rTmp.left = c->m_location.left;
	rTmp.top = c->m_location.top;
	rTmp.right = c->m_location.right - len;
	rTmp.bottom = c->m_location.bottom - len;
	cTmp = new Circle(rTmp, c->m_location, c->m_Color, 0,FALSE,len);
	circles.push_back(cTmp);

	rTmp.left = c->m_location.left + len;
	rTmp.top = c->m_location.top;
	rTmp.right = c->m_location.right;
	rTmp.bottom = c->m_location.bottom - len;
	cTmp = new Circle(rTmp, c->m_location, c->m_Color, 1, FALSE, len);
	circles.push_back(cTmp);

	rTmp.left = c->m_location.left;
	rTmp.top = c->m_location.top + len;
	rTmp.right = c->m_location.right - len;
	rTmp.bottom = c->m_location.bottom;
	cTmp = new Circle(rTmp, c->m_location, c->m_Color, 2, FALSE, len);
	circles.push_back(cTmp);

	rTmp.left = c->m_location.left + len;
	rTmp.top = c->m_location.top + len;
	rTmp.right = c->m_location.right;
	rTmp.bottom = c->m_location.bottom;
	cTmp = new Circle(rTmp, c->m_location, c->m_Color, 3, FALSE, len);
	circles.push_back(cTmp);
}

void DrawCircle(HWND hWnd)
{
	ULONGLONG start_time = GetTickCount64 ();
	ULONGLONG end_time;

	HDC hdc;
	HDC hmdc;
	HBITMAP hBit;
	HPEN hPen;

	hdc = GetDC(hWnd);
	hmdc = CreateCompatibleDC(hdc);
	hBit = CreateCompatibleBitmap (hdc, 800, 800);
	// ͸������
	hPen = CreatePen (PS_NULL, 0, 0);
	auto oldPen = SelectObject(hmdc, hPen);
	auto oldBit = SelectObject(hmdc, hBit);

	for (auto i = circles.begin(); i != circles.end();)
	{
		if (nullptr == *i)
		{
			++i;
			continue;
		}

		if ((*i)->InCircle(cursorPos))
		{
			AddCircle((*i));
			//������ɾ!!!
			delete (*i);
			// circles.erase(i++);
			*i = nullptr;
			continue;
		}

		(*i)->IsOk();

		(*i)->Paint(hmdc);

		++i;
	}

	BitBlt(hdc, 0, 0, 800, 800, hmdc, 0, 0, SRCCOPY);
	

	//ע���ͷ�˳��
	SelectObject(hmdc, oldBit);
	SelectObject(hmdc, oldPen);

	// �ͷ���Դ
	DeleteObject(hBit);
	DeleteObject (hPen);
	DeleteDC(hmdc);
	ReleaseDC(hWnd, hdc);

	end_time = GetTickCount64 ();
	ULONGLONG cost = end_time - start_time;
	if (cost < 16)
	{
		Sleep (16 - cost);
	}
}