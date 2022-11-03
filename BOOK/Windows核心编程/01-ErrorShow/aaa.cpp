#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int nCmdShow)
{
    INT_PTR a = DialogBoxParam(hInstance, MAKEINTRESOURCE(101),
                               NULL, WindowProc, nCmdShow);
//#pragma message(files. This may prevent the project from building correctly.)
#define chSTR2(x) #x
#define chSTR(x)  chSTR2(x)
#pragma message( chSTR(__LINE__)) 

    // // Register the window class.
    // const wchar_t CLASS_NAME[] = L"Sample Window Class";

    // WNDCLASS wc = {};

    // wc.lpfnWndProc = WindowProc;
    // wc.hInstance = hInstance;
    // wc.lpszClassName = CLASS_NAME;

    // RegisterClass(&wc);

    // // Create the window.

    // HWND hwnd = CreateWindowEx(
    //     0,                           // Optional window styles.
    //     CLASS_NAME,                  // Window class
    //     L"Learn to Program Windows", // Window text
    //     WS_OVERLAPPEDWINDOW,         // Window style

    //     // Size and position
    //     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

    //     NULL,      // Parent window
    //     NULL,      // Menu
    //     hInstance, // Instance handle
    //     NULL       // Additional application data
    // );

    // if (hwnd == NULL)
    // {
    //     return 0;
    // }

    // ShowWindow(hwnd, nCmdShow);

    // // Run the message loop.
    // MSG msg = {};
    // while (GetMessage(&msg, NULL, 0, 0))
    // {
    //     TranslateMessage(&msg);
    //     DispatchMessage(&msg);
    // }

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
    {
        // MessageBox(NULL, TEXT("Visual C++") , TEXT("窗口消息"), 0);
        SetDlgItemInt(hwnd, 1000, (UINT)wParam, FALSE);

        SetForegroundWindow(hwnd);
    }
        return 0;
    }

    return 0; //DefWindowProc(hwnd, uMsg, wParam, lParam);

}
