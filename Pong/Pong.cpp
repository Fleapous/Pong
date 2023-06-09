#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <iostream>
#include "resource.h"
#include <commdlg.h>
#include <vector>
using namespace std;

# define MAX_LOADSTRING 100


HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

WCHAR BallClassName[MAX_LOADSTRING];

ATOM RegisterClassMain(HINSTANCE hInstance);
ATOM RegisterClassBall(HINSTANCE hInstance);
ATOM RegisterClassPaddle(HINSTANCE hInstance);
ATOM RegisterClassBallTrial(HINSTANCE hInstance);

BOOL InitInstance(HINSTANCE, int);

LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);
void NewGame(const HWND& hWnd);
LRESULT CALLBACK WndProcBall(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcBallTrail(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ballMover(const HWND& hWnd);
LRESULT CALLBACK WndProcPaddle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

//main window vars
HBRUSH colorBrush = CreateSolidBrush(RGB(0, 200, 0));
HBITMAP bitMapBrush = nullptr;
HWND hMain;
bool brushTypeFlag = false;
bool bitMode = false;
int leftCounter = 0;
int rightCounter = 0;
struct trail{
	HWND handle;
	int timeToDie;
}bTrail;
vector<trail> ballTrail;

//ball vars 
HWND hWndBall;
void ballMover(const HWND& hWnd);

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
	RegisterClassBallTrial(hInstance);

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = nullptr;
	wcex.hbrBackground = /*(HBRUSH)CreateSolidBrush(RGB(0, 255, 0))*/ colorBrush;
	/*wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TUTORIAL);*/
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
	//wcex.lpszClassName = 
	wcex.lpszClassName = szWindowClass;
	//wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

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

ATOM RegisterClassBallTrial(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEXW);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProcBallTrail;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"BallTrailClass";
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	bool tmp;

	hMain = CreateWindowW(szWindowClass, szTitle, WS_EX_LAYERED | WS_OVERLAPPED | WS_EX_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance,
		nullptr);

	SetWindowLong(hMain, GWL_EXSTYLE, GetWindowLong(hMain, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hMain, 0, (255 * 0.8), LWA_ALPHA);

	if (!hMain) {
		return FALSE;
	}

	hWndBall = CreateWindowW(L"BallClass", L"BallWindow", WS_CHILD | WS_VISIBLE, 0, 0, 30, 30, hMain, nullptr, hInstance, nullptr);

	if (!hWndBall)
	{
		return FALSE;
	}


	HWND hWndPaddle = CreateWindowW(L"PaddleClass", L"PaddleWindow", WS_CHILD | WS_VISIBLE, 470, 0, 20, 70, hMain, nullptr, hInstance, nullptr);

	if (!hWndPaddle)
	{
		return FALSE;
	}

	ShowWindow(hMain, nCmdShow);
	ShowWindow(hWndBall, nCmdShow);
	ShowWindow(hWndPaddle, nCmdShow);

	UpdateWindow(hMain);
	UpdateWindow(hWndBall);
	UpdateWindow(hWndPaddle);
	return TRUE;
}

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP hBitmap;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, About);
		}
		break;
		case IDM_EXIT:
		{
			DestroyWindow(hWnd);
		}
		break;
		case IDM_NEWGAME:
		{
			NewGame(hWnd);
		}
		break;
		case IDM_TILE:
		{
			bitMode = true;
			CheckMenuItem(GetMenu(hWnd), IDM_TILE, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), IDM_STRETCH, MF_UNCHECKED);

			InvalidateRect(hWnd, NULL, true);
		}
		break;
		case IDM_STRETCH:
		{
			bitMode = false;
			CheckMenuItem(GetMenu(hWnd), IDM_TILE, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), IDM_STRETCH, MF_CHECKED);

			InvalidateRect(hWnd, NULL, true);
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
				colorBrush = CreateSolidBrush(cc.rgbResult);
				rgbCurrent = cc.rgbResult;

			}
			brushTypeFlag = false;
			InvalidateRect(hWnd, NULL, TRUE);

			NewGame(hWnd);
		}
		break;
		case IDM_BITMAP:
		{
			WCHAR szFile[100];
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = L"Bitmap file\0*.BMP\0";
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = 100;
			ofn.nFilterIndex = 1;

			if(GetOpenFileName(&ofn))
			{
				bitMapBrush = (HBITMAP)LoadImage(hInst, ofn.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

			}
			
			InvalidateRect(hWnd, NULL, true);
			InvalidateRect(hWndBall, NULL, true);
			InvalidateRect(paddleHandle, NULL, true);
			brushTypeFlag = true;
			NewGame(hWnd);
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
	}
	case WM_CREATE:
	{
		SetTimer(hWnd, 200, 70, NULL);

		EnableMenuItem(GetMenu(hWnd), IDM_STRETCH, MF_DISABLED);
		EnableMenuItem(GetMenu(hWnd), IDM_TILE, MF_DISABLED);
	}
	break;
	case WM_TIMER:
	{
		HWND tmp = CreateWindowW(L"BallTrailClass", nullptr, WS_CHILD | WS_VISIBLE, ballX, ballY, 20, 20, hWnd, nullptr, nullptr, nullptr);
		bTrail.handle = tmp;
		bTrail.timeToDie = 200;
		ballTrail.push_back(bTrail);

		ShowWindow(tmp, SW_SHOW);
		UpdateWindow(tmp);
	}
	case WM_ERASEBKGND:
	{
		return 1;
	}
	break;
	case WM_PAINT:
	{
		//wchar_t buffer1[11];
		//wchar_t buffer2[11];
		//wchar_t buffer3[11];
		//wchar_t buffer4[11];

		wchar_t leftCounterBuff[21];
		wchar_t rightCounterBuff[21];


		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC memHdc;
		HGDIOBJ oldBitmap;
		BITMAP bitmap;

		HFONT hFont = CreateFont(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FF_DONTCARE, L"Arial");
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

		if (brushTypeFlag)
		{
			//disable them too

			//re think the tile 
			EnableMenuItem(GetMenu(hWnd), IDM_STRETCH, MF_ENABLED);
			EnableMenuItem(GetMenu(hWnd), IDM_TILE, MF_ENABLED);

			memHdc = CreateCompatibleDC(hdc);
			oldBitmap = SelectObject(memHdc, bitMapBrush);

			GetObject(bitMapBrush, sizeof(bitmap), &bitmap);
			BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, memHdc, 0, 0, SRCCOPY);

			if (!bitMode)
			{
				StretchBlt(hdc, 0, 0, 500, 280, memHdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
			}
			else
			{
				for (int i = 0; i < 500; i += bitmap.bmWidth)
				{

					for (int j = 0; j < 300; j += bitmap.bmHeight)
					{
						BitBlt(hdc, i, j, bitmap.bmWidth, bitmap.bmWidth, memHdc, 0, 0, SRCCOPY);
					}
				}
			}
			SelectObject(memHdc, oldBitmap);
			DeleteDC(memHdc);



			//make bacground transperant

			LOGBRUSH lb;
			GetObject(colorBrush, sizeof(HBRUSH), &lb);

			COLORREF oposite = RGB(255 - GetRValue(lb.lbColor), 255 - GetGValue(lb.lbColor), 255 - GetBValue(lb.lbColor));
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, oposite);
			


			_itow_s(leftCounter, leftCounterBuff, sizeof(leftCounterBuff) / sizeof(wchar_t), 20);
			_itow_s(rightCounter, rightCounterBuff, sizeof(rightCounterBuff) / sizeof(wchar_t), 20);

			leftCounterBuff[wcslen(leftCounterBuff)] = L'\0';
			rightCounterBuff[wcslen(rightCounterBuff)] = L'\0';




			TextOutW(hdc, 121, 60, leftCounterBuff, wcslen(leftCounterBuff));
			TextOutW(hdc, 363, 60, rightCounterBuff, wcslen(rightCounterBuff));


		}
		else
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			FillRect(hdc, &rc, colorBrush);

			LOGBRUSH lb;
			GetObject(colorBrush, sizeof(HBRUSH), &lb);

			COLORREF oposite = RGB(255 - GetRValue(lb.lbColor), 255 - GetGValue(lb.lbColor), 255 - GetBValue(lb.lbColor));
			COLORREF bcgrndColor = RGB(GetRValue(lb.lbColor), GetGValue(lb.lbColor), GetBValue(lb.lbColor));
			SetTextColor(hdc, oposite);
			SetBkColor(hdc, bcgrndColor);


			_itow_s(leftCounter, leftCounterBuff, sizeof(leftCounterBuff) / sizeof(wchar_t), 20);
			_itow_s(rightCounter, rightCounterBuff, sizeof(rightCounterBuff) / sizeof(wchar_t), 20);

			leftCounterBuff[wcslen(leftCounterBuff)] = L'\0';
			rightCounterBuff[wcslen(rightCounterBuff)] = L'\0';




			TextOutW(hdc, 121, 60, leftCounterBuff, wcslen(leftCounterBuff));
			TextOutW(hdc, 363, 60, rightCounterBuff, wcslen(rightCounterBuff));

			
			/*for aligning the counters*/
			 
			//_itow_s(rc.left, buffer1, sizeof(buffer1) / sizeof(wchar_t), 10);
			//_itow_s(rc.right, buffer2, sizeof(buffer2) / sizeof(wchar_t), 10);
			//_itow_s(rc.top, buffer3, sizeof(buffer3) / sizeof(wchar_t), 10);
			//_itow_s(rc.bottom, buffer4, sizeof(buffer4) / sizeof(wchar_t), 10);

			//buffer1[wcslen(buffer1)] = L'\0';
			//buffer2[wcslen(buffer2)] = L'\0';
			//buffer3[wcslen(buffer3)] = L'\0';
			//buffer4[wcslen(buffer4)] = L'\0';

			//TextOutW(hdc, 100, 200, buffer1, wcslen(buffer1));
			//TextOutW(hdc, 200, 200, buffer2, wcslen(buffer2));
			//TextOutW(hdc, 150, 200, buffer3, wcslen(buffer3));
			//TextOutW(hdc, 250, 200, buffer4, wcslen(buffer4));

		}
		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
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

		SetTimer(hWnd, 50, 50, NULL);
	}
	break;
	case WM_TIMER:
	{
		ballMover(hWnd);

	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK WndProcBallTrail(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//dunno why i have the rect xd
	static int sizeX = 20;
	static int sizeY = 20;
	switch(message)
	{
	case WM_CREATE:
	{
		//LONG_PTR id = CHILD_WINDOW_ID_START + GetTickCount();
		//SetWindowLongPtr(hWnd, GWLP_USERDATA, id);

		HRGN reg = CreateEllipticRgn(0, 0, 20, 20);
		SetWindowRgn(hWnd, reg, true);

		SetTimer(hWnd, 50, 100, NULL);

	}
	break;

	case WM_TIMER:
	{

		RECT rect;
		GetClientRect(hWnd, &rect);

		int newWidth = rect.right - 5;
		int newHeight = rect.bottom - 5;
		SetWindowPos(hWnd, NULL, ballX, ballY, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);

		//HRGN reg = CreateEllipticRgn(0, 0, 20, 20);
		//SetWindowRgn(hWnd, reg, true);
	
		for (int i = 0; i < size(ballTrail); i++)
		{
			ballTrail[i].timeToDie--;
			if (ballTrail[i].timeToDie == 0)
			{
				DestroyWindow(ballTrail[i].handle);
			}
		}

		//RECT rect; 
		//GetClientRect(hWnd, &rect);
		//
		//HRGN reg = CreateEllipticRgn(ballX, ballY, rect.right - 1, rect.bottom - 1);
		//SetWindowRgn(hWnd, reg, true);

		/*SetWindowPos(hWnd, NULL, ballX, ballY, sizeX--, sizeY--, SWP_NOZORDER | SWP_NOMOVE);*/
		if (sizeX == 1) {
			
		}
		
	}
	break;
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
		if (!(HIWORD(lParam) > 180))
			MoveWindow(hWnd, 470, HIWORD(lParam), 20, 70, TRUE);
		else
		{
			MoveWindow(hWnd, 470, 180, 20, 70, TRUE);
		}
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

void ballMover(const HWND& hWnd)
{
	int Force = 20;

	// cheking if ball hits the paddle
	//rethink there is a problem with the logic
	if ((cursor_Y < ballY) && (ballY < cursor_Y + 70) && (ballX == 450))
	{
		X_axis = X_axis * -1;
		rightCounter++;
		InvalidateRect(hMain, NULL, TRUE);
	}

	//Y axis
	if ((ballY + 10 == 230))
	{
		Y_axis = Y_axis * -1;

	}
	if ((ballY - 10 <= -10))
	{
		Y_axis = Y_axis * -1;
	}

	//X axis

	if (ballX + 10 == 470)
	{
		leftCounter++;
		X_axis = 10;
		Y_axis = 10;
		ballX = 20;
		ballY = 20;
		MoveWindow(hWndBall, ballX, ballY, 20, 20, TRUE);
		InvalidateRect(hMain, NULL, TRUE);
	}

	if ((ballX - 10 == -10))
	{
		X_axis = X_axis * -1;
	}

	MoveWindow(hWnd, ballX += X_axis, ballY += Y_axis, 20, 20, TRUE);
}

void NewGame(const HWND& hWnd)
{
	X_axis = 10;
	Y_axis = 10;
	ballX = 20;
	ballY = 20;
	leftCounter = 0;
	rightCounter = 0;
	MoveWindow(hWndBall, ballX, ballY, 20, 20, TRUE);
	InvalidateRect(hWnd, NULL, TRUE);
}