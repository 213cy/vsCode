#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <Commctrl.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <Mmsystem.h>
#include "hookdll.h"
//#include <tchar.h>

//#pragma comment(lib,"WINMM.LIB") //Winmm.lib

int hookPid = 0;
WCHAR buffer[80];

void menu_disp()
{
    printf("\n -------- MENU  菜单选项 --------", hookPid);
    printf("  -------- (hooking PID: %d) -------- ", hookPid);
    printf("\n 1 - Start hook");
    printf("\n 2 - Remove hook");
    printf("\n 3 - reset actionCounter");
    printf("\n 4 - update tooltip");
    printf("\n 7 - List task whos PID equal %d", hookPid);
    printf("\n 8 - List tasks with \"hookdll.dll\" module loaded");
    printf("\n 9 - Clear screen");
    printf("\n 0 - Exit");
    printf("\n请选择：");
    printf(" Enter an opcode:  ");
}

int myhookproc()
{
    hookPid = sethook();
    if (hookPid)
    {
        printf("\n # # # procedure was hooked to process %d successfully.", hookPid);
        printf("\n # # # hooking ...");
    }
    else
    {
        printf("\n # # # hook failed(%d).", hookPid);
    }
    return hookPid;
}


int main()
{
    SetConsoleOutputCP(936);

    HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsole(hd, L"\nhello 控制台", sizeof(L"\nhello 控制台") / 2, NULL, NULL);
    WriteConsole(hd, L"\nhello 控制台", 10, NULL, NULL);

    // HANDLE hcsb = GetStdHandle( STD_OUTPUT_HANDLE );
    // CONSOLE_FONT_INFOEX cfi = {sizeof(cfi)};
    // // Populate cfi with the screen buffer's current font info
    // GetCurrentConsoleFontEx(hcsb, FALSE, &cfi);
    // // Modify the font size in cfi
    // // cfi.dwFontSize.X *= 2;
    // // cfi.dwFontSize.Y *= 2;
    // wcscpy(cfi.FaceName, L"Lucida Console");
    // Use cfi to set the screen buffer's new font
    // SetCurrentConsoleFontEx(hcsb, FALSE, &cfi);
    // CloseHandle(hcsb)

    char command[80];
    char codestr[80];
    int opcode = 10;
    while (opcode != 0)
    {
        menu_disp();
        scanf("%s", codestr);
        // scanf("%d", &opcode);
        sscanf(codestr, "%d", &opcode);

        switch (opcode)
        {

        case 1:
            printf("\n # # # selected hook Installed...");
            myhookproc();
            break;
        case 2:
            printf("\n # # # selected hook removed...");
            unhook();
            break;
        case 3:
            printf("\n # # # selected counter(%d) reset...", actionCounter);
            actionCounter = 0;
            break;
        case 4:
            printf("\n # # # selected update counter...");
            wsprintf(buffer, L"actionCounter = (%d)", actionCounter++);
            dispToolTip(buffer);
            break;


        case 7:
            sprintf(command, "tasklist /m /fi \"PID eq %d\"", hookPid);
            system(command);
            break;
        case 8:
            sprintf(command, "tasklist /m /fi \"modules eq hookdll.dll\"");
            system(command);
            break;

        case 9:
            system("cls");
            printf("\n # # #  ----  ----  screen cleared   ----  -----");
            break;
        case 0:
            printf("\n # # # exit program.");
            break;
        default:
            printf("\n # # # can't resloved your entered opcode \"%d\"(%s).", opcode, codestr);
        }
    }
    printf("\n # # # done\n # # # .");
    Sleep(2000);

    return 1;

    // HINSTANCE hInstLibrary = LoadLibrary(L"dlltest.dll");
    // printf("\n # # # LoadLibrary");
    // db = (INT_FUNC)GetProcAddress(hInstLibrary, "Double");
    // printf("\n # # # Hello :%d", db(333));
    // FreeLibrary(hInstLibrary);
}
