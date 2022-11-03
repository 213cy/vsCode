#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>
// #include <tchar.h>
#include <locale.h>
// #include <conio.h>

WCHAR buffer[64];
CHAR buf[64];

int main()
{
    printf(" # # # ####一些汉字###########################\n");

    SetConsoleOutputCP(936);

    // AllocConsole() ;
    // HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE) ;
    // WriteConsole(hd , "hello hplonline" , sizeof("hello hplonline") , NULL , NULL );
    // CloseHandle(hd) ;
    // FreeConsole();
    
    // WCHAR *szBuf = L"找不到记事本\n";
    const WCHAR *szBuf = L"wprintf 一些WCHAR汉字\n";
    wprintf(szBuf);
    printf(" # # # ####一些汉字###########################\n");
    printf("%ls", L"asdfssdf\n");
    printf(" # # # ####一些汉字###########################\n");

    printf("\nEnter interactive model!");
    getchar();

    setlocale(LC_ALL, "chs");

    printf(" # # # ####一些汉字###########################\n");


    DWORD nMode;
    HANDLE hConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hConsoleHandle, &nMode);
    SetConsoleMode(hConsoleHandle, nMode & ~ENABLE_MOUSE_INPUT); // 可以用鼠标右键了
    
    getchar();
    getchar();
    getchar();

    printf("\n # # # done .\n # # # .\n");
    Sleep(5000);
    return 0;
}