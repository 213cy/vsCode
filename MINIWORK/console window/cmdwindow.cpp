#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <Mmsystem.h>
//#include <tchar.h>

//#pragma comment(lib,"WINMM.LIB") //Winmm.lib

HWND hWnd_host ;
HWND hWnd_Btn;
HANDLE hThreadCmd;

DWORD Tid = 0;
WCHAR buf[80];

void menu_disp()
{
    printf("\n ------- MENU  菜单选项 ------- ( TID: %d ) ------- \n", Tid);
    printf(" 1 - Start thread\n");
    printf(" 2 - Print information\n");
    printf(" 3 - only create host window(won't work)\n");
    printf(" 0 - Exit\n");
    printf("  请选择 Enter an opcode:  ");
}

LRESULT CALLBACK pWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        printf("\n[  log  ] WM_CREATE message. create a button ");
        // Initialize the window.

        printf("\n # # # InitCommonControlsEx...");
        // INITCOMMONCONTROLSEX InitCtrls;
        // InitCtrls.dwSize = sizeof(InitCtrls);
        // iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
        // InitCtrls.dwICC = ICC_WIN95_CLASSES;
        // if (!InitCommonControlsEx(&InitCtrls))
        //     MessageBox(NULL, L"InitCommonControlsEx failed", L"error", MB_OK);

        // Create a button control
        hWnd_Btn = CreateWindowEx(0, L"BUTTON", L"My Button", WS_VISIBLE | WS_CHILD,
                                  10, 10, 110, 30, hwnd, NULL, GetModuleHandle(NULL), NULL);

        return TRUE;

    case WM_PAINT:
        // Paint the window's client area.
        // printf("\n[  log  ] WM_PAINT message. ");
        UpdateWindow(hwnd);
        return 0;

    case WM_SIZE:
        // Set the size and position of the window.
        printf("\n[  log  ] WM_SIZE message.");
        return TRUE;

    case WM_DESTROY:
        // Clean up window-specific data objects.
        printf("\n[  log  ] WM_DESTROY message.");
        Tid = NULL;
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void createHostWindow()
{
    WCHAR *class_name = (WCHAR *)L"DUMMY_HOST_CLASS";
    WNDCLASSEX wx = {};
    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = pWndProc; // function which will handle messages
    wx.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wx.hInstance = GetModuleHandle(NULL);
    wx.lpszClassName = class_name;
    if (!RegisterClassEx(&wx))
    {
        MessageBox(NULL, L"can't register window class", L"error", MB_OK);
    }
    hWnd_host = CreateWindowEx(0, class_name, L"dummy_dummy_host_host",
                                    WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 300, 100,
                                    NULL, NULL, GetModuleHandle(NULL), NULL);
    printf("\n[  log  ] Created a host window (0x%x)", hWnd_host);
}

DWORD WINAPI cmdMessageLoop(LPVOID lpParam)
{
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

DWORD WINAPI newThread(LPVOID lpParam)
{
    createHostWindow();
    cmdMessageLoop(lpParam);
    return 0;
}

int main()
{
    // SetConsoleOutputCP(936);
    system("chcp 936");

    char command[80];
    char codestr[80];
    int opcode = 10;
    while (opcode != 0)
    {
        menu_disp();
        scanf("%s", codestr);
        // scanf("%d", &opcode);
        sscanf(codestr, "%d", &opcode);

        switch (opcode)
        {
        case 1:
            printf(" # # # CreateThread...\n");
            hThreadCmd = CreateThread(
                NULL,      // default security attributes
                0,         // use default stack size
                newThread, // thread function name
                NULL,      // argument to thread function
                0,         // use default creation flags
                NULL);     // returns the thread identifier
            Tid = GetThreadId(hThreadCmd);
            break;
        case 2:
            printf(" # # # display infomation...\n");
            printf("[  log  ]GetLastError (%d).\n", GetLastError());

            break;

        case 3:
            printf(" # # # createHostWindow...\n");
            newThread(NULL);
            break;

        case 0:
            printf(" # # # exit program.\n");
            break;
        default:
            printf(" # # # can't resloved your entered opcode \"%d\"(%s).\n", opcode, codestr);
        }
    }
    printf(" # # # done\n # # # .\n");
    Sleep(1000);

    return 1;
}
