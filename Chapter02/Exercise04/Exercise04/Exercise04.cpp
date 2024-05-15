#include "framework.h"
#include "Exercise04.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WCHAR szWindowClass[] = L"WindowClass Name";
WCHAR szTitle[] = L"Chapter02 Exercise04";

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
    HDC hdc = NULL;
    PAINTSTRUCT ps = { 0, };
    static TCHAR str[1000] = { 0, };
    static int count, yPos;
    RECT rt = {0, 0, 1000, 1000};

    switch (message) {
    case WM_CREATE:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        DrawText(hdc, str, _tcslen(str), &rt, DT_TOP | DT_LEFT);

        EndPaint(hWnd, &ps);
        break;
    case WM_CHAR:
        if (wParam == VK_BACK) {
            if (count > 0) {
                if (str[count - 1] == VK_RETURN) {
                    count--;
                    str[count] = NULL;
                }

                count--;
                str[count] = NULL;
            }
            else {
                return 0;
            }
        }
        else {
            str[count++] = wParam;
        }
        str[count] = NULL;
        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}