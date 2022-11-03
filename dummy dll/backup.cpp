#include "windows.h"

/* You should define DLL_EXPORTS *only* when building the DLL. */
#ifdef DLL_EXPORTS
#define EXPORTSAPI __declspec( dllexport )
#else
#define EXPORTSAPI __declspec( dllimport )
#endif

/* Define calling convention in one place, for convenience. */
#define EXPORTSCALL __cdecl
/* Make sure functions are exported with C linkage under C++ compilers. */
#ifdef __cplusplus
extern "C"
{
#endif

int APIENTRY EXPORTSAPI DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved);
// LRESULT CALLBACK EXPORTSAPI  EXPORTSCALL MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

/* Declare function using the above definitions. */
VOID EXPORTSAPI  EXPORTSCALL RunDllEntry( HWND hwnd,        // handle to owner window   
        HINSTANCE hinst,  // instance handle for the DLL   
        LPTSTR lpCmdLine, // string the DLL will parse   
        int nCmdShow      // show state   
        );

/* Declare variable using the above definitions. */
extern EXPORTSAPI int actionCounter;

#ifdef __cplusplus
} // __cplusplus defined.
#endif








#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <winternl.h>
#include <Commctrl.h>
#include <stdio.h>
#include "dummy.h"
//#include <tchar.h>

VOID dispToolTip(WCHAR *pText);

HWND hWnd_tips __attribute__((section("shared"), shared)) = NULL;
TOOLINFO ti;

WCHAR buffer[128];
SYSTEMTIME lt;
FILE *file;
VOID RunDllEntry( HWND hwnd,        // handle to owner window   
        HINSTANCE hinst,  // instance handle for the DLL   
        LPTSTR lpCmdLine, // string the DLL will parse   
        int nCmdShow      // show state   
        );

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
    // AllocConsole() ;
    // HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE) ;
    // WriteConsole(hd , "hello hplonline" , sizeof("hello hplonline") , NULL , NULL );
    // CloseHandle(hd) ;
    // FreeConsole();

    DWORD PID = GetCurrentProcessId();
    DWORD TID = GetCurrentThreadId();

    GetLocalTime(&lt);
    fprintf(file, "\n%d/%d %02d:%02d:%02d", lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);

    //  AllocConsole();
    AttachConsole(ATTACH_PARENT_PROCESS);
    // HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE); //获取标准输出设备句柄
    // wsprintf(buffer, L"\n[  log  ]closes the file(%d) currently associated with Standard output stream", stdout);
    // WriteConsole(g_hOutput, buffer, wcslen(buffer), NULL, NULL);

    printf("\n[  log  ] ===========");

    printf("\n[ dll ]calling process identifier : 0x%X(%d). ", PID, PID);
    printf("\n[ dll ]calling thread identifier : 0x%X(%d). ", TID, TID);

    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:

        freopen("conout$", "w", stdout);
        fopen_s(&file, "outfile.txt", "a+");

        // FILE *aaa = freopen("conout$", "w", stdout);
        // printf("\n[  log  ]file_aaa(%d).file_stdout(%d).", aaa, *stdout);

        printf("\n[PROCESS_ATTACH] Reserved = %d (0=dynamic,other=static) . <<<---", Reserved);
        printf("\n[PROCESS_ATTACH] loads   <<<---");
        break;
    case DLL_PROCESS_DETACH:

        fclose(file);
        FreeConsole();

        printf("\n[PROCESS_DETACH] Reserved = %d (0=dynamic,other=static) . <<<---", Reserved);
        printf("\n[PROCESS_DETACH] frees      --->>>");
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
    DWORD dwParentPID = pbi.InheritedFromUniqueProcessId;
    printf("\n[ export ] status: (%d).  dwParentPID: %X(%d).", status, dwParentPID, dwParentPID);
    printf("\n[ export ] ===========================================");

    MessageBox(NULL, L"Resource not available\nFound Warcraft III\n",
               L"Succeed\n", MB_OK);
}

VOID dispToolTip(WCHAR *pText)
{

    if (!hWnd_tips)
    {
        // hWnd_tips = CreateWindowEx(.. GetDesktopWindow(), NULL, NULL, NULL);
        hWnd_tips = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                   TTS_NOPREFIX | TTS_ALWAYSTIP,
                                   0, 0, 0, 0, NULL, NULL, NULL, NULL);
        printf("\n[  log  ]tip(%d) created", hWnd_tips);

        ti = {0};
        // ti.cbSize = sizeof(ti);
        ti.cbSize = TTTOOLINFOW_V2_SIZE;
        ti.uFlags = TTF_TRACK;
        ti.lpszText = (WCHAR *)L"This is a new tooltip.";

        bool isFlag = SendMessage(hWnd_tips, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
        printf("\n[  log  ]TTM_ADDTOOL message whether suceessed(%d)", isFlag);

        SendMessage(hWnd_tips, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(800, 50));
        SendMessage(hWnd_tips, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
    }
    else
    {
        ti.lpszText = pText;
        SendMessage(hWnd_tips, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
        printf("\n[  log  ]updated tooltip(%d)  (0x%x)...", hWnd_tips, hWnd_tips);
        //SendMessage(hWnd_tips, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
        DestroyWindow(hWnd_tips);
    }
}
