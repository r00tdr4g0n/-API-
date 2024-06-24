#include "framework.h"
#include "Exercise10.h"
#include <time.h>

#define RADIUS 10
#define MAP_X_SIZE 30
#define MAP_Y_SIZE 30
#define FRUIT_CNT 5
#define MAX_SNAKE_LEN 200

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WCHAR szWindowClass[] = L"WindowClass Name";
WCHAR szTitle[] = L"Window Title Name";

typedef struct POS {
    int x;
    int y;
} POS;

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

void DrawMap(HDC a_hdc)
{
    for (int i = 0; i < MAP_X_SIZE; i++) {
        Rectangle(a_hdc, i * 20, 0, i * 20 + RADIUS * 2, RADIUS * 2);
    }
    
    for (int i = 1; i < MAP_Y_SIZE - 1; i++) {
        Rectangle(a_hdc, 0, i * (RADIUS * 2), RADIUS * 2, i * (RADIUS * 2) + (RADIUS * 2));
        Rectangle(a_hdc, (MAP_X_SIZE - 1) * (RADIUS * 2), i * (RADIUS * 2), (MAP_X_SIZE - 1) * (RADIUS * 2) + (RADIUS * 2), i * (RADIUS * 2) + (RADIUS * 2));
    }

    for (int i = 0; i < MAP_X_SIZE; i++) {
        Rectangle(a_hdc, i * 20, (MAP_Y_SIZE - 1) * (RADIUS * 2), i * 20 + RADIUS * 2, (MAP_Y_SIZE - 1) * (RADIUS * 2) + RADIUS * 2);
    }
}

void DrawFruit(HDC a_hdc, POS* a_fruit, UINT a_size)
{

    SelectObject(a_hdc, GetStockObject(BLACK_BRUSH));

    for (int i = 0; i < a_size; i++) {
        Ellipse(a_hdc, 
            a_fruit[i].x * RADIUS * 2, a_fruit[i].y * RADIUS * 2,
            a_fruit[i].x * RADIUS * 2 + RADIUS * 2, a_fruit[i].y * RADIUS * 2 + RADIUS * 2
        );
    }

    SelectObject(a_hdc, GetStockObject(WHITE_BRUSH));
}

void DrawSnake(HDC a_hdc, POS* a_snake, UINT a_len)
{
    HBRUSH redBr = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH blueBr = CreateSolidBrush(RGB(0, 0, 255));
    HBRUSH oldBr;

    oldBr = (HBRUSH)SelectObject(a_hdc, redBr);

    Ellipse(a_hdc,
        a_snake[0].x * RADIUS * 2, a_snake[0].y * RADIUS * 2,
        a_snake[0].x * RADIUS * 2 + RADIUS * 2, a_snake[0].y * RADIUS * 2 + RADIUS * 2
    );

    oldBr = (HBRUSH)SelectObject(a_hdc, blueBr);

    for (int i = 1; i < a_len; i++) {
        Ellipse(a_hdc,
            a_snake[i].x * RADIUS * 2, a_snake[i].y * RADIUS * 2,
            a_snake[i].x * RADIUS * 2 + RADIUS * 2, a_snake[i].y * RADIUS * 2 + RADIUS * 2
        );
    }

    SelectObject(a_hdc, oldBr);

    DeleteObject(redBr);
    DeleteObject(blueBr);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static int x, y;
    static POS fruit[FRUIT_CNT];
    static POS snake[MAX_SNAKE_LEN];
    static UINT uiLen;
    static bool bStart;

    switch (message) {
    case WM_CREATE:
        bStart = false;

        x = 20;
        y = 20;

        snake[0] = { MAP_X_SIZE / 2 + 1, MAP_Y_SIZE / 2 };
        snake[1] = { MAP_X_SIZE / 2, MAP_Y_SIZE / 2 };
        snake[2] = { MAP_X_SIZE / 2 - 1, MAP_Y_SIZE / 2 };
        snake[3] = { MAP_X_SIZE / 2 - 2, MAP_Y_SIZE / 2 };

        uiLen = 4;

        srand((UINT)time(NULL));

        for (int i = 0; i < FRUIT_CNT; i++) {
            fruit[i].x = (rand() % (MAP_X_SIZE - 2)) + 1;
            fruit[i].y = (rand() % (MAP_Y_SIZE - 2)) + 1;

            for (int j = 0; j < i; j++) {
                if (fruit[i].x == fruit[j].x && fruit[i].y == fruit[j].y) {
                    i--;
                }
            }
        }

        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        DrawMap(hdc);
        DrawFruit(hdc, fruit, FRUIT_CNT);
        DrawSnake(hdc, snake, uiLen);

        EndPaint(hWnd, &ps);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
            if (!bStart) {
                bStart = true;
                SetTimer(hWnd, 1, 500, NULL);
            }
            else {
                bStart = false;
                KillTimer(hWnd, 1);
            }

            break;
        default:
            break;
        }
        InvalidateRgn(hWnd, NULL, TRUE);
        break;
    case WM_TIMER:
        if (bStart) {
            for (int i = 0; i < uiLen - 1; i++) {
                snake[uiLen - 1 - i].x = snake[uiLen - 2 - i].x;
                snake[uiLen - 1 - i].y = snake[uiLen - 2 - i].y;
            }

            snake[0].x++;

            InvalidateRgn(hWnd, NULL, TRUE);
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}