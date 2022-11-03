#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include "dummy.h"


WCHAR buffer[128];
CHAR buf[128];
SYSTEMTIME lt;
HANDLE hd_Output;
FILE *file;
FILE *aaa;

DWORD dwParentPID ;

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
    DWORD PID = GetCurrentProcessId();
    DWORD TID = GetCurrentThreadId();

    GetLocalTime(&lt);
    sprintf(buf, "%d-%d %02d:%02d:%02d %03d",
     lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);

    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:

        // aaa = freopen("conout$", "w", stdout);
        // freopen("conout$", "w", stdout);
        fopen_s(&file, "outfile.txt", "a+");
        fprintf(file, "\n\n[ log ] %s", buf);
        // printf("\n\n[ log ] %s", buf);



        AllocConsole();
        // AttachConsole(ATTACH_PARENT_PROCESS);
        hd_Output = GetStdHandle(STD_OUTPUT_HANDLE); //获取标准输出设备句柄
        wsprintf(buffer, L"\n[  log  ] stdout = 0x%X . stdout_file = 0x%X", stdout, *stdout);
        WriteConsole(hd_Output, buffer, wcslen(buffer), NULL, NULL);
        wsprintf(buffer, L"\n[  log  ] STD_OUTPUT_HANDLE=0x%X", hd_Output);
        // WriteConsole(hd, L"hello 控制台\n", sizeof(L"hello 控制台\n"), NULL, NULL);
        WriteConsole(hd_Output, buffer, wcslen(buffer), NULL, NULL);
        CloseHandle(hd_Output);
        // FreeConsole();

        aaa = freopen("conout$", "w", stdout);

        printf("\n[  log  ] aaa = 0x%X . aaa_file = 0x%X.", aaa, *aaa);
        printf("\n[  log  ] stdout = 0x%X . *stdout = 0x%X.", stdout, *stdout);
        printf("\n[  log  ] file = 0x%X . *file = 0x%X.", file, *file);

        printf("\n\n[ start ] %s", buf);
        printf("\n[ dll ] calling process identifier : 0x%X(%d). ", PID, PID);
        printf("\n[ dll ] calling thread identifier : 0x%X(%d). ", TID, TID);

        printf("\n[PROCESS_ATTACH] Reserved = %d (0=dynamic,other=static) . <<<---", Reserved);
        printf("\n[PROCESS_ATTACH] loads   <<<---");

        break;
    case DLL_PROCESS_DETACH:


        printf("\n[PROCESS_DETACH] Reserved = %d (0=dynamic,other=static) .    --->>>", Reserved);
        printf("\n[PROCESS_DETACH] frees      --->>>");

        printf("\n[  log  ] file = 0x%X . *file = 0x%X.", file, *file);

        printf("\n[ end ] ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
        MessageBox(NULL, L"213cyy \nFound Warcraft III\n",
                   L"Leave dll\n", MB_OK);
        FreeConsole();


        fprintf(file, "\ndwParentPID = 0x%x(%d)\n%s", dwParentPID, dwParentPID, buf);
        fclose(file);
        break;
    case DLL_THREAD_ATTACH:
        printf("\n[THREAD_ATTACH] loads   <<<---");
        break;
    case DLL_THREAD_DETACH:
        printf("\n[THREAD_DETACH] frees      --->>>");
        break;
    }

    return TRUE;
}

VOID RunDllEntry(HWND hwnd,        // handle to owner window
                 HINSTANCE hinst,  // instance handle for the DLL
                 LPTSTR lpCmdLine, // string the DLL will parse
                 int nCmdShow      // show state
)
{
    printf("\n[ export ] GetCommandLine:  %s", lpCmdLine);
    printf("\n[ export ] GetCommandLine:  %s", GetCommandLineA());
    printf("\n[ export ] GetCommandLine:  %ls", GetCommandLineW());
    printf("\n[ export ]Current process pseudo handle: <%d>. ", GetCurrentProcess());

    PROCESS_BASIC_INFORMATION pbi;
    LONG status = NtQueryInformationProcess(GetCurrentProcess(),
                                            (PROCESSINFOCLASS)0,
                                            (PVOID)&pbi,
                                            sizeof(PROCESS_BASIC_INFORMATION),
                                            NULL);

    // Copy parent Id on success
    dwParentPID = pbi.InheritedFromUniqueProcessId;
    printf("\n[ export ] status: (%d).  dwParentPID: %X(%d).", status, dwParentPID, dwParentPID);

    // MessageBox(NULL, L"Resource not available\nFound Warcraft III\n",
    //            L"Succeed\n", MB_OK);
}

