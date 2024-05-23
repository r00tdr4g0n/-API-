#include "framework.h"
#include "Exercise09.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WCHAR szWindowClass[] = L"WindowClass Name";
WCHAR szTitle[] = L"Chapter02 Exercise09";

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
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
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
    static RECT editRect = { 200, 320, 400, 350 };
    static RECT drawRect = { 0, };
    static WCHAR str[100] = { 0, };
    static int pos = 0;
    static int command = -1;
    static bool bEnter = false;
    static bool bDraw = false;
    HDC hdc = NULL;
    SIZE size = { 0, };
    PAINTSTRUCT ps = { 0, };

    switch (message) {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        Rectangle(hdc, editRect.left, editRect.top, editRect.right, editRect.bottom);
        
        if (!bEnter) {
            TextOut(hdc, editRect.left + 5, editRect.top + 6, str, wcslen(str));
        }
        else {
            if (bDraw) {

                switch (command) {
                case 0:
                    Ellipse(hdc, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom);
                    break;
                case 1:
                    MoveToEx(hdc, drawRect.left, drawRect.top, NULL);
                    LineTo(hdc, drawRect.right, drawRect.bottom);
                    break;
                case 2:
                    Rectangle(hdc, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom);
                    break;
                default:
                    break;
                }

                TextOut(hdc, 10, editRect.top + 6, str, wcslen(str));
            }

            memset(str, 0, sizeof(str));
            pos = 0;
            bEnter = false;
        }
        
        GetTextExtentPoint(hdc, str, wcslen(str), &size);
        SetCaretPos(size.cx + editRect.left + 5, editRect.top + 7);

        EndPaint(hWnd, &ps);
        break;
    case WM_CHAR:
        if (wParam == VK_BACK) {
            if (pos > 0) {
                pos--;
                str[pos] = 0;
            }
        }
        else if (wParam == VK_RETURN) {
            WCHAR* start = 0;
            WCHAR* end = 0;
            WCHAR* p = 0;
            WCHAR* next = 0;
            int idx = 0;
            WCHAR shape[100] = { 0, };
            WCHAR tmp[100] = { 0, };
            WCHAR *rect[4] = { 0, };

            start = wcschr(str, L'(');
            end = wcschr(str, L')');
            p = str;

            bEnter = true;

            if (!start || !end || !p) {
                break;
            }

            while (p != start) {
                shape[idx++] = *p;
                p++;
            }

            p = start + 1;
            idx = 0;

            while (p && p != end) {
                tmp[idx++] = *p;
                p++;
            }

            rect[0] = wcstok_s(tmp, L",", &next);
            rect[1] = wcstok_s(NULL, L",", &next);
            rect[2] = wcstok_s(NULL, L",", &next);
            rect[3] = wcstok_s(NULL, L",", &next);

            if (!wcscmp(shape, L"Ellipse")) {
                command = 0;
            }
            else if (!wcscmp(shape, L"Line")) {
                command = 1;
            }
            else if (!wcscmp(shape, L"Rectangle")) {
                command = 2;
            }
            else {
                command = -1;
            }

            if (command != -1 && 
                rect[0] &&
                rect[1] &&
                rect[2] &&
                rect[3] &&
                wcslen(rect[0]) && 
                wcslen(rect[1]) && 
                wcslen(rect[2]) && 
                wcslen(rect[3])) {
                bDraw = true;

                drawRect.left = _wtoi(rect[0]);
                drawRect.top = _wtoi(rect[1]);
                drawRect.right = _wtoi(rect[2]);
                drawRect.bottom = _wtoi(rect[3]);
            }
        }
        else {
            str[pos++] = wParam;
            str[pos] = 0;
        }

        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}