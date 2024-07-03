#include "framework.h"
#include "Exercise11.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WCHAR szWindowClass[] = L"WindowClass Name";
WCHAR szTitle[] = L"Window Title Name";

#define SX 10
#define SY 10
#define ROW 4
#define COL 8
#define W 80
#define H 80

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
    static BYTE circles[ROW][COL] = { 0, };
    int x, y;

    switch (message) {
    case WM_CREATE:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                Rectangle(hdc, 
                    SX + j * W, 
                    SY + i * H, 
                    SX + j * W + W + 1, 
                    SY + i * H + H + 1);

                if (circles[i][j]) {
                    Ellipse(hdc, 
                        SX + j * W + 1,
                        SY + i * H + 1,
                        SX + j * W + W,
                        SY + i * H + H);
                }
            }
        }

        EndPaint(hWnd, &ps);

        break;
    case WM_LBUTTONDOWN:
        x = lParam & 0x0000ffff;
        y = (lParam & 0xffff0000) >> 16;

        circles[y / H % ROW][x / W % COL] = 1;

        InvalidateRgn(hWnd, NULL, FALSE);

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}