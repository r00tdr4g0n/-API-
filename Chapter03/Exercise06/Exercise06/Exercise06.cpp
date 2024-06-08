#include "framework.h"
#include "Exercise06.h"

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
    static int catX, catY;
    static int mouseX, mouseY;
    static bool clicked;
    static WCHAR cat[32] = L"고양이";
    static WCHAR mouse[32] = L"쥐";

    switch (message) {
    case WM_CREATE:
        catX = 100;
        catY = 100;
        clicked = false;
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        TextOut(hdc, catX, catY, cat, wcslen(cat));

        if (clicked) {
            TextOut(hdc, mouseX, mouseY, mouse, wcslen(mouse));
        }

        EndPaint(hWnd, &ps);
        break;
    case WM_LBUTTONDOWN:
        mouseX = LOWORD(lParam);
        mouseY = HIWORD(lParam);

        clicked = true;
        SetTimer(hWnd, 1, 10, NULL);

        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_LBUTTONUP:
        clicked = false;
        KillTimer(hWnd, 1);

        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_MOUSEMOVE:
        mouseX = LOWORD(lParam);
        mouseY = HIWORD(lParam);
        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_TIMER:
        if (wParam == 1) {
            if (catX - mouseX > 0) {
                catX--;
            }
            else {
                catX++;
            }

            if (catY - mouseY > 0) {
                catY--;
            }
            else {
                catY++;
            }

            InvalidateRgn(hWnd, NULL, TRUE);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}