#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

// #pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// #pragma comment(lib, "comctl32.lib")

HINSTANCE g_hinst;
HWND hwndTT;
TOOLINFO ti;
WCHAR buf[80];
int conuter;

void CreateMyTooltip(HWND hparent)
{
    hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                            TTS_NOPREFIX | TTS_ALWAYSTIP,
                            0, 0, 0, 0, NULL, NULL, NULL, NULL);

    // ti = {0};
    // TOOLINFO ti = {0};
    //ti.cbSize = sizeof(TTTOOLINFO);
    //*********************************************************
    // Specific settings for specific compiler options (Unicode/VC2013)
    //*********************************************************
    ti.cbSize = TTTOOLINFOW_V2_SIZE;

    // ti.cbSize = sizeof(ti);
    ti.uFlags = TTF_TRACK;
    // ti.hwnd = hparent;
    ti.lpszText = TEXT("Tooltip string");
    // GetClientRect(hparent, &ti.rect);

    if (!SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)&ti))
        MessageBox(0, TEXT("Failed: TTM_ADDTOOL"), 0, 0);

    SendMessage(hwndTT, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(400, 400));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateMyTooltip(hwnd);
        break;

    case WM_MOVE:
        printf("\n[  log  ]updated tooltip(%d)  (0x%x)...", hwndTT, hwndTT);
        printf("\n[  log  ]一些中文");

        // SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
        wsprintf(buf, L"tooltip tooltip tooltip(%d).", conuter++);
        ti.lpszText = buf;
        // SendMessage(hwndTT, TTM_SETTOOLINFO, 0, (LPARAM)(LPTOOLINFO)&ti);
        SendMessage(hwndTT, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
        SendMessage(hwndTT, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main()
{
    system("chcp 65001");
    // g_hinst = hInstance;
    g_hinst  = GetModuleHandle(NULL);

    printf("\n[  log  ]GetModuleHandle(%d)(0x%x).", GetModuleHandle(NULL), GetModuleHandle(NULL));
    // printf("\n[  log  ]hInstance(%d)(0x%x).", hInstance, hInstance);

    WNDCLASS wc = {0};
    wc.lpszClassName = TEXT("TooltipTest");
    wc.hInstance = g_hinst;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    RegisterClass(&wc);

    CreateWindow(wc.lpszClassName, TEXT("Tooltip"), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                 100, 100, 200, 150, 0, 0, g_hinst, 0);

    // CreateMyTooltip(NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}