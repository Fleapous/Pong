#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <iostream>
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

//paddle vars
HWND paddleHandle;

//ball cordinates
int ballX = 20;
int ballY = 20;

// push forces
int X_axis = 10;
int Y_axis = 10;

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

	HACCEL hAccelTable = LoadAccelerators(hInstance, /*MAKEINTRESOURCE(IDC_TUTORIAL)*/ nullptr);


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
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));
	/*wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TUTORIAL);*/
	wcex.lpszMenuName = nullptr;
	/*wcex.lpszClassName = MAKEINTRESOURCE(IDC_TUTORIAL);*/
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

	HWND hWndBall = CreateWindowW(L"BallClass", L"BallWindow", WS_CHILD | WS_VISIBLE, 0, 0, 30, 30, hWnd, nullptr, hInstance, nullptr);

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

		SetTimer(hWnd, 50, 250, NULL);
	}
	break;
	case WM_TIMER:
	{
		// cheking if ball hits the paddle
		if ((cursor_Y < ballY) && (cursor_Y < ballY + 70) && (ballX == 450))
		{
			X_axis = X_axis * -1;
		}

		//Y axis
		if ((ballY + 10 == 250))
		{
			Y_axis = Y_axis * -1;

		}
		if((ballY  - 10 <= -10))
		{
			Y_axis = Y_axis * -1;
		}

		//X axis

		if (ballX + 10 == 470)
		{
			X_axis *= 0;
			Y_axis *= 0;
		}

		if ((ballX - 10 == -10))
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