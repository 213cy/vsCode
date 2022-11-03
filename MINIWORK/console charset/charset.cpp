#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>
#include <locale.h>

const WCHAR *music[3] = {L"C:\\Users\\Administrator\\Videos\\MagicSword.mp3",
                         L"C:\\Users\\Administrator\\Videos\\NineSwords.mp3",
                         L"C:\\Users\\Administrator\\Videos\\RedFlower.mp3"};

void menu_disp()
{
    wprintf(L"\n\n\n -------- -------  MENU  菜单选项 -------- --------");
    printf(" \n命令列表: 1, 2, exit");
    printf("\n please enter an command:  ");
}

int main()
{
    char codestr[80] = {"0"};
    WCHAR buf[128];

    // setlocale(LC_ALL, ".UTF8");
    
    // SetConsoleCP是控制输入(键盘)cp 对于printf没有影响
    // SetConsoleCP(65001);
    // SetConsoleCP(20936);
    // SetConsoleCP(936);
    // printf 是通过标准输出(stdout)打印字符 ,只有设置输入cp 才能影响其显示
    // SetConsoleOutputCP(65001);
    // SetConsoleOutputCP(20936);
    SetConsoleOutputCP(936);

    // system("chcp 65001");
    // system("chcp 936");
    
    printf("Enter interactive model!\n");
    system("dir");
    system("echo 系统命令调用");


    while (strcmp(codestr, "exit") != 0)
    {
        menu_disp();
        scanf("%s", codestr);
        if (strcmp(codestr, "1") == 0)
        {
            wsprintf(buf, L"open \"%s\" alias currentMusic", music[rand() % 3]);
            wprintf(buf);
        }
        else if (strcmp(codestr, "2") == 0)
        {
            printf("\n %ls ", L"aasd22222222222");
            printf("\n===================");
            wprintf(L"\n wprintf 一些WCHAR汉字");
            printf("\n===================");
            printf("\n一些utf-8汉字");
            printf("\n===================");
        }
        else
        {
            printf(" command received (%s).\n", codestr);
        }
    }
    return 0;
}