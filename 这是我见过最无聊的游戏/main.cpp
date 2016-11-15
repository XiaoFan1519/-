#include <windows.h>
#include <list>
#include<vector>
#include "Circle.h"


using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawCircle(HWND);

// 保存圆的集合
// list<Circle*> circles;
vector<Circle*> circles;
// 保存被删除的圆，但没有被delete的内存
list<Circle*> memory;
// vector<Circle*> circles;
RECT rect;

// 保存鼠标坐标
POINT cursorPos;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	// 初始化容器
	circles.reserve (8000);

	// 初始化资源
	

	//初始化个圆
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 800;
	rect.bottom = 800;
	COLORREF color = RGB(rand() % 256, rand() % 256, rand() % 256);
	Circle* circle = new Circle(rect,RECT() ,color, 0, TRUE,400);
	circles.push_back(circle);

	//下面都是创建窗口的代码
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
	hwnd = CreateWindow(szAppName, TEXT("这是我见过的最无聊的游戏"),WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,0, 0,800+16, 800+38,NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);

	//获取客户区大小
	GetClientRect(hwnd, &rect);

	UpdateWindow(hwnd);

	do{
		//利用空闲时间来画画
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		DrawCircle(hwnd);
	
	}while (msg.message != WM_QUIT);

	//删除所有的圆的指针
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
	//圆的半径
	len = (c->m_location.right - c->m_location.left) / 2;

	//下面新建4个新圆
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
	// 透明画笔
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
			//别忘了删!!!
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
	

	//注意释放顺序
	SelectObject(hmdc, oldBit);
	SelectObject(hmdc, oldPen);

	// 释放资源
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