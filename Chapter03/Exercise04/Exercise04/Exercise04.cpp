#include "framework.h"
#include "Exercise04.h"

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static HPEN red, blue, old;
    static int x, y, tX, tY;
    static RECT rectView;
    static bool timer;
    static int dir;

    switch (message) {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        rectView.right = 1400;
        rectView.bottom = 680;

        red = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
        blue = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

        timer = false;
        dir = 2;
        x = 620;
        y = 340;
        tX = x - 40;
        tY = y;

        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        Rectangle(hdc, rectView.left, rectView.top, rectView.right, rectView.bottom);
        
        old = (HPEN)::SelectObject(hdc, red);
        Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
        
        ::SelectObject(hdc, blue);
        Ellipse(hdc, tX - 20, tY - 20, tX + 20, tY + 20);

        ::SelectObject(hdc, old);

        EndPaint(hWnd, &ps);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_RETURN) {
            if (!timer) {
                timer = true;
                SetTimer(hWnd, 1, 100, NULL);
            }
            else {
                timer = false;
                KillTimer(hWnd, 1);
            }
        }
        else if (wParam == VK_LEFT) {
            dir = 0;
        }
        else if (wParam == VK_UP) {
            dir = 1;
        }
        else if (wParam == VK_RIGHT) {
            dir = 2;
        }
        else if (wParam == VK_DOWN) {
            dir = 3;
        }
        break;
    case WM_TIMER:
        if (wParam == 1) {
            switch (dir) {
            case 0:
                if (x - 60 >= rectView.left) {
                    tX = x;
                    tY = y;
                    x -= 40;
                }
                break;
            case 1:
                if (y - 60 >= rectView.top) {
                    tX = x;
                    tY = y;
                    y -= 40;
                }
                break;
            case 2:
                if (x + 60 <= rectView.right) {
                    tX = x;
                    tY = y;
                    x += 40;
                }
                break;
            case 3:
                if (y + 60 <= rectView.bottom) {
                    tX = x;
                    tY = y;
                    y += 40;
                }
                break;
            }

            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}