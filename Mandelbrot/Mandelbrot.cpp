// Mandelbrot.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Mandelbrot.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
const size_t MaxIterations(80000);               


// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
size_t IteratePoint(double zReal, double zImaginary, double cReal, double cImaginary);
COLORREF Color(int n);
void DrawSet(HWND hwnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MANDELBROT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MANDELBROT));

    MSG msg;

    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MANDELBROT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MANDELBROT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 2500, 1400, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
		DrawSet(hWnd);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

COLORREF Color(int n)
{
	if (n == MaxIterations) return RGB(0, 0, 0);

	const int nColors = 16;
	switch (n%nColors)
	{
	case 0: return RGB(100, 100, 100); 
	case 1: return RGB(100, 0, 0);
	case 2: return RGB(200, 0, 0);
	case 3: return RGB(100, 100, 0);
	case 4: return RGB(200, 100, 0);
	case 5: return RGB(200, 200, 0);
	case 6: return RGB(0, 200, 0);
	case 7: return RGB(0, 100, 100);
	case 8: return RGB(0, 200, 100);
	case 9: return RGB(0, 100, 200);
	case 10: return RGB(0, 200, 200);
	case 11: return RGB(0, 0, 200);
	case 12: return RGB(100, 0, 100);
	case 13: return RGB(200, 0, 100);
	case 14: return RGB(100, 0, 200);
	case 15: return RGB(200, 0, 200);

	default: return RGB(200, 200, 200);
	};
}

void DrawSet(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	int imageHeight(rect.bottom);
	int imageWidth(rect.right);

	HDC hdc(GetDC(hWnd));
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP bmp = CreateCompatibleBitmap(hdc, imageWidth, 1);
	HGDIOBJ oldBmp = SelectObject(memDC, bmp); 

	const double realMin(-2.1);
	double imaginaryMin(-1.3);
	double imaginaryMax(+1.3);

	double realMax(realMin + (imaginaryMax - imaginaryMin) * imageWidth / imageHeight);

	double realScale((realMax - realMin) / (imageWidth -1));
	double imaginaryScale((imaginaryMax - imaginaryMin) / (imageHeight -1));

	double cReal(0.0);
	double cImaginary(0.0);
	double zReal(0.0);
	double zImaginary(0.0);

	for (int y = 0; y < imageHeight; ++y)
	{
		zImaginary = cImaginary = imaginaryMax - y * imaginaryScale;
		for (int x = 0; x < imageWidth; ++x)
		{
			zReal = cReal = realMin + x*realScale;
			SetPixel(memDC, x, 0, Color(IteratePoint(zReal, zImaginary, cReal, cImaginary)));
		}
		BitBlt(hdc, 0, y, imageWidth, 1, memDC, 0, 0, SRCCOPY);
	}
	SelectObject(memDC, oldBmp);
	DeleteObject(bmp);
	DeleteDC(memDC);
	ReleaseDC(hWnd, hdc);
}

size_t IteratePoint(double zReal, double zImaginary, double cReal, double cImaginary)
{
	double zReal2(0.0), zImaginary2(0.0);
	size_t n(0);
	for (; n < MaxIterations; ++n)
	{
		zReal2 = zReal*zReal;
		zImaginary2 = zImaginary * zImaginary;
		if (zReal2 + zImaginary2 > 4)
			break;
		zImaginary = 2 * zReal*zImaginary + cImaginary;
		zReal = zReal2 - zImaginary2 + cReal;
	}
	return n;
}