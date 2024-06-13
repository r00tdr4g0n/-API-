#include "framework.h"
#include "Exercise08.h"
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
    static int startX, startY, oldX, oldY, gapX, gapY;
    static float dx, dy, x, y;
    int endX, endY;
    static bool clicked, draw;

    switch (message) {
    case WM_CREATE:
        draw = false;
        clicked = false;
        startX = startY = oldX = oldY = 20;
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        if (draw) {
            Ellipse(hdc, (int)x - 20, (int)y - 20, (int)x + 20, (int)y + 20);
        }

        EndPaint(hWnd, &ps);

        break;
    case WM_LBUTTONDOWN:
        draw = false;
        clicked = true;

        oldX = startX;
        oldY = startY;
        startX = LOWORD(lParam);
        startY = HIWORD(lParam);
        oldX = startX;
        oldY = startY;

        InvalidateRgn(hWnd, NULL, FALSE);

        break;
    case WM_LBUTTONUP:
        draw = true;
        clicked = false;
        
        x = (float)startX;
        y = (float)startY;
        gapX = abs(oldX - startX);
        gapY = abs(oldY - startY);

        if (gapX > gapY) {
            if (oldX > startX) {
                dx = 1.0;
            }
            else {
                dx = -1.0;
            }

            if (oldY > startY) {
                dy = (gapY / (float)gapX);
            }
            else {
                dy = -(gapY / (float)gapX);
            }
        }
        else {
            if (oldY > startY) {
                dy = 1.0;
            }
            else {
                dy = -1.0;
            }

            if (oldX > startX) {
                dx = (gapX / (float)gapY);
            }
            else {
                dx = -(gapX / (float)gapY);
            }
        }

        SetTimer(hWnd, 1, 20, NULL);

        InvalidateRgn(hWnd, NULL, FALSE);

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
    case WM_TIMER:
        if (wParam == 1) {
            if ((int)x != oldX) x += dx;
            if ((int)y != oldY) y += dy;

            if ((int)x == oldX && (int)y == oldY) KillTimer(hWnd, 1);

            InvalidateRgn(hWnd, NULL, TRUE);
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}