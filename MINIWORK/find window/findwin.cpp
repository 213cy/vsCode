#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>

char buffer[64];
WCHAR buf[64];

void menu_disp()
{
    printf("\n\n\n -------- -------  MENU  菜单选项 -------- --------");
    printf("\n 命令列表: 1, 2, exit");
    printf("\n please enter an command:  ");
}

int main()
{
    HWND hwObj;
    DWORD procID;
    DWORD threadID;
    char codestr[80] = {"0"};

    SetConsoleOutputCP(936);

    printf(" [Console] Find Foreground Window: %d.\n", GetForegroundWindow());
    HWND hWnd = FindWindow(L"ConsoleWindowClass", NULL);
    printf(" [Console] Find Console Window: %d (0x%x).\n", hWnd, hWnd);
    GetWindowTextA(hWnd, buffer, 10);
    printf(" [Console] Window Text : %s.\n", buffer);

    double *r = (double *)hWnd;
    printf(" [double *] : (0x%x)(0x%x)(0x%x)\n", hWnd, (double *)hWnd, r);
    // double r2 = * (double *)hWnd;
    // printf(" [* double*] : (0x%x)\n", r2 );

    printf("\n ============ Enter interactive model! ===========\n");

    while (strcmp(codestr, "exit") != 0)
    {
        menu_disp();
        scanf("%s", codestr);
        if (strcmp(codestr, "1") == 0)
        {
            hwObj = FindWindow(L"Notepad", L"Test.txt - 记事本");
            printf("\n [记事本] Find test.txt notepad window: %d (0x%x).", hwObj, hwObj);
            threadID = GetWindowThreadProcessId(hwObj, &procID);
            printf("\n [记事本] Window Thread ProcessId: %d (0x%x).", procID, procID);
            printf("\n [记事本] GetWindow (%d).", GetWindow(hwObj, GW_CHILD));
            printf("\n [记事本] FindWindowEX (%d).", FindWindowEx(hwObj, NULL, L"Edit", NULL));

            I
        }
        else if (strcmp(codestr, "2") == 0)
        {
            // HWND hWnd = FindWindow(L"ConsoleWindowClass", L"Administrator: C:\\Windows\\system32\\cmd.exe");
            // HWND hWnd = FindWindow(L"ConsoleWindowClass", NULL);
            HWND hWnd_tip = FindWindow(L"tooltips_class32", NULL);
            hwObj = FindWindow(L"OsWindow", L"Warcraft III");
            printf("\n [魔兽3] Find WAR3 window: (0x%x).", hwObj);
        }
        else
        {
            printf(" command received (%s).\n", codestr);
        }
    }
    return 0;
}