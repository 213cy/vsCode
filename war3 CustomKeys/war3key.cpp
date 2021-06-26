#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>
#include "hookdll.h"

DWORD hookPid = 0;

void menu_disp()
{
    printf("\n -------- 菜单选项(MENU) --------");
    printf("  -------- (hooking PID: %d) -------- ", hookPid);
    printf("\n 1 - Rehook");
    printf("\n 2 - Unhook");
    printf("\n 3 - Music switch");
    printf("\n 4 - Display infomation");
    printf("\n 9 - Exit");
    printf("\n请输入操作码(opcode)：  ");
}

void rehook()
{
    while (!hookPid)
    {
        hookPid = sethook();
        if (hookPid)
        {
            printf("\n # # # Find WAR3 window( 找到魔兽3 ).");
            printf("\n # # # hooking to process (%d) successfully...", hookPid);
        }
        else
        {
            printf("\n # # # waiting for war3 running ...");
            Sleep(20000);
        }
    }
}
int main()
{
    SetConsoleOutputCP(936);
    musicFlag = TRUE;
    rehook();

    char buff[80] = {"0"};
    char *codestr = buff + 1; //(char *) (buff+1)
    int opcode ;

    while (opcode != 9)
    {
        menu_disp();
        scanf("%s", codestr);
        // scanf("%d", &opcode);
        sscanf(buff, "%d", &opcode);

        switch (opcode)
        {
        case 1:
            printf("\n # # # selected rehook...");
            hookPid = 0;
            rehook();
            break;
        case 2:
            printf("\n # # # selected unhook...");
            unhook();
            // hookPid = 0;
            break;        
            case 3:
            printf("\n # # # switch musicFlag...");
            musicFlag = !musicFlag;
            if (musicFlag)
            {
                printf("\n[  log  ]music is on.");
            }
            else
            {
                printf("\n[  log  ]music is off.");
            }
            
            // hookPid = 0;
            break;

        case 4:
            printf("\n # # # display infomation...");
            HANDLE hProcess;
            DWORD ExitCode;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, hookPid);
            if (!GetExitCodeProcess(hProcess, &ExitCode))
            {
                printf("\n[  log  ]war3 no longer exist.");
            }
            else
            {
                printf("\n[  log  ]war3(pid=%d) exitcode: (%d). <STILL_ACTIVE = 259>",
                       hookPid, ExitCode);
            }
            CloseHandle(hProcess);

            printf("\n[  log  ]war3(window=%d) isexist: (%d).", hWnd_hooked, IsWindow(hWnd_hooked));
            printf("\n[  log  ]tooltip window: (%d).", hWnd_tip);
            printf("\n[  log  ]music flag: (%d).", musicFlag);
            printf("\n[  log  ]apm: (%d).", apmRecorder);

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

    return 1;
}
