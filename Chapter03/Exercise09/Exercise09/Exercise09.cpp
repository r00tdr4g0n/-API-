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
    int crash;
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

void CheckCrash(OBS& obs, OBS& player)
{
    int d1 = (obs.x - player.x) * (obs.x - player.x);
    int d2 = (obs.y - player.y) * (obs.y - player.y);

    if ((int)sqrt(d1 + d2) < (obs.r + player.r)) {
        obs.crash = 1;
    }
}

void CheckCrash(OBS* obs, OBS& player)
{
    for (int i = 0; i < OBS_CNT; i++) {
        CheckCrash(obs[i], player);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static OBS obs[OBS_CNT] = { 0, };
    static OBS player = { 0, };
    static HBRUSH green, red, old;
    static bool clicked;
    static bool gameover;
    static RECT r = { 0, };

    switch (message) {
    case WM_CREATE:
    {
        green = CreateSolidBrush(RGB(0, 255, 0));
        red = CreateSolidBrush(RGB(255, 0, 0));
        clicked = false;
        gameover = false;

        srand((UINT)time(NULL));
        GetClientRect(hWnd, &r);

        int w = r.right - r.left;
        int h = r.bottom - r.top;

        player = { 0, 0, 50, -1 };

        OBS pos = { 0, };
        for (int i = 0; i < OBS_CNT; i++) {
            pos = { rand() % w, rand() % h , (rand() % MAX_RAD) + MIN_RAD, 0 };

            if (CheckDistance(player, pos) && CheckObs(obs, pos, i)) {
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

        for (int i = 0; i < OBS_CNT; i++) {
            if (obs[i].crash) 
                old = (HBRUSH)SelectObject(hdc, red);
            else 
                old = (HBRUSH)SelectObject(hdc, green);

            Ellipse(hdc, 
                obs[i].x - obs[i].r, obs[i].y - obs[i].r, 
                obs[i].x + obs[i].r, obs[i].y + obs[i].r);
            SelectObject(hdc, old);
        }

        SelectObject(hdc, GetStockObject(BLACK_BRUSH));
        Ellipse(hdc, player.x - player.r, player.y - player.r, player.x + player.r, player.y + player.r);

        if (gameover) {
            int crashCnt = 0;
            WCHAR str[128] = { 0, };

            for (int i = 0; i < OBS_CNT; i++) {
                if (obs[i].crash == 1) crashCnt++;
            }

            swprintf_s(str, L"충돌한 공의 개수는 %d입니다.", crashCnt);

            SIZE size;
            GetTextExtentPoint(hdc, str, wcslen(str), &size);
            TextOut(hdc, r.right / 2 - size.cx / 2, r.bottom /2 - size.cy / 2, str, wcslen(str));
        }

        EndPaint(hWnd, &ps);

        break;
    case WM_LBUTTONDOWN:
        clicked = true;
        SetCapture(hWnd);
        break;
    case WM_LBUTTONUP:
        clicked = false;
        ReleaseCapture();
        break;
    case WM_MOUSEMOVE:
        if (clicked) {
            player.x = LOWORD(lParam) << 16 >> 16;
            player.y = HIWORD(lParam) << 16 >> 16;

            if (player.x < 0) player.x = 0;
            if (player.y < 0) player.y = 0;
            if (player.x > r.right) player.x = r.right;
            if (player.y > r.bottom) player.y = r.bottom;

            if (player.x == r.right && player.y == r.bottom) {
                clicked = false;
                gameover = true;
            }

            CheckCrash(obs, player);

            InvalidateRgn(hWnd, NULL, TRUE);
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}