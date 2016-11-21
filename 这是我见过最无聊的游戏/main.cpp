#include "init.h"
#include "Circle.h"


using namespace std;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void DrawCircle (HWND);

// ����Բ�ļ���
vector<Circle*> circles;

// ���汻ɾ����Բ������
queue<int> indexs;

// 60 FPS 
// ÿ�� �� 17 ms
const ULONGLONG FPS_60 = 17;

// �����������
POINT cursorPos;

// d2d ��Ҫ�ı���
ID2D1Factory* m_pDirect2dFactory;
ID2D1HwndRenderTarget* m_pRenderTarget;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	// ϵͳ��ʼ��
	HRESULT hr = CoInitialize (NULL);

	// ����d2d��Դ
	hr = S_OK;
	// Create a Direct2D factory.
	hr = D2D1CreateFactory (D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	// ��ʼ������
	circles.reserve (10000);

	//��ʼ����Բ
	D2D1_ELLIPSE ellipse;
	ellipse.point.x = 
		ellipse.point.y = 400;
	ellipse.radiusX =
		ellipse.radiusY = 400;

	COLORREF color = RGB (rand () % 256, rand () % 256, rand () % 256);
	Circle* circle = new Circle (ellipse, ellipse, color, 0, TRUE);
	circles.push_back (circle);

	//���涼�Ǵ������ڵĴ���
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
	hwnd = CreateWindow (szAppName, TEXT ("�����Ҽ����������ĵ���Ϸ"), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, 0, 0, 800 + 16, 800 + 38, NULL, NULL, hInstance, NULL);

	ShowWindow (hwnd, iCmdShow);
	UpdateWindow (hwnd);

	do {
		//���ÿ���ʱ��������
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}

		DrawCircle (hwnd);

	} while (msg.message != WM_QUIT);

	//ɾ�����е�Բ��ָ��
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


	// �ͷ� COM
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

// ����Բ��������
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

// �����ĸ�Բ
void AddCircle (Circle* c) 
{
	Circle *cTmp;
	
	// �뾶
	float radius = c->m_ellipse.radiusX / 2;
	
	D2D1_ELLIPSE ellipse = c->m_ellipse;

	//�����½�4����Բ flag 0
	ellipse.point.x -= radius;
	ellipse.point.y -= radius;
	ellipse.radiusX = ellipse.radiusY = radius;
	cTmp = new Circle (ellipse, c->m_ellipse, c->m_Color, 0, FALSE);
	AddCircles (cTmp);

	// flag 1
	ellipse = c->m_ellipse;
	ellipse.point.x += radius;
	ellipse.point.y -= radius;
	ellipse.radiusX = ellipse.radiusY = radius;
	cTmp = new Circle (ellipse, c->m_ellipse, c->m_Color, 1, FALSE);
	AddCircles (cTmp);

	// flag 2
	ellipse = c->m_ellipse;
	ellipse.point.x -= radius;
	ellipse.point.y += radius;
	ellipse.radiusX = ellipse.radiusY = radius;
	cTmp = new Circle (ellipse, c->m_ellipse, c->m_Color, 2, FALSE);
	AddCircles (cTmp);

	// flag 3
	ellipse = c->m_ellipse;
	ellipse.point.x += radius;
	ellipse.point.y += radius;
	ellipse.radiusX = ellipse.radiusY = radius;
	cTmp = new Circle (ellipse, c->m_ellipse, c->m_Color, 3, FALSE);
	AddCircles (cTmp);
}

// ���� RenderTarget ��Դ
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

	// ��ʼ����
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
			//������ɾ!!!
			delete c;
			circles[index] = nullptr;
			indexs.push (index);
			continue;
		}

		c->IsOk ();
		c->Paint (m_pRenderTarget);
	}

	// ��������
	m_pRenderTarget->EndDraw ();

	// �ͷ���Դ
	SafeRelease (&m_pRenderTarget);

	end_time = GetTickCount64 ();
	ULONGLONG cost = end_time - start_time;
	if (cost < FPS_60)
	{
		Sleep (FPS_60 - cost);
	}
}