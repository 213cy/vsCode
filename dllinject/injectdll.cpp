#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

//#include "helloworld.h"
#include <stdio.h>
#include <windows.h>
//#include <tchar.h>


INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
    /* open file */
    FILE *file;
    fopen_s(&file, "C:\\Users\\Administrator\\b.txt", "a+");

    //int addr = 10;
    LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        fprintf(file, "DLL attach function called(0x%p).\n", addr);
        break;
    case DLL_PROCESS_DETACH:
        fprintf(file, "DLL detach function called.\n");
        break;
    case DLL_THREAD_ATTACH:
        fprintf(file, "DLL thread attach function called.\n");
        break;
    case DLL_THREAD_DETACH:
        fprintf(file, "DLL thread detach function called(0x%p).\n", addr);
        break;
    }

    /* close file */
    fclose(file);

    return TRUE;
}
