#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>
//#include <tchar.h>

WCHAR buffer[64];
CHAR buf[64];

int main()
{
    SetConsoleOutputCP(936);

    char codestr[64];
    // getchar();

    printf("\nEnter interactive model!");

    while (strcmp(codestr, "exit") != 0)
    {
        printf("\n\n -------- -------  MENU  菜单选项 -------- --------");
        printf("\n 命令列表: 1, tasklist, clear, exit");
        printf("\n      please enter command:  ");

        scanf("%s", codestr);
        if (strcmp(codestr, "1") == 0)
        {
            printf(" # # # ####一些汉字###########################\n");
        }
        else if (strcmp(codestr, "tasklist") == 0)
        {
            // sprintf(buf, "tasklist /m /fi \"modules eq Comctl32.dll\"");
            sprintf(buf, "tasklist /m /fi \"PID eq %d\"", GetCurrentProcess());
            printf("%s   ~~~ ", buf);
            system(buf);
            sprintf(buf, "tasklist /m /fi \"PID eq %d\"", GetCurrentProcessId());
            printf("%s", buf);
            system(buf);
        }
        else if (strcmp(codestr, "clear") == 0)
        {
            system("cls");
            printf(" # # #  ----  ----  screen cleared   ----  -----\n");
        }
        printf("\n  command been executed :  %s .", codestr);
    }
    printf("\n # # # done .\n # # # .\n");
    Sleep(2000);
    return 0;
}