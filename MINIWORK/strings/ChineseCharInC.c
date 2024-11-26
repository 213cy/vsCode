#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif


#include <windows.h>
#include <stdio.h>
#include <locale.h>

//  using gcc build c file
//  set SetConsoleOutputCP(CP_UTF8)
// "-finput-charset=UTF-8",  // 处理mingw中文编码问题
// "-fexec-charset=UTF-8",   // 处理mingw中文编码问题
int main() {
    // Set the locale to use UTF-8 encoding
    setlocale(LC_ALL, "en_US.UTF-8");

    // Set the console output code page to UTF-8 (CP_UTF8 = 65001)
    SetConsoleOutputCP(CP_UTF8);
 
    // Print a Chinese character string
    printf("---------------------\n");
    printf("你好，世界！\n");
    printf("---------------------\n");
    wprintf(L"你好，世界！\n");
    printf("---------------------\n");

    getchar();
    return 0;
}





// ======================================






