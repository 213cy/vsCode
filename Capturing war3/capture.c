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

VOID CALLBACK MyTimerProc(
    HWND hwnd,    // handle to window for timer messages
    UINT message, // WM_TIMER message
    UINT idTimer, // timer identifier
    DWORD dwTime) // current system time
{

    printf("\n[%d]window(0x%X) timer time up!", dwTime, hwnd);
}

DWORD WINAPI newThread(LPVOID lpParam)
{
    UINT uResult = SetTimer(NULL,                    // handle to main window
                            IDT_TIMER,               // timer identifier
                            10000,                   // 10-second interval
                            (TIMERPROC)MyTimerProc); // timer callback
    // printf("\nuResult = %d(0==failed)", uResult);

    RegisterHotKey(NULL, 1, MOD_ALT | MOD_CONTROL, 0x41); //0x41 is 'a'
    RegisterHotKey(NULL, 2, MOD_ALT | MOD_CONTROL, 0x42); //0x42 is 'b'
    RegisterHotKey(NULL, 3, MOD_ALT | MOD_CONTROL, 0x43); //0x42 is 'c'
    // printf("\nHotkey 'CONTROL+ALT+b' registered");

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        // printf("\nmsg.message %d ", msg.message);
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
                printf("\nWM_HOTKEY A received, Image capturing...");
                CaptureAnImage(NULL);
                break;
            case 2:
                printf("\nWM_HOTKEY B received, Reinitiallizing...");
                endCapture();
                initCapture();
                break;
            case 3:
                printf("\nWM_HOTKEY C received, Reinitiallizing...");
                // WM_MOUSEMOVE  WM_LBUTTONDOWN  WM_LBUTTONUP
                SetCursorPos(240, 785);
                Sleep(100);
                INPUT m_InPut[2] = {0};
                m_InPut[0].type = m_InPut[1].type = INPUT_MOUSE;
                m_InPut[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                m_InPut[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                SendInput(2, m_InPut, sizeof(INPUT));
                Sleep(1000);
                SetCursorPos(1, 1);
                Sleep(1000);
                SetCursorPos(1437, 897);

                break;
            }
            continue;
        }
        if (msg.message == WAITFORCAPTUER_THREAD_MSG) 
        {
            printf("WAITFORCAPTUER_THREAD_MSG message\n");
            continue;
        }
    }

    KillTimer(NULL, IDT_TIMER); //?
    printf("\n # # # KillTimer...");

    return msg.wParam;
    //  return (int)msg.wParam;
}

int main()
{
    SetConsoleOutputCP(65001);
    // MessageBox(NULL, L"No timer is available.", L"Failed", MB_OK);
    printf("\n # # # CreateThread...");
    DWORD TID;

    HANDLE hThreadCmd = CreateThread(
        NULL,      // default security attributes
        0,         // use default stack size
        newThread, // thread function name
        NULL,      // argument to thread function
        0,         // use default creation flags
        &TID);     // returns the thread identifier
    CloseHandle(hThreadCmd);


    OpenClipboard(NULL);
    EmptyClipboard();
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, 2 * 4);
    LPDWORD lpMem = (DWORD *)GlobalLock(hMem);
    memcpy(lpMem , &TID, 4);
    int a =12345;
    memcpy(lpMem + 1, &a, 4);
    // memcpy((char *)lpMem , "abcdabcd", 8);
    GlobalUnlock(hMem);
    SetClipboardData(CF_TEXT, hMem);
    printf("\n(%d) ",GetLastError()) ;
    CloseClipboard();

    OpenClipboard(NULL);
    HGLOBAL hMem2 = GetClipboardData(CF_TEXT);
    LPDWORD lpMem2= (DWORD *)GlobalLock(hMem2);
    printf("\n(%d) (%d)\n(%d) (%d)",TID,a, *lpMem2,*(lpMem2+1)) ;
    // printf("\n(%x) (%x) (%x) (%x)",lpMem2,lpMem2+1,lpMem2+2,lpMem2+3) ;
    a=*(lpMem2+1);
    GlobalUnlock(hMem2);
    CloseClipboard();

    WCHAR classname[50];
    char buff[80] = {"0"};
    char *codestr = buff + 1; //(char *) (buff+1)
    int opcode;
    while (opcode != 9)
    {
        RealGetWindowClass(hWndScreen, classname, 50);
        printf("\n -------- 菜单选项(MENU) --------");
        printf("  -------- (window class: %ls) -------- ", classname);
        printf("\n 1 - initCapture");
        printf("\n 2 - endCapture");
        printf("\n 3 - CaptureAnImage2");
        printf("\n 4 - Display infomation");
        printf("\n 9 - Exit");
        printf("\n HOTKEYS: CONTROL+ALT+a , CONTROL+ALT+b , CONTROL+ALT+c");
        printf("\n请输入操作码(opcode)：  ");

        scanf("%s", codestr);
        sscanf(buff, "%d", &opcode);

        switch (opcode)
        {
        case 1:
            printf("\n # # # initCapture...");
            initCapture();
            break;
        case 2:
            printf("\n # # # endCapture...");
            endCapture();
            break;
        case 3:
            printf("\n # # # CaptureAnImage2...");
            CaptureAnImage2();
            break;
        case 4:;
            // hWndDesk;
            WCHAR aaa[50];

            INPUT inputs = {0};
            inputs.type = INPUT_MOUSE;
            inputs.mi.dx = 0xFFFF / 1400 * 100;
            inputs.mi.dy = 0xFFFF / 900 * 200;
            inputs.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
            SendInput(1, &inputs, sizeof(inputs));
            Sleep(1);

            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, 500, 500, 0, 0);

            HWND hWnd_notepad = FindWindow(L"Notepad", L"Test.txt - 记事本");
            RealGetWindowClass(hWnd_notepad, aaa, 50);
            printf("\n # # # DC :(%x) (%ls) .", hWnd_notepad, aaa);
            break;

        case 9:
            printf("\n # # # exit program.");
            break;
        default:
            printf("\n # # # can't resloved your entered opcode \"%d\"(%s).", opcode, codestr);
        }
    }
    printf("\n # # # done.\n # # # .");
    Sleep(1000);
    printf("\n # # # .");
    Sleep(1000);

    return 0;
}
