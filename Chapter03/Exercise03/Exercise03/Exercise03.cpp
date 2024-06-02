#include "framework.h"
#include "Exercise03.h"

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
    static int x, y;
    static RECT rectView;
    static bool timer;
    static int dir;

    switch (message) {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        timer = false;
        dir = 2;
        x = 20;
        y = 20;
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        Ellipse(hdc, x-20, y-20, x+20, y+20);
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
                x -= 40;

                if (x - 20 < rectView.left) {
                    x += 40;
                }
                break;
            case 1:
                y -= 40;

                if (y - 20 < rectView.top) {
                    y += 40;
                }
                break;
            case 2:
                x += 40;
            
                if (x + 20 > rectView.right) {
                    x -= 40;
                }
                break;
            case 3:
                y += 40;

                if (y + 20 > rectView.bottom) {
                    y -= 40;
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