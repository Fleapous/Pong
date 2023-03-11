#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

# define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);


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

	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_TUTORIAL, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	bool tmp;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_EX_LAYERED | WS_OVERLAPPED,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance,
		nullptr);
	tmp = SetLayeredWindowAttributes(hWnd, 0, (255 * 0.8), LWA_ALPHA);

	//if (!tmp){
	//	return false;
	//}

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	//case WM_COMMAND:
	//{
	//	int wmId = LOWORD(wParam);
	//	switch (wmId)
	//	{

	//		//case IDM_ABOUT:
	//		//	DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
	//		//	break;
	//		//case IDM_EXIT:
	//		//	DestroyWindow(hWnd);
	//		//	break;
	//	default:
	//		return DefWindowProc(hWnd, message, wParam, lParam);

	//	}
	//}
	//break;

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
	//case WM_CREATE:
	//{
	//	const COLORREF rgbRed = 0x000000FF;
	//	SetLayeredWindowAttributes(hWnd, rgbRed, 100, LWA_ALPHA);
	//}
	break;
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