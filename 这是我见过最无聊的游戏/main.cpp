#include "init.h"
#include "Circle.h"
#include "FPSUtil.h"

using namespace std;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void DrawCircle (HWND);

// 不需要重绘的圆的集合
vector<Circle*> doneList;

// 需要重绘的圆的集合
list<Circle*> paintList;

// 保存被删除的圆的索引
queue<int> indexs;

// 保存鼠标坐标
POINT cursorPos;

// d2d 需要的变量
ID2D1Factory* m_pDirect2dFactory;
ID2D1HwndRenderTarget* m_pRenderTarget;
IDWriteFactory* m_pDWriteFactory;
IDWriteTextFormat* m_pTextFormat;

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
		D2D1::RenderTargetProperties (D2D1_RENDER_TARGET_TYPE_HARDWARE,
			D2D1::PixelFormat (),
			0.0,
			0.0,
			D2D1_RENDER_TARGET_USAGE_NONE,
			D2D1_FEATURE_LEVEL_10),
		D2D1::HwndRenderTargetProperties (hWnd, size, D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS),
		&m_pRenderTarget
	);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	// 系统初始化
	HRESULT hr = CoInitialize (NULL);

	// 创建d2d资源
	hr = S_OK;
	// Create a Direct2D factory.
	hr = D2D1CreateFactory (D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	hr = DWriteCreateFactory (
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
	);

	hr = m_pDWriteFactory->CreateTextFormat (
		L"Gabriola",                   // Font family name
		NULL,                          // Font collection(NULL sets it to the system font collection)
		DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		DWRITE_FONT_STYLE_NORMAL,      // Style
		DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		16.0f,                         // Size    
		L"en-us",                      // Local
		&m_pTextFormat                 // Pointer to recieve the created object
	);

	// 初始化容器
	doneList.reserve (10000);

	//初始化个圆
	D2D1_ELLIPSE ellipse;
	ellipse.point.x = 
		ellipse.point.y = 400;
	ellipse.radiusX =
		ellipse.radiusY = 400;

	COLORREF color = RGB (rand () % 256, rand () % 256, rand () % 256);
	Circle* circle = new Circle (ellipse, ellipse, color, 0, TRUE);
	doneList.push_back (circle);

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
	hwnd = CreateWindow (szAppName, TEXT ("这是我见过的最无聊的游戏"), 
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
		0, 0, 
		800 + 16 + 160, 800 + 38,
		NULL, NULL, hInstance, NULL);

	ShowWindow (hwnd, iCmdShow);
	UpdateWindow (hwnd);

	CreateDeviceResources (hwnd);
	do {
		//利用空闲时间来画画
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
		else
		{
			DrawCircle (hwnd);
		}

	} while (msg.message != WM_QUIT);
	// 释放资源
	SafeRelease (&m_pRenderTarget);

	//删除所有的圆的指针
	for (auto i = doneList.begin (); i != doneList.end (); ++i)
	{
		if (nullptr == *i)
		{
			continue;
		}
		delete *i;
	}
	doneList.clear ();


	// 释放 COM
	SafeRelease (&m_pDirect2dFactory);
	SafeRelease (&m_pDWriteFactory);
	SafeRelease (&m_pTextFormat);

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

// 添加到已完成列表中
void AddDone (Circle* c)
{
	if (indexs.empty ())
	{
		doneList.push_back (c);
		return;
	}

	int index = indexs.front ();
	indexs.pop ();
	doneList[index] = c;
}

// 增加四个圆
void AddCircle (Circle* c) 
{
	Circle *cTmp;
	
	// 半径
	float radius = c->m_ellipse.radiusX / 2;
	
	D2D1_ELLIPSE ellipse = c->m_ellipse;

	//下面新建4个新圆 flag 0
	ellipse.point.x -= radius;
	ellipse.point.y -= radius;
	ellipse.radiusX = ellipse.radiusY = radius;
	cTmp = new Circle (ellipse, c->m_ellipse, c->m_Color, 0, FALSE);
	paintList.push_front (cTmp);

	// flag 1
	ellipse = c->m_ellipse;
	ellipse.point.x += radius;
	ellipse.point.y -= radius;
	ellipse.radiusX = ellipse.radiusY = radius;
	cTmp = new Circle (ellipse, c->m_ellipse, c->m_Color, 1, FALSE);
	paintList.push_front (cTmp);

	// flag 2
	ellipse = c->m_ellipse;
	ellipse.point.x -= radius;
	ellipse.point.y += radius;
	ellipse.radiusX = ellipse.radiusY = radius;
	cTmp = new Circle (ellipse, c->m_ellipse, c->m_Color, 2, FALSE);
	paintList.push_front (cTmp);

	// flag 3
	ellipse = c->m_ellipse;
	ellipse.point.x += radius;
	ellipse.point.y += radius;
	ellipse.radiusX = ellipse.radiusY = radius;
	cTmp = new Circle (ellipse, c->m_ellipse, c->m_Color, 3, FALSE);
	paintList.push_front (cTmp);
}

void DrawCircle (HWND hWnd)
{
	FPSUtil::setStartTime ();

	if (nullptr == m_pRenderTarget)
	{
		return;
	}

	// 开始画画
	m_pRenderTarget->BeginDraw ();
	m_pRenderTarget->SetTransform (D2D1::Matrix3x2F::Identity ());
	// m_pRenderTarget->Clear (D2D1::ColorF (D2D1::ColorF::Black));

	size_t count = doneList.size (); // 之前为了减少每次执行size浪费的时间，所以在循环前先获取大小
	for (size_t index = 0; index < count; ++index)
	{
		Circle* c = doneList[index];
		if (nullptr == c)
		{
			continue;
		}

		if (c->InCircle (cursorPos))
		{
			AddCircle (c);
			//别忘了删!!!
			delete c;
			doneList[index] = nullptr;
			indexs.push (index);
		}
	}
	
	// 绘制圆
	for (auto it = paintList.begin (); it != paintList.end (); ) {
		Circle* c = *it;
		if (c->IsOk ()) {
			// 从绘画列表中移出
			it = paintList.erase (it);
			AddDone (c);
		}
		else {
			c->Paint (m_pRenderTarget);
			it++;
		}
	}
	FPSUtil::setEndTime ();

	fps (m_pRenderTarget, m_pTextFormat);

	// 结束画画
	m_pRenderTarget->EndDraw ();
}