#include "framework.h"
#include "Exercise07.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WCHAR szWindowClass[] = L"WindowClass Name";
WCHAR szTitle[] = L"Chapter02 Exercise07";

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
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 600,
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
    static int width = 80;
    static int heigth = 160;

    HDC hdc = NULL;
    PAINTSTRUCT ps = { 0, };
    static HBRUSH brush = NULL;
    HBRUSH oldBrush = NULL;
    RECT r = { 0, };

    switch (message) {
    case WM_CREATE:
        brush = CreateSolidBrush(RGB(255, 0, 0));
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        r.left = 160;
        r.top = 30;
        r.right = 160 + width;
        r.bottom = 30 + heigth;
        Rectangle(hdc, r.left, r.top, r.right, r.bottom);
        DrawText(hdc, L"위쪽", wcslen(L"위쪽"), &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

        r.left = 80;
        r.top += 160;
        r.right = r.left + 80;
        r.bottom = r.top + 160;
        Rectangle(hdc, r.left, r.top, r.right, r.bottom);
        DrawText(hdc, L"왼쪽", wcslen(L"왼쪽"), &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

        r.left += 160;
        r.right = r.left + 80;
        Rectangle(hdc, r.left, r.top, r.right, r.bottom);
        DrawText(hdc, L"오른쪽", wcslen(L"오른쪽"), &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

        r.left = 160;
        r.top = 30 + 160 * 2;
        r.right = r.left + 80;
        r.bottom = r.top + 160;
        Rectangle(hdc, r.left, r.top, r.right, r.bottom);
        DrawText(hdc, L"아래쪽", wcslen(L"아래쪽"), &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

        EndPaint(hWnd, &ps);
        break;
    case WM_KEYDOWN:
        hdc = GetDC(hWnd);

        switch (wParam) {
        case VK_LEFT:
            r.left = 80;
            r.top = 160 + 30;
            r.right = r.left + 80;
            r.bottom = r.top + 160;
            break;
        case VK_UP:
            r.left = 160;
            r.top = 30;
            r.right = 160 + width;
            r.bottom = 30 + heigth;
            break;
        case VK_RIGHT:
            r.left = 240;
            r.top = 160 + 30;
            r.right = r.left + 80;
            r.bottom = r.top + 160;
            break;
        case VK_DOWN:
            r.left = 160;
            r.top = 30 + 160 * 2;
            r.right = r.left + 80;
            r.bottom = r.top + 160;
            break;
        default:
            break;
        }
        
        oldBrush = (HBRUSH)SelectObject(hdc, brush);
        Rectangle(hdc, r.left, r.top, r.right, r.bottom);

        SelectObject(hdc, oldBrush);
        ReleaseDC(hWnd, hdc);

        break;

    case WM_KEYUP:
        switch (wParam) {
        case VK_LEFT:
        case VK_UP:
        case VK_RIGHT:
        case VK_DOWN:
            InvalidateRgn(hWnd, NULL, TRUE);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}