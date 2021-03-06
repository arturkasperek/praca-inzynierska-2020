// GraphicApp.cpp
//

#include "stdafx.h"
#include "GraphicApp.h"
#include <string>
#include <cstdlib>
#include <shellapi.h>

#define MAX_LOADSTRING 100

// Zmienne globalne:
HINSTANCE hInst;                             
WCHAR szTitle[MAX_LOADSTRING];               
WCHAR szWindowClass[MAX_LOADSTRING];           

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND button;
HWND label;
HWND firstNum;

int LPWSTRToNumber(LPWSTR val) {
	return _tstoi(val);
}

void sendMessageToAnotherApp(std::string message) {
	//DWORD data = std::strtoul(message.c_str(), NULL, sizeof(DWORD));
	COPYDATASTRUCT cds;
	const char * data = message.c_str();
	//cds.dwData = 1;
	cds.cbData = (strlen(data) + 1) * sizeof(char);
	cds.lpData = (void*)data;


	RegisterWindowMessageA(data);

	SendMessage(HWND_BROADCAST, WM_COPYDATA, 0, (LPARAM)&cds);
}

std::string sieveOfEratosthenes(int n)
{ 
	bool *prime = new bool[n + 1];
	memset(prime, true, sizeof(bool) * (n + 1));

	for (int p = 2; p*p <= n; p++)
	{
		bool isPrime = prime[p];
		sendMessageToAnotherApp("{\"log\": \"Wykreslam teraz wielokrotnosci liczby "+ std::to_string(p) + " ...\"}");
		if (prime[p] == true)
		{
			for (int i = p * 2; i <= n; i += p) {
				prime[i] = false;
			}
		}
	}

	
	std::string result = "";
	for (int p = 2; p <= n; p++) {
		if (prime[p]) {
			result += std::to_string(p) + "; ";
		}
	}

	return result;
}

void processInput(int n) {
	auto result = "Liczby pierwsze to: " + sieveOfEratosthenes(n);
	std::wstring result2 = std::wstring(result.begin(), result.end());
	auto result3 = result2.c_str();
	sendMessageToAnotherApp("{\"result\": \"" + result + "\"}");
	SetWindowText(label, result3);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GRAPHICAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICAPP));

    MSG msg;

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


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GRAPHICAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 500, 230, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   button = CreateWindowEx(0, L"BUTTON", L"Oblicz!", WS_CHILD | WS_VISIBLE,
	   50, 80, 150, 30, hWnd, NULL, hInstance, NULL);
   label = CreateWindowEx(0, L"STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 120, 350, 20, hWnd, NULL, hInstance, NULL);
   firstNum = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 50, 150, 20,
	   hWnd, NULL, hInstance, NULL);

   SetWindowText(label, L"Liczby pierwsze to:");

   LPWSTR *szArglist;
   int nArgs;
   int i;

   szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

   if (szArglist[1] != NULL) {
	   SetWindowText(firstNum, szArglist[1]);
	   int inputValue = LPWSTRToNumber(szArglist[1]);
	   processInput(inputValue);
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
			if ((HWND)lParam == button) {
				DWORD firstNumLen = GetWindowTextLength(firstNum);
				LPWSTR firstNumValue = (LPWSTR)GlobalAlloc(GPTR, firstNumLen + 1);
				GetWindowText(firstNum, firstNumValue, firstNumLen + 1);

				int firstNumInt = LPWSTRToNumber(firstNumValue);

				processInput(firstNumInt);
			}
            int wmId = LOWORD(wParam);
            // Analizuj zaznaczenia menu:
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
