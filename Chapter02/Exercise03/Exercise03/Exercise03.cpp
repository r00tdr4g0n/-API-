#include "framework.h"
#include "Exercise03.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WCHAR szWindowClass[] = L"WindowClass Name";
WCHAR szTitle[] = L"Chapter02 Exerciese03";

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
    PAINTSTRUCT ps = { 0, };
    HDC hdc = NULL;
    static TCHAR str[10][100] = { 0, };
    static int line = 0;
    static int pos = 0;

    switch (message) {
    case WM_CREATE:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        for (int i = 0; i <= line; i++) {
            TextOut(hdc, 0, i * 20, str[i], _tcslen(str[i]));
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_CHAR:
        if (wParam == VK_BACK) {
            if (pos > 0) {
                pos--;
                str[line][pos] = NULL;
            }
            else {
                if (line > 0) {
                    line--;
                    pos = _tcslen(str[line]);
                }
            }
        }
        else if (wParam == VK_RETURN) {
            if (line < 9) {
                line++;
                pos = 0;
            }
        }
        else {
            if (line < 9 && pos == 99) {
                line++;
                pos = 0;
            }

            if (line <= 9 && pos < 99) {
                str[line][pos++] = wParam;
                str[line][pos] = NULL;
            }
        }

        InvalidateRgn(hWnd, NULL, TRUE);
        
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}