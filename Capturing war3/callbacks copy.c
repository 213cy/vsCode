#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <stdio.h>
#include "callbacks.h"

// #define image_width 255
#define image_width 256
#define image_height 210

// int cx = 255, cy = 210;
int cx = image_width, cy = image_height;
int x1;
int y1;

HWND hWndScreen;
HDC hdcScreen;
HDC hdcMemDC;

HBITMAP hbmScreen = NULL;

HGDIOBJ hPen;
HGDIOBJ hPenOld;

BITMAPINFOHEADER bi = {0};

HANDLE hDIB = NULL;

// SIZE_T dwBmpSize;
DWORD dwBmpSize;
char *lpbitmap = NULL;

void initCapture()
{
    hWndScreen = FindWindow(L"OsWindow", L"Warcraft III");
    // MessageBox(NULL, L"Found Warcraft III", L"Succeed", MB_OK);
    if (hWndScreen == NULL)
    {
        hWndScreen = GetDesktopWindow();
        x1 = 133, y1 = 581;
    }
    else
        x1 = 133, y1 = 681;

    // Retrieve the device context handle to get the client area of war3 window.
    // hdcScreen = GetDC(hWndScreen);
    hdcScreen = GetWindowDC(hWndScreen);
    // Create a compatible DC, which is used in a BitBlt from the window DC.
    hdcMemDC = CreateCompatibleDC(hdcScreen);
    // Create a compatible bitmap from the Window DC.
    hbmScreen = CreateCompatibleBitmap(hdcScreen, cx, cy);
    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC, hbmScreen);

    hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

    // Get the BITMAP from the HBITMAP.
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = cx;
    bi.biHeight = cy;
    bi.biPlanes = 1;
    bi.biBitCount = 24; // try 24 and 3 * cx * cy;
    bi.biCompression = BI_RGB;

// GetDIBits remarks: "The scan lines must be aligned on a DWORD ..."
// See https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getdibits#remarks
// 即需要保证 (cx * bi.biBitCount / 32) 为一个整数
// 当 bi.biBitCount=24 时,要求cx能被4整除
// 综合考虑 令 cx =256
    dwBmpSize = 3 * cx * cy;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc
    // have greater overhead than HeapAlloc.
    hDIB = GlobalAlloc(GHND, dwBmpSize);
    lpbitmap = (char *)GlobalLock(hDIB);
}

// Clean up.
void endCapture()
{
    // Unlock and Free the DIB from the heap.
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    DeleteObject(hPen);

    DeleteObject(hbmScreen);
    // DeleteObject(hdcMemDC);
    ReleaseDC(hWndScreen, hdcScreen);
}

//   FUNCTION: CaptureAnImage(HWND hWnd)
//   PURPOSE: Captures a screenshot into a window ,and then saves it in a .bmp file.
//      Note: This function attempts to create a file called captureqwsx.bmp
int CaptureAnImage(HWND hWndxxx)
{
    initCapture();
    DWORD dwBytesWritten = 0;

    // Bit block transfer into our compatible memory DC.
    // The source DC is the war3 screen
    BitBlt(hdcMemDC, 0, 0, cx, cy, hdcScreen, x1, y1, SRCCOPY);

    hPenOld = SelectObject(hdcScreen, hPen);
    MoveToEx(hdcScreen, x1 - 1, y1 - 1, (LPPOINT)NULL);
    LineTo(hdcScreen, x1 - 1, y1 + 1 + cy);
    LineTo(hdcScreen, x1 + 1 + cx, y1 + 1 + cy);
    LineTo(hdcScreen, x1 + 1 + cx, y1 - 1);
    LineTo(hdcScreen, x1 - 1, y1 - 1);
    SelectObject(hdcScreen, hPenOld);

    ///////////////////////////////////////////////////////////////

    // Gets the "bits" from the bitmap, and copies them into a buffer
    // that's pointed to by lpbitmap.
    // GetDIBits(hdcScreen, hbmScreen, 0, (UINT)cy,
    //           lpbitmap, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    // GetDIBits(hdcScreen, hbmScreen, 0, (UINT)cy,
    //           lpbitmap, (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // printf("\n # # # bi.biBitCount(%d)  bi.biSizeImage(%d).", bi.biBitCount, bi.biSizeImage);

    GetDIBits(hdcMemDC, hbmScreen, 0, (UINT)cy,
              lpbitmap, (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    printf("\n # # # bi.biBitCount(%d)  bi.biSizeImage(%d).", bi.biBitCount, bi.biSizeImage);

    FILE *file;
    fopen_s(&file, "data.txt", "w+b");
    fwrite(lpbitmap, 1, dwBmpSize, file);
    fclose(file);

    // A file is created, this is where we will save the screen capture.
    HANDLE hFile = CreateFile(L"capturedat.bin",
                              GENERIC_WRITE,
                              0,
                              NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
    // Close the handle for the file that was created.
    CloseHandle(hFile);
    printf("\n # # # dwBytesWritten(%d).", dwBytesWritten);

    return 0;
}

void CaptureAnImage3()
{
    int recnum;
    int i, j, k;
    char *pchar, *mloc;
    ;

    HWND hwin;
    HDC dc, memdc;
    RECT rect;
    HBITMAP hbitm, hold;

    hwin = GetDesktopWindow();
    dc = GetWindowDC(hwin);
    // GetWindowRect(hwin, &rect);
    // cx = rect.right - rect.left;
    // cy = rect.bottom - rect.top;
    printf("\ncx: (%d) cy: (%d)", cx, cy);
    memdc = CreateCompatibleDC(dc);
    printf("\nHandles: %x %x %x", hwin, dc, memdc);
    hbitm = CreateCompatibleBitmap(dc, cx, cy);

    hold = SelectObject(memdc, hbitm);

    //Copy color data for the entire display into a
    //bitmap that is selected into a compatible DC.

    BitBlt(memdc, 0, 0, cx, cy, dc, 0, 0, SRCCOPY);

    bi.biSize = sizeof(BITMAPINFOHEADER);
    // bi.biWidth = cx;
    // bi.biHeight = -cy;
    // bi.biPlanes = 1;
    bi.biBitCount = 24;
    // bi.biCompression = BI_RGB;
    // bi.biSizeImage = 0;
    // bi.biXPelsPerMeter = 0;
    // bi.biYPelsPerMeter = 0;
    // bi.biClrUsed = 0;
    // bi.biClrImportant = 0;

    HANDLE hDIB = GlobalAlloc(GHND, cx * cy * 3);
    mloc = (char *)GlobalLock(hDIB);

    recnum = GetDIBits(memdc, hbitm, 0, cy, mloc, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    printf("\n # # # bi.biBitCount(%d)  bi.biSizeImage(%d).", bi.biBitCount, bi.biSizeImage);

    printf("Copyed %d lines %d!\n", recnum, hbitm);

    FILE *file;
    fopen_s(&file, "data.txt", "w+b");
    fwrite(mloc, 1, cx * cy * 3, file);
    fclose(file);
    // for (k = 0; k < 3; k++)
    //     for (j = 0; j < cy; j++)
    //         for (i = 0; i < cx; i++)
    //         {
    //             pchar[i * cy + j + k * cx * cy] = mloc[(j * cx + i) * 3 + 2 - k];
    //         }

    SelectObject(memdc, hold);
    DeleteDC(memdc);
    ReleaseDC(hwin, dc);
    DeleteObject(hbitm);
}

void CaptureAnImage2()
{
    int cx, cy, recnum;
    int i, j, k;
    char *pchar, *mloc;

    HWND hwin, hactw;
    HDC dc, memdc;
    RECT rect;
    HBITMAP hbitm, hold;
    BITMAPINFOHEADER binfoh;

    hactw = GetForegroundWindow();
    hwin = GetDesktopWindow();
    dc = GetWindowDC(hwin);
    // GetWindowRect(hwin, &rect);
    cx = 255;
    cy = 210;
    printf("cx: %d cy: %d!\n", cx, cy);
    memdc = CreateCompatibleDC(dc);
    printf("Handles: %d %d %d!\n", hwin, dc, memdc);
    hbitm = CreateCompatibleBitmap(dc, cx, cy);

    hold = SelectObject(memdc, hbitm);

    //Copy color data for the entire display into a
    //bitmap that is selected into a compatible DC.

    BitBlt(memdc, 0, 0, cx, cy, dc, 0, 0, SRCCOPY);

    binfoh.biSize = sizeof(BITMAPINFOHEADER);
    binfoh.biWidth = cx;
    binfoh.biHeight = -cy;
    binfoh.biPlanes = 1;
    binfoh.biBitCount = 24;
    binfoh.biCompression = BI_RGB;
    binfoh.biCompression = BI_BITFIELDS;
    binfoh.biSizeImage = 0;
    binfoh.biXPelsPerMeter = 0;
    binfoh.biYPelsPerMeter = 0;
    binfoh.biClrUsed = 0;
    binfoh.biClrImportant = 0;
    // mloc = (char *)mxMalloc();

    HANDLE hDIB = GlobalAlloc(GHND, cx * cy * 3);
    mloc = (char *)GlobalLock(hDIB);

    recnum = GetDIBits(memdc, hbitm, 0, cy, mloc, (BITMAPINFO *)&binfoh, DIB_RGB_COLORS);
    printf("\n # # # binfoh.biBitCount(%d)  binfoh.biSizeImage(%d).", binfoh.biBitCount, binfoh.biSizeImage);
    printf("Copyed %d lines %d!\n", recnum, hbitm);


    FILE *file;
    fopen_s(&file, "data.txt", "w+b");
    fwrite(mloc, 1, cx * cy * 3, file);
    fclose(file);
    // for (k = 0; k < 3; k++)
    //     for (j = 0; j < cy; j++)
    //         for (i = 0; i < cx; i++)
    //         {
    //             pchar[i * cy + j + k * cx * cy] = mloc[(j * cx + i) * 3 + 2 - k];
    //         }
    // mxFree(mloc);
    SelectObject(memdc, hold);
    DeleteDC(memdc);
    ReleaseDC(hwin, dc);
    DeleteObject(hbitm);
}