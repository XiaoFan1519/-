#include "init.h"
#include "Circle.h"


using namespace std;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void DrawCircle (HWND);

// 保存圆的集合
vector<Circle*> circles;

// 保存被删除的圆的索引
queue<int> indexs;

// 60 FPS 
// 每次 ≈ 17 ms
const ULONGLONG FPS_60 = 17;

// 保存鼠标坐标
POINT cursorPos;

// d2d 需要的变量
ID2D1Factory* m_pDirect2dFactory;
ID2D1HwndRenderTarget* m_pRenderTarget;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	// 系统初始化
	HRESULT hr = CoInitialize (NULL);

	// 创建d2d资源
	hr = S_OK;
	// Create a Direct2D factory.
	hr = D2D1CreateFactory (D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	// 初始化容器
	circles.reserve (10000);

	//初始化个圆
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 800;
	rect.bottom = 800;
	COLORREF color = RGB (rand () % 256, rand () % 256, rand () % 256);
	Circle* circle = new Circle (rect, RECT (), color, 0, TRUE, 400);
	circles.push_back (circle);

	//下面都是创建窗口的代码
	static TCHAR szAppName[] = TEXT ("CircleGame");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject (BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow (szAppName, TEXT ("这是我见过的最无聊的游戏"), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, 0, 0, 800 + 16, 800 + 38, NULL, NULL, hInstance, NULL);

	ShowWindow (hwnd, iCmdShow);
	UpdateWindow (hwnd);

	do {
		//利用空闲时间来画画
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}

		DrawCircle (hwnd);

	} while (msg.message != WM_QUIT);

	//删除所有的圆的指针
	for (auto i = circles.begin (); i != circles.end (); ++i)
	{
		if (nullptr == *i)
		{
			continue;
		}
		delete *i;
		*i = NULL;
	}
	circles.clear ();


	// 释放 COM
	SafeRelease (&m_pDirect2dFactory);

	CoUninitialize ();

	return msg.wParam;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage (0);
		return 0;
	case WM_MOUSEMOVE:
		cursorPos.x = LOWORD (lParam);
		cursorPos.y = HIWORD (lParam);
		return 0;
	}
	return DefWindowProc (hwnd, iMsg, wParam, lParam);
}

// 增加圆到容器中
void AddCircles (Circle* c)
{
	if (indexs.empty ())
	{
		circles.push_back (c);
		return;
	}

	int index = indexs.front ();
	indexs.pop ();
	circles[index] = c;
}

// 增加四个圆
void AddCircle (Circle* c) {
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
	cTmp = new Circle (rTmp, c->m_location, c->m_Color, 0, FALSE, len);
	AddCircles (cTmp);

	rTmp.left = c->m_location.left + len;
	rTmp.top = c->m_location.top;
	rTmp.right = c->m_location.right;
	rTmp.bottom = c->m_location.bottom - len;
	cTmp = new Circle (rTmp, c->m_location, c->m_Color, 1, FALSE, len);
	AddCircles (cTmp);

	rTmp.left = c->m_location.left;
	rTmp.top = c->m_location.top + len;
	rTmp.right = c->m_location.right - len;
	rTmp.bottom = c->m_location.bottom;
	cTmp = new Circle (rTmp, c->m_location, c->m_Color, 2, FALSE, len);
	AddCircles (cTmp);

	rTmp.left = c->m_location.left + len;
	rTmp.top = c->m_location.top + len;
	rTmp.right = c->m_location.right;
	rTmp.bottom = c->m_location.bottom;
	cTmp = new Circle (rTmp, c->m_location, c->m_Color, 3, FALSE, len);
	AddCircles (cTmp);
}

// 创建 RenderTarget 资源
void CreateDeviceResources (HWND hWnd)
{
	RECT rc;
	GetClientRect (hWnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU (
		rc.right - rc.left,
		rc.bottom - rc.top
	);

	// Create a Direct2D render target.
	m_pDirect2dFactory->CreateHwndRenderTarget (
		D2D1::RenderTargetProperties (),
		D2D1::HwndRenderTargetProperties (hWnd, size),
		&m_pRenderTarget
	);
}

void DrawCircle (HWND hWnd)
{
	ULONGLONG start_time = GetTickCount64 ();
	ULONGLONG end_time;

	CreateDeviceResources (hWnd);

	if (nullptr == m_pRenderTarget)
	{
		return;
	}

	// 开始画画
	m_pRenderTarget->BeginDraw ();
	m_pRenderTarget->SetTransform (D2D1::Matrix3x2F::Identity ());
	m_pRenderTarget->Clear (D2D1::ColorF (D2D1::ColorF::Black));

	int count = circles.size ();
	for (int index = 0; index < count; ++index)
	{
		Circle* c = circles[index];
		if (nullptr == c)
		{
			continue;
		}

		if (c->InCircle (cursorPos))
		{
			AddCircle (c);
			//别忘了删!!!
			delete c;
			circles[index] = nullptr;
			indexs.push (index);
			continue;
		}

		c->IsOk ();
		c->Paint (m_pRenderTarget);
	}

	// 结束画画
	m_pRenderTarget->EndDraw ();

	// 释放资源
	SafeRelease (&m_pRenderTarget);

	end_time = GetTickCount64 ();
	ULONGLONG cost = end_time - start_time;
	if (cost < FPS_60)
	{
		Sleep (FPS_60 - cost);
	}
}