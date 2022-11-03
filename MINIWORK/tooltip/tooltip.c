#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <Commctrl.h>
#include <stdio.h>
//#include <tchar.h>

HWND hWnd_tips=NULL;
TOOLINFO ti;
SYSTEMTIME lt;

WCHAR buffer[64];
CHAR buf[64];

VOID ToolTipCreate(HWND hparent)
{
    if (!IsWindow(hWnd_tips))
    {
        // DestroyWindow(hWnd_tips);

        // hWnd_tips = CreateWindowEx(.. GetDesktopWindow(), NULL, GetModuleHandle(NULL), NULL);
        hWnd_tips = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                   TTS_NOPREFIX | TTS_ALWAYSTIP,
                                   0, 0, 0, 0, NULL, NULL, NULL, NULL);
        printf("\n[  log  ] Create tooltip : 0x%X", hWnd_tips);

        // ti = {0};
        // ti.cbSize = sizeof(TTTOOLINFO);
        // ti.cbSize = sizeof(ti);
        // ti.cbSize = sizeof(ti) - sizeof(void *); // Fixed for v1.0.36.05:
        // Tooltips fail to work on Windows 2000
        // unless the size for the *lpReserved member in _WIN32_WINNT 0x0501 is omitted.
        ti.cbSize = TTTOOLINFOW_V2_SIZE;
        ti.uFlags = TTF_TRACK;
        ti.lpszText = (WCHAR *)L"This is a new tooltip.";

        // BOOL isFlag = SendMessage(hWnd_tips, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
        BOOL isFlag = SendMessage(hWnd_tips, TTM_ADDTOOL, 0, (LPARAM)&ti);
        printf("\n[  log  ] TTM_ADDTOOL message whether suceessed : (%d)", isFlag);

        // SendMessage(hWnd_tip, TTM_ACTIVATE, TRUE, 0);
        // SendMessage(hWnd_tip, TTM_ACTIVATE, false, 0);
        SendMessage(hWnd_tips, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(800, 50));
        SendMessage(hWnd_tips, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
    }
    else
    {
        printf("\n[  log  ] Destroy Tooltip : 0x%X", hWnd_tips);
        DestroyWindow(hWnd_tips);
    }
}

VOID ToolTipUpdate()
{
    printf("\n[  log  ] updated tooltip (0x%x).", hWnd_tips);
    GetLocalTime(&lt);
    wsprintf(buffer, L"NOW : %02d:%02d:%02d %03d", lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
    ti.lpszText = buffer;
    SendMessage(hWnd_tips, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
    SendMessage(hWnd_tips, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

    // ti.lpszText = pText;
    // SendMessage(hWnd_tip, TTM_SETTOOLINFO, 0, (LPARAM)(LPTOOLINFO)&ti);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        ToolTipCreate(hwnd);
        break;

    case WM_MOVE:
        printf("\n[  log  ] WM_MOVE message, updated tooltip(0x%x)...", hWnd_tips);
        ToolTipUpdate();

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main()
{
    system("chcp 65001");

    char codestr[64];
    // getchar();

    printf("\nEnter interactive model!");

    while (strcmp(codestr, "exit") != 0)
    {
        printf("\n\n -------- -------  MENU  菜单选项 -------- --------");
        printf("\n 命令列表: cmd, update, win, exit");
        printf("\n      please enter command:  ");

        scanf("%s", codestr);
        if (strcmp(codestr, "cmd") == 0)
        {
            printf("\n 命令行直接创建.");
            ToolTipCreate(NULL);
        }
        else if (strcmp(codestr, "update") == 0)
        {
            wsprintf(buffer, L"rand number = %d", rand());
            ToolTipUpdate();
        }
        if (strcmp(codestr, "win") == 0)
        {
            printf("\n 窗口间接创建, 程序卡在消息循环(关闭窗口解除阻塞)");

            WNDCLASS wc = {0};
            wc.lpszClassName = L"TooltipTest";
            wc.hInstance = GetModuleHandle(NULL);
            wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
            wc.lpfnWndProc = WndProc;
            RegisterClass(&wc);

            CreateWindow(wc.lpszClassName, L"Tooltip", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                         100, 100, 200, 150, 0, 0, GetModuleHandle(NULL), 0);

            MSG msg;
            while (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        printf("\n  command been executed :  %s .", codestr);
    }
    printf("\n # # # done .\n # # # .\n");
    Sleep(2000);
    return 0;
}