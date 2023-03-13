#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <iostream>
#include "resource.h"
#include <commdlg.h>
using namespace std;

# define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

WCHAR BallClassName[MAX_LOADSTRING];

ATOM RegisterClassMain(HINSTANCE hInstance);
ATOM RegisterClassBall(HINSTANCE hInstance);
ATOM RegisterClassPaddle(HINSTANCE hInstance);

BOOL InitInstance(HINSTANCE, int);

LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcBall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcPaddle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

HBRUSH mainWindowBrush;

//ball vars 
HWND hWndBall;

//paddle vars
HWND paddleHandle;

//ball cordinates
int ballX = 20;
int ballY = 20;

// push forces
int X_axis = 1;
int Y_axis = 1;

//cursor cords
int cursor_X = 0;
int cursor_Y = 0;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//std::wstring title = LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//title.copy(szTitle, title.size());

	//std::wstring className = LoadString(hInstance, IDC_TUTORIAL, szWindowClass, MAX_LOADSTRING);
	//className.copy(szWindowClass, className.size());

	std::wstring title(L" tutorial \0");
	title.copy(szTitle, title.size());

	std::wstring className(L" TUTORIAL \0");
	className.copy(szWindowClass, className.size());

	std::wstring ballName(L" ballClassName \0");
	className.copy(BallClassName, className.size());

	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_TUTORIAL, szWindowClass, MAX_LOADSTRING);

	RegisterClassMain(hInstance);
	RegisterClassBall(hInstance);
	RegisterClassPaddle(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL));


	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast <int>(msg.wParam);
}

ATOM RegisterClassMain(HINSTANCE hInstance)
{

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProcMain;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	/*wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TUTORIAL));*/
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = /*(HBRUSH)CreateSolidBrush(RGB(0, 255, 0))*/ nullptr;
	/*wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TUTORIAL);*/
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
	//wcex.lpszClassName = 
	wcex.lpszClassName = szWindowClass;
	//wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

ATOM RegisterClassBall(HINSTANCE hInstance) 
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProcBall;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"BallClass";
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

ATOM RegisterClassPaddle(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProcPaddle;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = reinterpret_cast <HBRUSH>(COLOR_ACTIVECAPTION + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"PaddleClass";
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	bool tmp;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_EX_LAYERED | WS_OVERLAPPED | WS_EX_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance,
		nullptr);

	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, 0, (255 * 0.8), LWA_ALPHA);

	if (!hWnd) {
		return FALSE;
	}

	hWndBall = CreateWindowW(L"BallClass", L"BallWindow", WS_CHILD | WS_VISIBLE, 0, 0, 30, 30, hWnd, nullptr, hInstance, nullptr);

	if (!hWndBall)
	{
		return FALSE;
	}


	HWND hWndPaddle = CreateWindowW(L"PaddleClass", L"PaddleWindow", WS_CHILD | WS_VISIBLE, 470, 0, 20, 70, hWnd, nullptr, hInstance, nullptr);

	if (!hWndPaddle)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	ShowWindow(hWndBall, nCmdShow);
	ShowWindow(hWndPaddle, nCmdShow);

	UpdateWindow(hWnd);
	UpdateWindow(hWndBall);
	UpdateWindow(hWndPaddle);
	return TRUE;
}

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
		{
			DestroyWindow(hWnd);
		}
		break;
		case IDM_NEWGAME:
		{
			X_axis = 1;
			Y_axis = 1;
			ballX = 20;
			ballY = 20;
			MoveWindow(hWndBall, ballX, ballY, 20, 20, TRUE);
		}
		break;
		case IDM_CHANGECOLOR:
		{

			// mostly taken from docs

			CHOOSECOLOR cc;                 // common dialog box structure 
			static COLORREF acrCustClr[16]; // array of custom colors
			static DWORD rgbCurrent;        // initial color selection

			// Initialize CHOOSECOLOR 
			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.rgbResult = rgbCurrent;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor(&cc) == TRUE)
			{
				mainWindowBrush = CreateSolidBrush(cc.rgbResult);
				rgbCurrent = cc.rgbResult;

			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
	}
	case WM_ERASEBKGND:
	{
		return 1;
	}
	break;
	case WM_PAINT:
	{

		PAINTSTRUCT ps; 
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rc;
		GetClientRect(hWnd, &rc);

		FillRect(hdc, &rc, (HBRUSH)mainWindowBrush);
		EndPaint(hWnd, &ps);

		//HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // red brush
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		int clientWidth = LOWORD(lParam);
		int clientHeight = HIWORD(lParam);

		int X = GetSystemMetrics(SM_CXSCREEN);
		int Y = GetSystemMetrics(SM_CYSCREEN);


		RECT rc;
		GetWindowRect(hWnd, &rc);
		SetWindowPos(hWnd, nullptr, X / 2 - 250, Y / 2 - 150, 500, 300, SWP_SHOWWINDOW);
		wchar_t s[256];
		swprintf_s(s, 256, L" Window 's size : %d x %d Client area 's size : % d x % d",
			rc.right - rc.left, rc.bottom - rc.top,
			clientWidth, clientHeight);
		SetWindowText(hWnd, s);
	}
	break;
	case WM_MOUSEMOVE:
	{
		cursor_X = LOWORD(lParam);
		cursor_Y = HIWORD(lParam);

		SendMessage(paddleHandle, WM_MOVE, 0, MAKELPARAM(cursor_X, cursor_Y));
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK WndProcBall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) 
	{
	case WM_CREATE:
	{
		HRGN reg = CreateEllipticRgn(0, 0, 20, 20);
		SetWindowRgn(hWnd, reg, true);

		SetTimer(hWnd, 5, 5, NULL);
	}
	break;
	case WM_TIMER:
	{
		// cheking if ball hits the paddle
		//rethink there is a problem with the logic
		if ((cursor_Y < ballY) && (ballY < cursor_Y + 70) && (ballX == 450))
		{
			X_axis = X_axis * -1;
		}

		//Y axis
		if ((ballY + 1 == 250))
		{
			Y_axis = Y_axis * -1;

		}
		if((ballY  - 1 <= -10))
		{
			Y_axis = Y_axis * -1;
		}

		//X axis

		if (ballX + 1 == 470)
		{
			X_axis *= 0;
			Y_axis *= 0;
		}

		if ((ballX - 1 == -10))
		{
			X_axis = X_axis * -1;
		}

		MoveWindow(hWnd, ballX += X_axis, ballY += Y_axis, 20, 20, TRUE);

	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK WndProcPaddle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	paddleHandle = hWnd;
	switch (message)
	{
	case WM_MOVE:
	{
		MoveWindow(hWnd, 470, HIWORD(lParam), 20, 70, TRUE);
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return static_cast<INT_PTR>(TRUE);

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return static_cast<INT_PTR>(TRUE);
		}
		break;
	}
	return static_cast <INT_PTR>(FALSE);
}