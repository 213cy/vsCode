#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <stdio.h>
#include <windows.h>
#include "callbacks.h"
// #include "resource.h"

#define IDT_TIMER 0
#define WAITFORCAPTUER_THREAD_MSG WM_USER + 100

WCHAR classname[50];
HWND hwindowObj;
UINT_PTR timerID;

VOID CALLBACK MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
    printf("\n = = = window(0x%X) timer time(%d) up!", hwnd, dwTime);
    KillTimer(NULL, timerID);
    printf("\n = = = KillTimer...");
}

DWORD WINAPI newThread(LPVOID lpParam)
{
    timerID = SetTimer(NULL, IDT_TIMER, 10000, (TIMERPROC)MyTimerProc);
    printf("\n = = = timer(%d) start!", timerID);

    RegisterHotKey(NULL, 1, MOD_ALT | MOD_CONTROL, 0x41); //0x41 is 'a'
    RegisterHotKey(NULL, 2, MOD_ALT | MOD_CONTROL, 0x42); //0x42 is 'b'
    RegisterHotKey(NULL, 3, MOD_ALT | MOD_CONTROL, 0x43); //0x42 is 'c'
    // printf("\n = = = Hotkey 'CONTROL+ALT+b' registered");

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        // printf("\n = = = msg.message %d ", msg.message);
        if (msg.message == WM_TIMER)
        {
            DispatchMessage(&msg);
            continue;
        }
        if (msg.message == WM_HOTKEY)
        {
            switch (msg.wParam)
            {
            case 1:
                printf("\n = = = WM_HOTKEY A received, Image capturing and saving...");
                CaptureAnImage(0);
                ImageSave();
                break;
            case 2:
                printf("\n = = = WM_HOTKEY B received, Reinitiallizing...");
                endCapture();
                hwindowObj = initCapture();
                RealGetWindowClass(hwindowObj, classname, 50);
                break;
            case 3:
                printf("\n = = = WM_HOTKEY C received, perserved hotkey");
                // WM_MOUSEMOVE  WM_LBUTTONDOWN  WM_LBUTTONUP
                SendMessage(hwindowObj, WM_LBUTTONDOWN, 1, MAKELPARAM(100, 100));
                SendMessage(hwindowObj, WM_LBUTTONUP, 1, MAKELPARAM(100, 100));

                break;
            }
            continue;
        }
        if (msg.message == WAITFORCAPTUER_THREAD_MSG)
        {
            printf("\n = = = WAITFORCAPTUER_THREAD_MSG message received");
            CaptureAnImage(0);
            continue;
        }
    }

    return 0;
}

int main()
{
    SetConsoleOutputCP(65001);
    // MessageBox(NULL, L"No timer is available.", L"Failed", MB_OK);

    hwindowObj = initCapture();
    RealGetWindowClass(hwindowObj, classname, 50);

    DWORD TID;
    HANDLE hThreadCmd = CreateThread(
        NULL,      // default security attributes
        0,         // use default stack size
        newThread, // thread function name
        NULL,      // argument to thread function
        0,         // use default creation flags
        &TID);     // returns the thread identifier
    printf("\n # # # Create Thread(0x%x)...", hThreadCmd);
    CloseHandle(hThreadCmd);

    OpenClipboard(NULL);
    EmptyClipboard();
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, 2 * 4);
    LPDWORD lpMem = (DWORD *)GlobalLock(hMem);
    memcpy(lpMem, &TID, 4);
    int a = 12345;
    memcpy(lpMem + 1, &a, 4);
    // memcpy((char *)lpMem , "abcdabcd", 8);
    GlobalUnlock(hMem);
    HANDLE hdata = SetClipboardData(CF_TEXT, hMem);
    printf("\n # # # Paste PID to clipboard(0x%x)... ", hdata);
    CloseClipboard();

    char buff[80] = {"0"};
    char *codestr = buff + 1; //(char *) (buff+1)
    int opcode;
    while (opcode != 9)
    {
        printf("\n ------- 菜单选项(MENU) ------- ");
        printf(" {window(0x%x) class: %ls | thread id: %d} ------- ", hwindowObj, classname, TID);
        printf("\n 1 - Disaplay clipboard data");
        printf("\n 2 - Get window class name");
        printf("\n 3 - Save image");
        printf("\n 4 - Mouse behaviour simulate");
        printf("\n 9 - Exit");
        printf("\n HOTKEYS: CONTROL+ALT+a - Image capture and save");
        printf("\n HOTKEYS: CONTROL+ALT+b - Reinitialize");
        printf("\n HOTKEYS: CONTROL+ALT+c - Test");
        printf("\n请输入操作码(opcode)：  ");

        scanf("%s", codestr);
        sscanf(buff, "%d", &opcode);

        switch (opcode)
        {
        case 1:
            printf("\n # # # Disaplay clipboard data...");
            OpenClipboard(NULL);
            HGLOBAL hMem2 = GetClipboardData(CF_TEXT);
            LPDWORD lpMem2 = (DWORD *)GlobalLock(hMem2);
            printf("\ndata to wirte:(%d) (%d)\ndata in clipboard:(%d) (%d)",
                   TID, a, *lpMem2, *(lpMem2 + 1));
            GlobalUnlock(hMem2);
            CloseClipboard();
            break;
        case 2:
            printf("\n # # # GetWindowClass...");
            // hWndDesk;
            WCHAR aaa[50];
            HWND hWnd_notepad = FindWindow(L"Notepad", L"Test.txt - 记事本");
            RealGetWindowClass(hWnd_notepad, aaa, 50);
            printf("\n # # # windows handle:(0x%x) | classname:(%ls).", hWnd_notepad, aaa);
            break;
        case 3:
            printf("\n # # # Save image in file...");
            CaptureAnImage(0);
            ImageSave();
            break;
        case 4:;

            INPUT inputs = {0};
            inputs.type = INPUT_MOUSE;
            // dx = ($FFFF div(GetSystemMetrics(SM_CXSCREEN) - 1)) * 150;
            // dy = ($FFFF div(GetSystemMetrics(SM_CYSCREEN) - 1)) * 120;
            inputs.mi.dx = 0xFFFF / 1440 * 100;
            inputs.mi.dy = 0xFFFF / 900 * 200;
            inputs.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
            SendInput(1, &inputs, sizeof(inputs));
            Sleep(1000);

            INPUT m_InPut[2] = {0};
            m_InPut[0].type = m_InPut[1].type = INPUT_MOUSE;
            m_InPut[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            m_InPut[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(2, m_InPut, sizeof(INPUT));
            Sleep(1000);

            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, 500, 500, 0, 0);

            break;

        case 9:
            printf("\n # # # exit program.");
            break;
        default:
            printf("\n # # # can't resloved your entered opcode (%d)\"%s\".", opcode, codestr);
        }
    }
    printf("\n # # # done.\n # # # .");
    Sleep(1000);
    printf("\n # # # .");
    Sleep(1000);

    return 0;
}
