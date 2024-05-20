#include "framework.h"
#include "Exercise08.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WCHAR szWindowClass[] = L"WindowClass Name";
WCHAR szTitle[] = L"Chapter02 Exercise08";

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
    static WCHAR str[5][100] = { 0, };
    static int line = 0;
    static int idx = 0;
    static SIZE size = { 0, };
    HDC hdc = NULL;
    POINT pos = {100, 100};
    RECT r = { pos.x, pos.y, pos.x + 200, pos.y + 100 };
    PAINTSTRUCT ps{ 0, };


    switch (message) {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        if (line < 5) {
            Rectangle(hdc, r.left, r.top, r.right, r.bottom);    
            SetCaretPos(pos.x + size.cx, pos.y + 20 * line);
        }
        else {
            HideCaret(hWnd);
        }

        for (int i = 0; i <= line; i++) {
            TextOut(hdc, pos.x, pos.y + i * 20, str[i], wcslen(str[i]));
        }

        EndPaint(hWnd, &ps);
        break;
    case WM_CHAR:
        hdc = GetDC(hWnd);
        
        if (wParam == VK_RETURN) {
            if (line < 5) {
                line++;
                idx = 0;
                size.cx = 0;
            }
        }
        else {
            if (line < 5) {
                str[line][idx++] = wParam;
                str[line][idx] = NULL;
                GetTextExtentPoint(hdc, str[line], wcslen(str[line]), &size);

                if (size.cx + 15 >= 200) {
                    line++;
                    idx = 0;
                    size.cx = 0;
                }
            }
        }
        
        ReleaseDC(hWnd, hdc);

        InvalidateRect(hWnd, &r, TRUE);
        break;
    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}