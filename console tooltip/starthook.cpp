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
void dispToolTip(WCHAR *pText, BOOL isNew);

HWND hWnd_host;
HWND hWnd_Btn;
HWND hWnd_tip;
TOOLINFO ti;
HANDLE hThreadCmd;

BOOL isFlag;

DWORD hookPid = 0;
int counter;
WCHAR buf[80];

void menu_disp()
{
    printf("\n -------- MENU  菜单选项 --------", hookPid);
    printf("  -------- (hooking PID: %d) -------- \n", hookPid);
    printf(" 1 - create host window\n");
    printf(" 2 - InitCommonControlsEx\n");
    printf(" 3 - start thread\n");
    printf(" 4 - new tooltip\n");
    printf(" 5 - update tooltip\n");
    printf(" 6 - Print characters\n");
    printf(" 7 - List task whos PID equal %d\n", hookPid);
    printf(" 8 - List tasks with \"hookdll.dll\" module loaded\n");
    printf(" 9 - Clear screen\n");
    printf(" 0 - Exit\n");
    printf("请选择：");
    printf(" Enter an opcode:  ");
}

LRESULT CALLBACK pWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
    case WM_CREATE:
        // Initialize the window.
        // Load and register Tooltip control class

        // INITCOMMONCONTROLSEX iccx;
        // iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
        // iccx.dwICC = ICC_WIN95_CLASSES;
        // if (!InitCommonControlsEx(&iccx))
        //     MessageBox(NULL, L"InitCommonControlsEx failed", L"error", MB_OK);

        // Create a button control
        hWnd_Btn = CreateWindowEx(0, L"BUTTON", L"My Button", WS_VISIBLE | WS_CHILD,
                                  10, 10, 110, 30, hwnd, NULL, GetModuleHandle(NULL), NULL);
        printf("\n[  log  ]button created complete");

        // Create a tooltip
        // A tooltip control should not have the WS_CHILD style, nor should it
        // have an id, otherwise its behavior will be adversely affected.
        HWND hTooltip;
        hTooltip = CreateWindowEx(0, TOOLTIPS_CLASS, L"", TTS_ALWAYSTIP,
                                  0, 0, 0, 0, hwnd, 0, GetModuleHandle(NULL), 0);

        // ti = {0};
        // ti.cbSize = sizeof(ti);
        ti.cbSize = TTTOOLINFOW_V2_SIZE;

        ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
        ti.uId = (UINT_PTR)hWnd_Btn;
        ti.lpszText = (WCHAR *)L"This is a button.";
        ti.hwnd = hwnd;
        SendMessage(hTooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);
        return TRUE;

        // case WM_PAINT:
        //     // Paint the window's client area.
        //     return 0;

    case WM_SIZE:
        // Set the size and position of the window.
        printf("\n[  log  ]WM_SIZE message GetModuleHandle(%d)(0x%x).", GetModuleHandle(NULL), GetModuleHandle(NULL));

        if (hWnd_tip)
        {
            printf("\n[  log  ]send message to tip(%d)", hWnd_tip);

            wsprintf(buf, L"counter=(%d).", counter++);
            ti.lpszText = buf;
            // ti.lpszText = pText;
            SendMessage(hWnd_tip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);

            // ShowWindow(hWnd_tip, SW_SHOW);
            // UpdateWindow(hWnd_tip);
        }
        else
        {
            // Create a tooltip
            // A tooltip control should not have the WS_CHILD style, nor should it
            // have an id, otherwise its behavior will be adversely affected.
            hWnd_tip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, L"",
                                      TTS_NOPREFIX | TTS_ALWAYSTIP,
                                      0, 0, 0, 0, NULL, NULL, NULL, NULL);
            printf("\n[  log  ]tips(%d) created in window (%d)", hWnd_tip, hWnd_host);

            // Associate the tooltip with the button control
            // ti = {0};
            // ti.cbSize = sizeof(ti);
            ti.cbSize = TTTOOLINFOW_V2_SIZE;
            ti.uFlags = TTF_TRACK;
            ti.lpszText = (WCHAR *)L"initialize tip through WM_SIZE.";

            isFlag = SendMessage(hWnd_tip, TTM_ADDTOOL, 0, (LPARAM)&ti);
            printf("\n[  log  ]TTM_ADDTOOL message whether suceessed(%d)", isFlag);

            SendMessage(hWnd_tip, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(400, 100));
            SendMessage(hWnd_tip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
        }
        return TRUE;

    case WM_DESTROY:
        // Clean up window-specific data objects.
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
    printf("\n[  log  ]created a host window (%d)", hWnd_host);

    // ShowWindow(hWnd_host, SW_SHOW);
    // UpdateWindow(hWnd_host);
}

void dispToolTip(WCHAR *pText, BOOL isNew)
{
    if (isNew)
    {
        DestroyWindow(hWnd_tip);
        hWnd_tip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                                  TTS_NOPREFIX | TTS_ALWAYSTIP, // WS_POPUP |
                                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                  NULL, NULL, NULL, NULL);
        printf("[  log  ]created tooltip(%d)  (0x%x)...\n", hWnd_tip, hWnd_tip);

        // ti = {0};
        // ti.cbSize = sizeof(ti);
        ti.cbSize = TTTOOLINFOW_V2_SIZE;
        ti.uFlags = TTF_TRACK;
        // ti.lpszText = (WCHAR *)L"This is a new tooltip.";
        // ti.lpszText = buffer;
        ti.lpszText = pText;

        isFlag = SendMessage(hWnd_tip, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
        printf("[  log  ]TTM_ADDTOOL message whether suceessed(%d)\n", isFlag);

        // SendMessage(hWnd_tip, TTM_ACTIVATE, TRUE, 0);
        // SendMessage(hWnd_tip, TTM_ACTIVATE, false, 0);
        SendMessage(hWnd_tip, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(400, 100));
        SendMessage(hWnd_tip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
    }
    else
    {
        // SendMessage(hWnd_tip, TTM_SETTOOLINFO, 0, (LPARAM)(LPTOOLINFO)&ti);
        // SetWindowPos(hWnd_tip, HWND_TOPMOST, 100, 100, 200, 50, SWP_SHOWWINDOW);
        // SendMessage(hWnd_tip, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(50, 50));
        // SendMessage(hWnd_tip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);

        printf("[  log  ]updated tooltip(%d)  (0x%x)...\n", hWnd_tip, hWnd_tip);

        // SendMessage(hWnd_tip, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
        ti.lpszText = pText;
        // SendMessage(hWnd_tip, TTM_SETTOOLINFO, 0, (LPARAM)(LPTOOLINFO)&ti);
        SendMessage(hWnd_tip, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
        SendMessage(hWnd_tip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
    }
}

void displayinfo()
{
    printf("[  log  ]GetLastError (%d).\n", GetLastError());
    hWnd_tip = FindWindow(L"tooltips_class32", NULL);
    SetWindowLong(hWnd_tip, GWL_EXSTYLE, WS_EX_TOPMOST);
    SetWindowLong(hWnd_tip, GWL_STYLE, TTS_NOPREFIX | TTS_ALWAYSTIP);
    printf("[  log  ]FindWindowEX (%d)(0x%X).GetLastError (%d).\n", hWnd_tip, hWnd_tip, GetLastError());
}

DWORD WINAPI cmdMessageLoop(LPVOID lpParam)
{
    // wsprintf(buf, L"tooltip tooltip tooltip.");
    // dispToolTip(buf, TRUE);

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
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

int main()
{
    // SetConsoleCP(65001);
    // SetConsoleCP(20936);
    // SetConsoleCP(936);
    // SetConsoleOutputCP(65001);
    //SetConsoleOutputCP(20936);
    // SetConsoleOutputCP(936);
    // system("chcp 65001");
    system("chcp 936");

    char command[80];
    char codestr[80];
    int opcode = 10;
    while (opcode != 0)
    {
        // HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
        // WriteConsole(hd, L"hello 控制台\n", sizeof(L"hello 控制台\n"), NULL, NULL);
        // CloseHandle(hd);

        menu_disp();
        scanf("%s", codestr);
        // scanf("%d", &opcode);
        sscanf(codestr, "%d", &opcode);

        switch (opcode)
        {

        case 1:
            printf(" # # # createHostWindow...\n");
            newThread(NULL);

            break;
        case 2:
            printf(" # # # InitCommonControlsEx...\n");
            // INITCOMMONCONTROLSEX InitCtrls;
            // InitCtrls.dwSize = sizeof(InitCtrls);
            // InitCtrls.dwICC = ICC_WIN95_CLASSES;
            // InitCommonControlsEx(&InitCtrls);
            break;
        case 3:
            printf(" # # # CreateThread...\n");
            hThreadCmd = CreateThread(
                NULL,      // default security attributes
                0,         // use default stack size
                newThread, // thread function name
                NULL,      // argument to thread function
                0,         // use default creation flags
                NULL);     // returns the thread identifier

            break;

        case 4:
            printf(" # # # new tooltip ...\n");

            wsprintf(buf, L"tooltip tooltip tooltip.");
            dispToolTip(buf, TRUE);

            break;

        case 5:
            printf(" # # # update tooltip...\n");

            wsprintf(buf, L"counter=(%d).", counter++);
            dispToolTip(buf, FALSE);
            break;

        case 6:
            // displayinfo();
            printf(" # # # display infomation...\n");
            // HMODULE hModule = GetModuleHandle(NULL);
            printf("[  log  ]GetModuleHandle(%d)(0x%x).\n", GetModuleHandle(NULL), GetModuleHandle(NULL));
            printf("[  log  ]enter point main(%d)(0x%x).\n", main, main);

            //const TCHAR *szBuf= L"找不到记事本\n";
            //printf("[  log  ]%s%c",szBuf,szBuf);
            wprintf(L"一些汉字\n");
            printf("一些汉字\n");
            printf("汉字汉字汉字一些汉字\n");
            printf(" # # # ####一些汉字###########################\n");
            break;

        case 7:
            sprintf(command, "tasklist /m /fi \"PID eq %d\"", GetCurrentProcess());
            system(command);
            break;
        case 8:
            sprintf(command, "tasklist /m /fi \"modules eq Comctl32.dll\"");
            system(command);
            break;

        case 9:
            system("cls");
            printf(" # # #  ----  ----  screen cleared   ----  -----\n");
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
    printf(" # # # .\n");

    // cmdMessageLoop();
    return 1;
}
