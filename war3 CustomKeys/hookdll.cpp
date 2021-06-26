#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <Commctrl.h>
#include <stdio.h>
#include "hookdll.h"
//#include <tchar.h>

// #pragma data_seg(".aaa")
// //static int apmRecorder ;
// int apmRecorder = 10;
// DWORD consolePID = 0;
// #pragma data_seg()
// #pragma comment(linker, "/section:.aaa,rws")

VOID CALLBACK MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
VOID updateToolTip();

int apmRecorder __attribute__((section("shared"), shared)) = 0;
BOOL musicFlag __attribute__((section("shared"), shared)) = FALSE;
DWORD consolePID __attribute__((section("shared"), shared)) = 0;
DWORD consoleTID __attribute__((section("shared"), shared)) = 0;
HHOOK hhook_Keyboard __attribute__((section("shared"), shared)) = NULL;
HHOOK hhook_Mouse __attribute__((section("shared"), shared)) = NULL;
HWND hWnd_hooked __attribute__((section("shared"), shared)) = NULL;
HWND hWnd_tip __attribute__((section("shared"), shared)) = NULL;

// HHOOK hhook_Keyboard, hhook_Mouse;
// HWND hWnd_tip;
int apmBGM;
UINT uTimer;
TOOLINFO ti;
FILE *file;

WCHAR buf[128];

const WCHAR *music[3] = {L"C:\\Users\\Administrator\\Videos\\MagicSword.mp3",
                         L"C:\\Users\\Administrator\\Videos\\NineSwords.mp3",
                         L"C:\\Users\\Administrator\\Videos\\RedFlower.mp3"};

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
    DWORD PID = GetCurrentProcessId();
    DWORD TID = GetCurrentThreadId();

    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        if (Reserved)
        {
            printf("\n[  log  ]static loads");
            consolePID = PID;
            consoleTID = TID;
        }
        else
        {
            AttachConsole(consolePID);
            freopen("conout$", "w", stdout);
            printf("\n[  log  ]dynamic loads");
            // =============================
            uTimer = SetTimer(NULL, NULL, 6000, (TIMERPROC)MyTimerProc); // timer callback
            printf("\n[  log  ]timer(%d) start...", uTimer);
            // =============================
            hWnd_tip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                      TTS_NOPREFIX | TTS_ALWAYSTIP,
                                      0, 0, 0, 0, NULL, NULL, NULL, NULL);

            ti = {0};
            // ti.cbSize = sizeof(ti);
            ti.cbSize = TTTOOLINFOW_V2_SIZE;
            ti.uFlags = TTF_TRACK;
            ti.lpszText = (WCHAR *)L"This is the initial tooltip.";

            SendMessage(hWnd_tip, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
            SendMessage(hWnd_tip, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(900, 50));
            SendMessage(hWnd_tip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
            printf("\n[  log  ]tooltip(%d) create...", hWnd_tip);
            // =============================
            fopen_s(&file, "C:\\Users\\Administrator\\Desktop\\Test.txt", "a+");
            SYSTEMTIME st, lt;
            GetLocalTime(&lt);
            fprintf(file, "\n%d:%d:%d %d", lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
            printf("\n[  log  ]file(%d) open...", file);
        }
        printf("\n[  log  ]PROCESS_ATTACH %d(%d). <<<---", PID, TID);
        break;
    case DLL_PROCESS_DETACH:
        printf("\n[  log  ]PROCESS_DETACH %d(%d). --->>>", PID, TID);
        if (Reserved)
        {
            printf("\n[  log  ]static frees");
        }
        else
        {
            printf("\n[  log  ]dynamic frees");
        }
        unhook();
        fclose(file);
        DestroyWindow(hWnd_tip);
        KillTimer(NULL, uTimer);
        FreeConsole();
        break;
    }

    return TRUE;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    static BOOL imeFlag = FALSE;
    if (VK_RETURN == wParam && (lParam & 0x80000000))
    {
        imeFlag = !imeFlag;
        // printf("\n[  log  ] (%d)", imeFlag);
        return CallNextHookEx(hhook_Keyboard, nCode, wParam, lParam);
    }
    if (imeFlag)
        return CallNextHookEx(hhook_Keyboard, nCode, wParam, lParam);

    if (lParam & 0x80000000) // 当前按键的 transition state (释放按键为1,按下为0)
    {
        updateToolTip();
        if ('S' == wParam || 'Q' == wParam)
            return 1;
    }
    else
    {
        if ('S' == wParam)
        {
            if (lParam & 0x20000000) // alt键按下
            {
                lParam = lParam ^ 0x20000000;
                PostMessage(hWnd_hooked, WM_KEYUP, VK_MENU, lParam);
                PostMessage(hWnd_hooked, WM_KEYDOWN, wParam, lParam);
                PostMessage(hWnd_hooked, WM_KEYUP, wParam, lParam);
                // printf("\n[  log  ]alt + s");

                return 1;
            }
            else
            {
                PostMessage(hWnd_hooked, WM_KEYDOWN, VK_NUMPAD5, lParam);
                PostMessage(hWnd_hooked, WM_KEYUP, VK_NUMPAD5, lParam);
                // printf("\n[  log  ]s");

                return 1;
            }
        }
        if ('Q' == wParam)
        {
            if (lParam & 0x20000000) // alt键按下
            {
                lParam = lParam ^ 0x20000000;
                PostMessage(hWnd_hooked, WM_KEYUP, VK_MENU, lParam);
                PostMessage(hWnd_hooked, WM_KEYDOWN, wParam, lParam);
                PostMessage(hWnd_hooked, WM_KEYUP, wParam, lParam);
                return 1;
            }
            else
            {
                PostMessage(hWnd_hooked, WM_KEYDOWN, VK_NUMPAD7, lParam);
                PostMessage(hWnd_hooked, WM_KEYUP, VK_NUMPAD7, lParam);
                return 1;
            }
        }
    }

    return CallNextHookEx(hhook_Keyboard, nCode, wParam, lParam);
}
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (WM_LBUTTONUP == wParam || WM_RBUTTONUP == wParam)
    {
        updateToolTip();
    }
    return CallNextHookEx(hhook_Mouse, nCode, wParam, lParam);
}

DWORD sethook()
{
    DWORD procID;
    DWORD threadID;

    hWnd_hooked = FindWindow(L"OsWindow", L"Warcraft III");
    if (!hWnd_hooked)
    {
        return 0;
    }

    threadID = GetWindowThreadProcessId(hWnd_hooked, &procID);

    hhook_Keyboard = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc,
                                      GetModuleHandle(L"hookdll"), threadID);
    hhook_Mouse = SetWindowsHookEx(WH_MOUSE, MouseProc,
                                   GetModuleHandle(L"hookdll"), threadID);

    return procID;
}
void unhook()
{
    UnhookWindowsHookEx(hhook_Keyboard);
    UnhookWindowsHookEx(hhook_Mouse);
}

VOID CALLBACK MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
    fprintf(file, "\n%d %d", apmRecorder, apmBGM);

    static bool waitPlay = FALSE;
    BOOL startPlay = FALSE;
    if (apmRecorder < apmBGM)
    {
        waitPlay = TRUE;
        startPlay = FALSE;
    }
    else
    {
        startPlay = TRUE;
    }
    // fprintf(file, " %d %d", waitPlay, startPlay);

    // easy
    // apmBGM = 0.85 * apmBGM + 0.18 * apmRecorder;
    // apmRecorder = -0.05 * apmBGM + 0.96 * apmRecorder;
    // hard
    apmBGM = 0.88 * apmBGM + 0.15 * apmRecorder;
    apmRecorder = -0.04 * apmBGM + 0.95 * apmRecorder;

    if (!musicFlag || !startPlay || !waitPlay)
        return;

    waitPlay = FALSE;

    mciSendString(L"status currentMusic mode", buf, _countof(buf), NULL);
    printf("\n[  log  ] music status: %ls", buf);
    if (!wcscmp(buf, L"playing")) // The sound is now playing.|| wcscmp(buf, L"")
    {
        printf("\n[  log  ] music is playing, return");
        return;
    }

    mciSendString(L"close currentMusic", NULL, 0, NULL);
    wsprintf(buf, L"open \"%s\" alias currentMusic", music[rand() % 3]);
    mciSendString(buf, NULL, 0, NULL);
    mciSendString(L"play currentMusic", NULL, 0, NULL);
}

VOID updateToolTip()
{
    wsprintf(buf, L"apm:  %d / %d", ++apmRecorder, apmBGM);
    ti.lpszText = buf;
    SendMessage(hWnd_tip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
    //SendMessage(hWnd_tip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
}

//   if ('S' == wParam)
//     {
//         return 1;

//         if (lParam & 0x20000000) // alt键按下
//         {
//             lParam = lParam ^ 0x20000000;
//             PostMessage(hWnd_hooked, WM_KEYUP, wParam, lParam);
//             printf("\n[  log  ]alt + s release");

//             return 1;
//         }
//         else
//         {
//             PostMessage(hWnd_hooked, WM_KEYUP, VK_NUMPAD5, lParam);
//             printf("\n[  log  ]VK_NUMPAD5 release");

//             return 1;
//         }
//     }
//     if ('Q' == wParam)
//     {
//         return 1;

//         if (lParam & 0x20000000) // alt键按下
//         {
//             lParam = lParam ^ 0x20000000;
//             PostMessage(hWnd_hooked, WM_KEYUP, wParam, lParam);
//             printf("\n[  log  ]alt + q release");
//             return 1;
//         }
//         else
//         {
//             PostMessage(hWnd_hooked, WM_KEYUP, VK_NUMPAD7, lParam);
//             printf("\n[  log  ]VK_NUMPAD7 release");
//             return 1;
//         }
//     }