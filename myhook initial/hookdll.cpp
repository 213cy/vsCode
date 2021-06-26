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
// //static int actionCounter ;
// int actionCounter = 10;
// DWORD consolePID = 0;
// #pragma data_seg()
// #pragma comment(linker, "/section:.aaa,rws")

VOID CALLBACK MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
// VOID dispToolTip(WCHAR *pText);

int actionCounter __attribute__((section("shared"), shared)) = 0;
DWORD consolePID __attribute__((section("shared"), shared)) = 0;
DWORD consoleTID __attribute__((section("shared"), shared)) = 0;
HHOOK hhook_Keyboard __attribute__((section("shared"), shared)) = NULL;
HHOOK hhook_Mouse __attribute__((section("shared"), shared)) = NULL;
HWND hWnd_hooked __attribute__((section("shared"), shared)) = NULL;
HWND hWnd_tips __attribute__((section("shared"), shared)) = NULL;

int actionRecorder;
// HHOOK hhook_Keyboard, hhook_Mouse;
// HWND hWnd_tips;
UINT uTimer;
TOOLINFO ti;
FILE *file;

WCHAR buffer[128];
const WCHAR *music[3] = {L"C:\\Users\\Administrator\\Videos\\MagicSword.mp3",
                         L"C:\\Users\\Administrator\\Videos\\NineSwords.mp3",
                         L"C:\\Users\\Administrator\\Videos\\RedFlower.mp3"};

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
    // AllocConsole() ;
    // HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE) ;
    // WriteConsole(hd , "hello hplonline" , sizeof("hello hplonline") , NULL , NULL );
    // CloseHandle(hd) ;
    // FreeConsole();

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

            // uTimer = SetTimer(NULL, NULL, 6000, (TIMERPROC)MyTimerProc); // timer callback
            // printf("\n[  log  ]timer(%d) start...", uTimer);

            wsprintf(buffer, L"This is the initial tooltip.");
            dispToolTip(buffer);

            fopen_s(&file, "C:\\Users\\Administrator\\Desktop\\Test.txt", "a+");
            SYSTEMTIME st, lt;
            GetLocalTime(&lt);
            fprintf(file, "\n%d:%d:%d %d", lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
        }
        printf("\n[  log  ]PROCESS_ATTACH %d(%d). <<<---", PID, TID);
        break;
    case DLL_PROCESS_DETACH:
        if (Reserved)
        {
            printf("\n[  log  ]static frees");
            unhook();
        }
        else
        {
            printf("\n[  log  ]dynamic frees");
            printf("\n[  log  ]PROCESS_DETACH %d(%d). --->>>", PID, TID);
            //printf("\n[  log  ]tooltipswindow = (%d). --->>>", hWnd_tips);
            KillTimer(NULL, uTimer);
            DestroyWindow(hWnd_tips);
            hWnd_tips = 0;
            fclose(file);
            FreeConsole();
        }
        printf("\n[  log  ]PROCESS_DETACH %d(%d). --->>>", PID, TID);
        break;
    case DLL_THREAD_ATTACH:
        printf("\n[  log  ]THREAD_ATTACH %d(%d).", PID, TID);
        break;
    case DLL_THREAD_DETACH:
        printf("\n[  log  ]THREAD_DETACH %d(%d).", PID, TID);
        break;
    }

    return TRUE;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (lParam & 0x1000000) // win键按下
        printf("\n[  log  ]扩展键");
    else
        printf("\n[  log  ]非扩展键");
    if (lParam & 0x20000000) // alt键按下
        printf("\n[  log  ]alt键按下");
    else
        printf("\n[  log  ]alt键松开");

    if (lParam & 0x40000000) // 前一个键的 transition state
        printf("\n[  log  ]前一个键松开");
    else
        printf("\n[  log  ]前一个键按下");
    if (lParam & 0x80000000) // 当前按键的 transition state (释放按键为1,按下为0)
        printf("\n[  log  ]当前按键松开");
    else
        printf("\n[  log  ]当前按键按下");
    printf("\n[  log  ]当前按键  当前按键 0x%X 当前按键  当前按键", wParam);
    return CallNextHookEx(hhook_Keyboard, nCode, wParam, lParam);
    if (lParam & 0x80000000) // 当前按键的 transition state (释放按键为1,按下为0)
    {
        printf("\n[log:%d] actionCounter = (%d)", GetTickCount(), ++actionCounter);
        wsprintf(buffer, L"actionCounter = %d", actionCounter);
        dispToolTip(buffer);

        if ('S' == wParam)
        {
            PostMessage(hWnd_hooked, WM_KEYUP, VK_NUMPAD5, lParam);
            return 1;
        }
        if ('Q' == wParam)
        {
            // printf("\n[  log  ]QQQ哈哈哈");
            OutputDebugString(L"sd---------------------------fsddsfa");
            if (lParam & 0x20000000) // alt键按下
            {
                lParam = lParam ^ 0x20000000;
                PostMessage(hWnd_hooked, WM_KEYUP, 'Q', lParam);
                return 1;
            }
            else
            {
                PostMessage(hWnd_hooked, WM_KEYUP, VK_NUMPAD7, lParam);
                return 1;
            }
        }
    }
    else
    {
        if ('S' == wParam)
        {
            PostMessage(hWnd_hooked, WM_KEYDOWN, VK_NUMPAD5, lParam);
            return 1;
        }
        if ('Q' == wParam)
        {
            // printf("\n[  log  ]QQQ哈哈哈");
            if (lParam & 0x20000000) // alt键按下
            {
                lParam = lParam ^ 0x20000000;
                PostMessage(hWnd_hooked, WM_KEYDOWN, 'Q', lParam);
                return 1;
            }
            else
            {
                PostMessage(hWnd_hooked, WM_KEYDOWN, VK_NUMPAD7, lParam);
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
        printf("\n[log:%d] actionCounter = (%d)", GetTickCount(), ++actionCounter);

        wsprintf(buffer, L"actionCounter = %d", actionCounter);
        dispToolTip(buffer);
    }
    if (WM_MBUTTONUP == wParam)
    {
        wsprintf(buffer, L"This is a new tooltip.");
        dispToolTip(buffer);
    }
    return CallNextHookEx(hhook_Mouse, nCode, wParam, lParam);
}

DWORD sethook()
{
    HWND hwObj;
    DWORD procID;
    DWORD threadID;

    hwObj = FindWindow(L"OsWindow", L"Warcraft III");
    if (hwObj)
    {
        printf("\n[  log  ]Find WAR3 window( 找到魔兽3 ).");
        threadID = GetWindowThreadProcessId(hwObj, &procID);
        hWnd_hooked = hwObj;
    }
    else
    {
        hwObj = FindWindow(L"Notepad", L"Test.txt - 记事本");
        if (hwObj)
        {
            printf("\n[  log  ]Find test.txt notepad window ( 找到测试记事本 ).");
            threadID = GetWindowThreadProcessId(hwObj, &procID);
            hWnd_hooked = FindWindowEx(hwObj, NULL, L"Edit", NULL);
        }
        else
        {
            return 0;
        }
    }
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
    WCHAR buf[128];
    printf("\n[  log  ] hwnd=(%d) | message=(%d) | idTimer=(%d) | dwTime=(%d) ",
           hwnd, message, idTimer, dwTime);
    fprintf(file, "\n%d    %d", actionRecorder, actionCounter);
    bool shouldplaymusic = actionCounter > 14 && actionRecorder + 20 < 2 * actionCounter;
    actionRecorder = 0.4 * actionRecorder + 0.6 * actionCounter;
    actionCounter = 0;
    // if (!shouldplaymusic)
    if (TRUE)
        return;

    DWORD mciError = mciSendString(L"status currentMusic mode", buf, _countof(buf), NULL);
    if (mciError)
    {
        mciGetErrorString(mciError, buf, _countof(buf));
        // MessageBox(NULL, buf, L"error", MB_OK);
        printf("\n[  log  ] {%s}(%d)", buf, mciError);
    }
    if (!wcscmp(buf, L"playing")) // The sound is now playing.| wcscmp(buf, L"")
    {
        printf("\n[  log  ] music is playing, return");
        return;
    }

    //mciSendString(L"play C:\\Users\\Administrator\\Videos\\MagicSword.mp3", NULL, 0, 0);
    //mciSendString(music[rand() % 3], NULL, 0, 0);
    mciSendString(L"close currentMusic", NULL, 0, NULL);
    wsprintf(buf, L"open \"%s\" alias currentMusic", music[rand() % 3]);
    mciSendString(buf, NULL, 0, NULL);
    mciSendString(L"play currentMusic", NULL, 0, NULL);
}

VOID dispToolTip(WCHAR *pText)
{
    if (!hWnd_tips)
    {
        // hWnd_tips = CreateWindowEx(.. GetDesktopWindow(), NULL, NULL, NULL);
        hWnd_tips = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                   TTS_NOPREFIX | TTS_ALWAYSTIP,
                                   0, 0, 0, 0, NULL, NULL, NULL, NULL);
        printf("\n[  log  ]tip(%d) created", hWnd_tips);

        ti = {0};
        // ti.cbSize = sizeof(ti);
        ti.cbSize = TTTOOLINFOW_V2_SIZE;
        ti.uFlags = TTF_TRACK;
        ti.lpszText = (WCHAR *)L"This is a new tooltip.";

        bool isFlag = SendMessage(hWnd_tips, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
        printf("\n[  log  ]TTM_ADDTOOL message whether suceessed(%d)", isFlag);

        SendMessage(hWnd_tips, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(800, 50));
        SendMessage(hWnd_tips, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
    }
    else
    {
        ti.lpszText = pText;
        SendMessage(hWnd_tips, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
        printf("\n[  log  ]updated tooltip(%d)  (0x%x)...", hWnd_tips, hWnd_tips);
        //SendMessage(hWnd_tips, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
    }
}
