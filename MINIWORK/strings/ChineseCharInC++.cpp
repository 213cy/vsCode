#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
// #include <stdio.h>
#include <locale.h>
#include <iostream>


int ReplaceFilename() {
    // TCHAR* directoryPath = GetDirectoryName(fullPath);
    
    wchar_t newFilename[80];
    wchar_t fullPath[] = L"C:\\Users\\Example\\Documents\\file.txt";
    wchar_t * pwc = wcsrchr(fullPath,L'\\');
    *pwc = L'\0';

    wcscpy (newFilename,fullPath);
    wcscat (newFilename,L"\\aaa.txt");

    wprintf (L"%ls\n",newFilename);
    return 0;
}

int main() {
    // Set the console output code page to UTF-8 (CP_UTF8 = 65001)
    SetConsoleOutputCP(CP_UTF8);

    // Print a Chinese character string
    printf("aaa你好，世界！bbb\n");
    printf("---------------\n");

    wprintf(L"ccc你好，世界！ddd%lseee\n",L"你好，世界");
    wprintf (L"%ls\n",L"你好，世界");
    printf("---------------\n");

    std::cout << "你好，世界！\n" << std::endl;
    printf("---------------\n");
    ReplaceFilename();

    getchar();
    return 0;
}





// ======================================






