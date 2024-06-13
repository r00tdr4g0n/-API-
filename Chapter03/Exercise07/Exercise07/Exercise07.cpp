#include "framework.h"
#include "Exercise07.h"
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WCHAR szWindowClass[] = L"WindowClass Name";
WCHAR szTitle[] = L"Window Title Name";

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassExW(&wcex);

    hWnd = CreateWindow(szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

int LengthPts(int x1, int y1, int x2, int y2)
{
    return (int)sqrt(double((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

bool InCircle(int x1, int y1, int x2, int y2)
{
    if (LengthPts(x1, y1, x2, y2) <= 20) {
        return true;
    }
    else {
        return false;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static HBRUSH red, old;
    static int x, y, startX, startY, oldX, oldY;
    int endX, endY;
    static bool clicked;

    switch (message) {
    case WM_CREATE:
        red = ::CreateSolidBrush(RGB(255, 0, 0));
        clicked = false;
        x = y = startX = startY = oldX = oldY = 20;
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        if (clicked) {
            old = (HBRUSH)::SelectObject(hdc, red);
        }

        Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);

        if (clicked) {
           ::SelectObject(hdc, old);
        }

        EndPaint(hWnd, &ps);
        
        break;
    case WM_LBUTTONDOWN:
        if (InCircle(x, y, LOWORD(lParam), HIWORD(lParam))) {
            clicked = true;
        }

        InvalidateRgn(hWnd, NULL, TRUE);
        
        break;
    case WM_LBUTTONUP:
        clicked = false;
        oldX = startX;
        oldY = startY;
        InvalidateRgn(hWnd, NULL, TRUE);
        
        break;
    case WM_MOUSEMOVE:
        hdc = GetDC(hWnd);

        if (clicked) {
            SetROP2(hdc, R2_XORPEN);
            SelectObject(hdc, GetStockObject(WHITE_PEN));
            
            endX = LOWORD(lParam);
            endY = HIWORD(lParam);

            MoveToEx(hdc, startX, startY, NULL);
            LineTo(hdc, oldX, oldY);
            MoveToEx(hdc, startX, startY, NULL);
            LineTo(hdc, endX, endY);
            
            oldX = endX;
            oldY = endY;
        }

        ReleaseDC(hWnd, hdc);

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}