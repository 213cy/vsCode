#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>

#define BUF_SIZE 256

char name[] = "jiangyejiangyejiangye";
TCHAR szName[] = TEXT("Global\\MyFileMappingObject");

CHAR buf[64];
WCHAR buffer[64];

int main()
{


    SetConsoleOutputCP(936);

    printf("[  log  ]GetModuleHandle (0x%x).\n", GetModuleHandle(NULL));
    printf("[  log  ]enter point main (0x%x).\n", main);

    printf("\n 0x%x,0x%x,0x%x,0x%x", ***main, *main, main, &main);
    printf("\n 0x%x,0x%x,0x%x", *buf, buf, &buf);
    printf("\n 0x%x,0x%x,0x%x", *buffer, buffer, &buffer);

    printf(" \n\n[comment] ======================%d================== \n\n", rand());


    // HANDLE hMutex = CreateMutex(NULL, false, TEXT("aaa"));
    // HANDLE hMutex2 = CreateMutex(NULL, false, TEXT("Global\\C47EF948-964B-4AC9-A42D-36518DCE5882"));

    int intA;
    intA = 10 - 20.5;
    printf("%d %d %d \n", intA, intA - 12, intA - 12.5); // -10 -22 0



    // MessageBox(NULL, L"content", L"title Caption", MB_OK);

    SYSTEMTIME s;
    GetLocalTime(&s);
    printf("It's %d/%d/%d %02d:%02d:%02d now.\n", s.wYear, s.wMonth, s.wDay, s.wHour, s.wMinute, s.wMinute);

    printf(" \n\n[comment] ======================%d================== \n\n", rand());



    return 0;
}