#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>
//#include <string.h>
#include <tchar.h>

char name[] = "jiangyejiangyejiangye";
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");
INT a;

#define BUF_SIZE 256
#define IDT_TIMER 1

VOID CALLBACK MyTimerProc(
    HWND hwnd,    // handle to window for timer messages
    UINT message, // WM_TIMER message
    UINT idTimer, // timer identifier
    DWORD dwTime) // current system time
{

    printf("[%d]window(0x%X) timer time up!\n", dwTime, hwnd);
}

DWORD WINAPI newThread(LPVOID lpParam)
{
    UINT uResult = SetTimer(NULL,                    // handle to main window
                            IDT_TIMER,               // timer identifier
                            10000,                   // 10-second interval
                            (TIMERPROC)MyTimerProc); // timer callback
    printf("uResult = %d(0==failed)\n", uResult);

    if (RegisterHotKey(
            NULL,
            1,
            MOD_ALT | MOD_SHIFT,
            0x42)) //0x42 is 'b'
    {
        printf("Hotkey 'ALT+SHIFT+b' registered, using MOD_NOREPEAT flag\n");
    }

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        printf("msg.message %d \n", msg.message);
        if (msg.message == WM_HOTKEY)
        {
            printf("WM_HOTKEY received\n");
        }
        if (msg.message == WM_TIMER) //WM_KEYDOWN
        {
            printf("WM_TIMER message\n");
            DispatchMessage(&msg);
            // break;
        }
        if (msg.message == WM_CHAR)
        {
            printf("WM_CHAR message\n");
            // break;
        }
    }

    return msg.wParam;
}

int main()
{
    // SetConsoleCP(65001);
    // SetConsoleCP(20936);
    // SetConsoleCP(936);
    // SetConsoleOutputCP(65001);
    //SetConsoleOutputCP(20936);
    // SetConsoleOutputCP(936);
    system("chcp 65001");
    // system("chcp 936");

    // HANDLE hMutex = CreateMutex(NULL, false, TEXT("aaa"));
    // HANDLE hMutex2 = CreateMutex(NULL, false, TEXT("Global\\C47EF948-964B-4AC9-A42D-36518DCE5882"));

    int intA;
    intA = 10 - 20.5;
    printf("%d %d %d \n", intA, intA - 12, intA - 12.5);

    char buffer[8];
    strcpy(buffer, name);
    printf("%s\n", buffer);

    // MessageBox(NULL, L"content", L"title Caption", 0);

    SYSTEMTIME s;
    GetLocalTime(&s);
    printf("It's %d/%d/%d %02d:%02d:%02d now.\n", s.wYear, s.wMonth, s.wDay, s.wHour, s.wMinute, s.wMinute);

    HWND hWnd = FindWindow(L"ConsoleWindowClass", NULL);
    printf("ConsoleWindow:0x%X ForegroundWindow:0x%X\n", hWnd, GetForegroundWindow());

    printf(" # # # CreateThread...\n");
    HANDLE hThreadCmd = CreateThread(
        NULL,      // default security attributes
        0,         // use default stack size
        newThread, // thread function name
        NULL,      // argument to thread function
        0,         // use default creation flags
        NULL);     // returns the thread identifier

    char codestr[80] = {"0"};
    scanf("%s", codestr);

    KillTimer(NULL, IDT_TIMER); //?
    printf(" # # # KillTimer...\n");
    scanf("%s", codestr);
    // getchar();
    while (true)
    {
        Sleep(2000);
    }

    return 0;
}