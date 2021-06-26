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
WCHAR buf[80];

void menu_disp()
{
    printf("\n -------- MENU  菜单选项 --------", hookPid);
    printf("  -------- (hooking PID: %d) -------- ", hookPid);
    printf("\n 1 - Start hook");
    printf("\n 2 - Remove hook");
    printf("\n 3 - Play music");
    printf("\n 4 - reset actionCounter");
    printf("\n 5 - update tooltip");
    printf("\n 6 - display infomation");
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

void displayinfo()
{
    // HWND hWnd = FindWindow(L"ConsoleWindowClass", L"Administrator: C:\\Windows\\system32\\cmd.exe");
    // HWND hWnd = FindWindow(L"ConsoleWindowClass", L"hook main");
    // DWORD procID, ThreadID;
    // threadID = GetWindowThreadProcessId(hWnd, &procID);
    // DWORD threadID = GetWindowThreadProcessId( hWnd,  NULL);

    // HWND hWnd_notepad = FindWindow(L"Notepad", L"Test.txt - 记事本");
    // printf("\n[  log  ]GetWindow (%d).", GetWindow(hWnd_notepad, GW_CHILD));
    // printf("\n[  log  ]FindWindowEX (%d).", FindWindowEx(hWnd_notepad, NULL, L"Edit", NULL));

    printf("\n[  log  ]GetLastError (%d).", GetLastError());
    HWND hWnd_tips = FindWindow(L"tooltips_class32", NULL);
    SetWindowLong(hWnd_tips, GWL_EXSTYLE, WS_EX_TOPMOST);
    SetWindowLong(hWnd_tips, GWL_STYLE, TTS_NOPREFIX | TTS_ALWAYSTIP);
    printf("\n[  log  ]FindWindowEX (%d)(0x%X).GetLastError (%d).", hWnd_tips, hWnd_tips, GetLastError());
    // SendMessage(hWnd_tips, TTM_ACTIVATE, TRUE, 0);
}

int main()
{
    // SetConsoleCP(65001);
    // SetConsoleCP(20936);
    // SetConsoleCP(936);
    // SetConsoleOutputCP(65001);
    // SetConsoleOutputCP(20936);
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
            printf("\n # # # selected music played...");
            //PlaySound(L"C:\\Users\\Administrator\\Videos\\MagicSword.mp3", NULL, SND_FILENAME);
            DWORD mciError;
            mciError = mciSendString(L"status currentMusic mode", buf, _countof(buf), NULL);
            printf("\n[  log  ] currentMusic status: %s%s%s%s%s%s%s",
                   buf, buf + 1, buf + 2, buf + 3, buf + 4, buf + 5, buf + 6);
            printf("\n[  log  ] currentMusic status: %ls", buf);
            if (mciError)
            {
                mciGetErrorString(mciError, buf, _countof(buf));
                MessageBox(NULL, buf, L"error", MB_OK);
                printf("\n[  log  ] {%ls}{%s}(%d)", buf, buf, mciError);
            }
            if (!wcscmp(buf, L"playing")) // The sound is now playing.| wcscmp(buf, L"")
            {
                printf("\n[  log  ] music is playing, return");
                break;
            }
            mciSendString(L"close currentMusic", NULL, 0, NULL);
            mciSendString(L"open C:\\Users\\Administrator\\Videos\\MagicSword.mp3 alias currentMusic", NULL, 0, NULL);
            mciSendString(L"play currentMusic", NULL, 0, NULL);
            break;

        case 4:
            printf("\n # # # selected counter(%d) reset...", actionCounter);
            actionCounter = 0;
            break;

        case 5:
            printf("\n # # # selected display information...");
            wsprintf(buf, L"actionCounter = (%d)", actionCounter++);
            dispToolTip(buf);
            // displayinfo();
            break;

        case 6:
            const WCHAR *szBuf;
            szBuf= L"找不到记事本";
            //wprintf(L"你好");
            printf("\n[  log  ]%ls %s",szBuf,szBuf);
            wprintf(L"\n一些汉字wprintf");
            printf("\n一些汉字printf");
            printf("\n 汉字汉字汉字一些汉字");
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
            system("chcp 65001");
            //  system("chcp 936");
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
    Sleep(1000);
    printf("\n # # # .");
    Sleep(1000);

    return 1;

    // HINSTANCE hInstLibrary = LoadLibrary(L"dlltest.dll");
    // printf("\n # # # LoadLibrary");
    // db = (INT_FUNC)GetProcAddress(hInstLibrary, "Double");
    // printf("\n # # # Hello :%d", db(333));
    // FreeLibrary(hInstLibrary);
}
