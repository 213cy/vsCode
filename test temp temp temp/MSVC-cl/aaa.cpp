#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#pragma comment(lib,"User32.lib")

#include <windows.h>
#include <stdio.h>
#include <CommCtrl.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void displayinfo(HWND hWnd)
{
    SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
    SetWindowLong(hWnd, GWL_STYLE, TTS_NOPREFIX | TTS_ALWAYSTIP);

    // SetForegroundWindow(hwnd);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    printf("\n[  log  ]hInstance(0x%x).", hInstance);
    printf("\n[  log  ]GetModuleHandle(0x%x).", GetModuleHandle(NULL));

    // Register the window class.
    const WCHAR CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"can't register window class", L"Error", MB_OK);
    }

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                           // Optional window styles.
        CLASS_NAME,                  // Window class
        L"Learn to Program Windows", // Window text
        WS_OVERLAPPEDWINDOW,         // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (!hwnd)
    {
        printf("\n [ error ] CreateWindow failed, last error code : %d", GetLastError());
        getchar();
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //CreateCaret(hWnd,NULL,20,100);
    //ShowCaret(hWnd);

    // Run the message loop.
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // All painting occurs here, between BeginPaint and EndPaint.
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(14));

        EndPaint(hwnd, &ps);
    }
        return 0;
    case WM_INITDIALOG:
        printf("\n [ WindProc ] WM_INITDIALOG message ");

        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
