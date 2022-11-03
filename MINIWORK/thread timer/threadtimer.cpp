#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>

UINT uTimerID, nIDEvent;

#define IDT_TIMER 1

VOID CALLBACK MyTimerProc(
    HWND hwnd,    // handle to window for timer messages
    UINT message, // WM_TIMER message
    UINT idTimer, // timer identifier
    DWORD dwTime) // current system time
{
    printf(" [timer] hwnd=(%d) | message=(%d) | idTimer=(%d) | dwTime=(%d) \n",
           hwnd, message, idTimer, dwTime);
}

DWORD WINAPI newThread(LPVOID lpParam)
{
    uTimerID = SetTimer(NULL,                    // handle to main window
                        NULL,                    // timer identifier
                        5000,                    // 5-second interval
                        (TIMERPROC)MyTimerProc); // timer callback
    printf("\n [ Thread ] create timer : uTimerID = %d(0==failed)\n", uTimerID);

    if (RegisterHotKey(
            NULL,
            1,
            MOD_ALT | MOD_SHIFT,
            0x42)) //0x42 is 'b'
    {
        printf(" [Thread] Hotkey 'ALT+SHIFT+b' registered!\n");
    }

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        printf(" [Thread] msg.message = %d \n", msg.message);
        if (msg.message == WM_HOTKEY)
        {
            printf(" [Thread] WM_HOTKEY received\n");
        }
        if (msg.message == WM_TIMER) //WM_KEYDOWN
        {
            printf(" [Thread] WM_TIMER message\n");
            DispatchMessage(&msg);
            // break;
        }
        if (msg.message == WM_CHAR)
        {
            printf(" [Thread] WM_CHAR message\n");
            // break;
        }
    }

    return msg.wParam;
}

int main()
{
    char codestr[64];

    // HWND hWnd = FindWindow(L"ConsoleWindowClass", L"Administrator: C:\\Windows\\system32\\cmd.exe");
    HWND hWnd = FindWindow(L"ConsoleWindowClass", NULL);

    SetConsoleOutputCP(936);

    printf("Enter interactive model!\n");
    while (strcmp(codestr, "exit") != 0)
    {
        printf("\n\n\n --------   MENU  菜单选项 -------- (hWnd = 0x%x)  --------", hWnd);
        printf("\n 命令列表: cmd, thread, kill, exit");
        printf("\n please enter an command:  ");

        scanf("%s", codestr);

        if (strcmp(codestr, "cmd") == 0)
        {
            // nIDEvent = SetTimer(hWnd,                    // handle to main window
            //                     IDT_TIMER,               // timer identifier
            //                     3000,                    // 10-second interval
            //                     (TIMERPROC)MyTimerProc); // timer callback
            nIDEvent = SetTimer(NULL,                    // handle to main window
                                NULL,                    // timer identifier
                                3000,                    // 5-second interval
                                (TIMERPROC)MyTimerProc); // timer callback
            printf(" [cmd timer] nIDEvent = %d(0==failed)\n", uTimerID);
            printf(" [cmd timer] GetLastError = %d\n", GetLastError());
            printf(" [cmd timer] 失败了,cmd窗口没有WM_TIMER的消息处理?\n");
        }
        if (strcmp(codestr, "thread") == 0)
        {
            HANDLE hThreadCmd = CreateThread(
                NULL,      // default security attributes
                0,         // use default stack size
                newThread, // thread function name
                NULL,      // argument to thread function
                0,         // use default creation flags
                NULL);     // returns the thread identifier
            printf(" [ thread timer ] CreateThread ...\n");
        }
        if (strcmp(codestr, "kill") == 0)
        {
            KillTimer(hWnd, nIDEvent);
            KillTimer(NULL, uTimerID);
            printf(" [ kill timer] Killtimer...\n");
        }
        printf(" command received (%s).\n", codestr);
        // Sleep(2000);
    }
    return 0;
}