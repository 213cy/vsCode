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

// HHOOK hhook_Keyboard, hhook_Mouse;
// HWND hWnd_tips;

TOOLINFO ti;
FILE *file;

WCHAR buffer[128];

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{


    DWORD PID = GetCurrentProcessId();
    DWORD TID = GetCurrentThreadId();

    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        if (Reserved)
        {
            printf("\n[ Dll ] static loads");
            consolePID = PID;
            consoleTID = TID;
        }
        else
        {
            AttachConsole(consolePID);
            freopen("conout$", "w", stdout);
            printf("\n[ Dll ] dynamic loads");

            wsprintf(buffer, L"This is the initial tooltip.");
            dispToolTip(buffer);
        }
        printf("\n[ Dll ] PROCESS_ATTACH %d(%d). <<<---", PID, TID);
        break;
    case DLL_PROCESS_DETACH:
        if (Reserved)
        {
            printf("\n[ Dll ] static frees");
            unhook();
        }
        else
        {
            printf("\n[ Dll ] dynamic frees");
            printf("\n[ Dll ] PROCESS_DETACH %d(%d). --->>>", PID, TID);
            //printf("\n[ Dll ] tooltipswindow = (%d). --->>>", hWnd_tips);
            DestroyWindow(hWnd_tips);
            hWnd_tips = 0;

            FreeConsole();
        }
        printf("\n[ Dll ] PROCESS_DETACH %d(%d). --->>>", PID, TID);
        break;
    case DLL_THREAD_ATTACH:
        printf("\n[ Dll ] THREAD_ATTACH %d(%d).", PID, TID);
        break;
    case DLL_THREAD_DETACH:
        printf("\n[ Dll ] THREAD_DETACH %d(%d).", PID, TID);
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
    // return CallNextHookEx(hhook_Keyboard, nCode, wParam, lParam);

    if (lParam & 0x80000000) // 当前按键的 transition state (释放按键为1,按下为0)
    {
        printf("\n[ Keyboard ] %d : actionCounter = (%d)", GetTickCount(), ++actionCounter);
        wsprintf(buffer, L"actionCounter = %d", actionCounter);
        dispToolTip(buffer);

        if ('Q' == wParam)
        {
            OutputDebugString(L"sd---------------------------fsddsfa");
            PostMessage(hWnd_hooked, WM_KEYUP, VK_NUMPAD5, lParam);
            return 1;
        }
    }
    else
    {
        if ('Q' == wParam)
        {
            PostMessage(hWnd_hooked, WM_KEYDOWN, VK_NUMPAD5, lParam);
            return 1;
        }
    }
    return CallNextHookEx(hhook_Keyboard, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (WM_LBUTTONUP == wParam || WM_RBUTTONUP == wParam)
    {
        printf("\n[ Mouse ] %d: actionCounter(%d) +1", GetTickCount(), ++actionCounter);

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

    hwObj = FindWindow(L"Notepad", L"Test.txt - 记事本");
    if (hwObj)
    {
        printf("\n[ Hook ] Find test.txt notepad window ( 找到测试记事本 ).");
        threadID = GetWindowThreadProcessId(hwObj, &procID);
        hWnd_hooked = FindWindowEx(hwObj, NULL, L"Edit", NULL);


        hhook_Keyboard = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc,
                                          GetModuleHandle(L"hookdll"), threadID);
        hhook_Mouse = SetWindowsHookEx(WH_MOUSE, MouseProc,
                                       GetModuleHandle(L"hookdll"), threadID);
        return procID;
    }

    return 0;
}

VOID unhook()
{
    UnhookWindowsHookEx(hhook_Keyboard);
    UnhookWindowsHookEx(hhook_Mouse);
}

VOID dispToolTip(WCHAR *pText)
{
    if (!hWnd_tips)
    {
        hWnd_tips = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                   TTS_NOPREFIX | TTS_ALWAYSTIP,
                                   0, 0, 0, 0, NULL, NULL, NULL, NULL);

        ti = {0};
        // ti.cbSize = sizeof(ti);
        ti.cbSize = TTTOOLINFOW_V2_SIZE;
        ti.uFlags = TTF_TRACK;
        ti.lpszText = (WCHAR *)L"This is a new tooltip.";

        bool isFlag = SendMessage(hWnd_tips, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
        printf("\n[ ToolTip ] TTM_ADDTOOL message whether suceessed(%d)", isFlag);

        SendMessage(hWnd_tips, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(800, 50));
        SendMessage(hWnd_tips, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

        printf("\n[ ToolTip ] tooltip(0x%x) created", hWnd_tips);
    }
    else
    {
        ti.lpszText = pText;
        SendMessage(hWnd_tips, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
        printf("\n[ ToolTip ] tooltip(0x%x) updated", hWnd_tips);
        //SendMessage(hWnd_tips, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
    }
}
