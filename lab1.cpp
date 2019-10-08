// lab1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#pragma comment(lib, "Msimg32.lib")
#include "lab1.h"


#define MAX_LOADSTRING 100
#define TIMER_ELAPSE 50
#define SHAPE_WIDTH 150
#define SHAPE_HEIGHT 90
#define DELTA_KEY_DOWN 10
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HPEN hpen1 = CreatePen(PS_SOLID, 2, RGB(35, 140, 0)),
hpen2 = CreatePen(PS_SOLID, 2, RGB(50, 40, 30));
HBRUSH hbrush1 = CreateSolidBrush(RGB(35, 140, 0)),
hbrush2 = CreateSolidBrush(RGB(0, 255, 0));

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW +11);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static BOOLEAN decX = false, decY = false;
	static int x = 0, y = 0,oldx=0,oldy=0, winWidth, winHeight;
	static int caption, menu, border;
	static HDC memBit, hdc;
	static HBITMAP hBitmap;
	static BITMAP bm;
	static BOOLEAN Stop = true;
	
	
    switch (message)
    {
	case WM_CREATE:
		SetTimer(hWnd, 1, TIMER_ELAPSE, NULL);		
		hBitmap = (HBITMAP)LoadImage(NULL, _T("vashynko.bmp"), IMAGE_BITMAP,0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBitmap, sizeof(bm), &bm);
		hdc = GetDC(hWnd);
		memBit = CreateCompatibleDC(hdc);
		SelectObject(memBit, hBitmap);		
		ReleaseDC(hWnd, hdc);

	//	SelectObject(memBit, hpen2);
	//	SelectObject(memBit, hbrush2);


		break;

	case WM_SIZE:
	{
		winWidth = LOWORD(lParam);
		winHeight = HIWORD(lParam);
		break;
	}
	case WM_TIMER:
	{
		
		oldx = x;
		oldy = y;
		RECT rec;
		rec.left = x;
		rec.right = x + bm.bmWidth;
		rec.bottom = y + bm.bmHeight;
		rec.top = y;
	if (Stop) 
	{
	 if (decX) x-=2;
	 else x+=2;
	 if (decY) y-=2;
	 else y+=2;
	}		
	
	if (x + bm.bmWidth >= winWidth) x = winWidth - bm.bmWidth;
	if (x < 0) x = 0;
	if (y + bm.bmHeight >= winHeight) y = winHeight - bm.bmHeight;
	if (y < 0) y = 0;
	if (x + bm.bmWidth >= winWidth) decX = true;
	if (x == 0) decX = false;
	if (y + bm.bmHeight >= winHeight) decY = true;
	if (y == 0) decY = false;



	//TransparentBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, memBit, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));


	     FillRect(memBit, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 11));

	     InvalidateRect(hWnd, &rec, TRUE);
		break;
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
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
	case WM_MOUSEWHEEL:
	{
		int delta = (short)HIWORD(wParam);
		if (LOWORD(wParam) == MK_SHIFT) {
			x += delta;
			
			x+=delta;
		}
		else {
			y += delta;
			
			y+=delta;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYDOWN:
		{
			switch (wParam) {
			case VK_SPACE:
				Stop = !Stop;
				break;
			case VK_TAB:				
				Stop = !Stop;
				break;
			case VK_UP:
				y -= DELTA_KEY_DOWN;
				decY = true;
				break;
			case VK_LEFT:
				x -= DELTA_KEY_DOWN;
				decX = true;
				break;
			case VK_DOWN:
				y += DELTA_KEY_DOWN;
				decY = false;
				break;
			case VK_RIGHT:
				x += DELTA_KEY_DOWN;
				decX = false;
				break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
	       
		}
    case WM_PAINT:
        {
		hdc = BeginPaint(hWnd, &ps);
		//BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, memBit, 0, 0, SRCCOPY);
		        																																	
		TransparentBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, memBit, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));		   						
		EndPaint(hWnd, &ps);
        }
        break;	

    case WM_DESTROY:
		DeleteObject(memBit);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
