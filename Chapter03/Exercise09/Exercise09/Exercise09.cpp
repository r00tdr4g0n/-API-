#include "framework.h"
#include "Exercise09.h"
#include <time.h>
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WCHAR szWindowClass[] = L"WindowClass Name";
WCHAR szTitle[] = L"Window Title Name";

#define MAX_RAD 40
#define MIN_RAD 10
#define OBS_CNT 40

typedef struct OBS {
    int x;
    int y;
    int r;
} OBS;

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

bool CheckDistance(OBS& obs1, OBS& obs2)
{
    int d1 = (obs1.x - obs2.x) * (obs1.x - obs2.x);
    int d2 = (obs1.y - obs2.y) * (obs1.y - obs2.y);

    if ((int)sqrt(d1 + d2) < (obs1.r + obs2.r)) {
        return false;
    }

    return true;
}

bool CheckObs(OBS* obs, OBS pos, int cnt)
{
    for (int i = 0; i < cnt; i++) {
        if (!CheckDistance(obs[i], pos)) {
            return false;
        }
    }

    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static OBS obs[OBS_CNT] = { 0, };
    static HBRUSH green, red, old;

    switch (message) {
    case WM_CREATE:
    {
        RECT r = { 0, };
        green = CreateSolidBrush(RGB(0, 255, 0));
        red = CreateSolidBrush(RGB(255, 0, 0));

        srand((UINT)time(NULL));
        GetClientRect(hWnd, &r);

        int w = r.right - r.left;
        int h = r.bottom - r.top;

        obs[0] = { rand() % w, rand() % h, (rand() % MAX_RAD) + MIN_RAD };
        OBS pos = { 0, };
        for (int i = 1; i < OBS_CNT; i++) {
            pos = { rand() % w, rand() % h , (rand() % MAX_RAD) + MIN_RAD };

            if (CheckObs(obs, pos, i)) {
                obs[i] = pos;
            }
            else {
                i--;
            }
        }
        break;
    }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        old = (HBRUSH)SelectObject(hdc, green);

        for (int i = 0; i < OBS_CNT; i++) {
            Ellipse(hdc, 
                obs[i].x - obs[i].r, obs[i].y - obs[i].r, 
                obs[i].x + obs[i].r, obs[i].y + obs[i].r);
        }

        SelectObject(hdc, old);

        EndPaint(hWnd, &ps);

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}