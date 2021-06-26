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
int x1, y1;

HDC hdcScreen;
HDC hdcMemDC;
HBITMAP hbmScreen;

HGDIOBJ hPen;
HGDIOBJ hPenOld;

BITMAPINFOHEADER bi = {0};
DWORD dwBmpSize;

HANDLE hDIB = NULL;
// SIZE_T dwBmpSize;
WCHAR *szName = L"War3ImageFileMapping";
HANDLE hMapFile;

char *lpwar3image;
char *lpbitmap;

void initCapture()
{
    hWndScreen = FindWindow(L"OsWindow", L"Warcraft III");
    // MessageBox(NULL, L"Found Warcraft III", L"Succeed", MB_OK);
    if (hWndScreen == NULL)
    {
        // hWndScreen = GetDesktopWindow();
        x1 = 133, y1 = 581;
    }
    else
        x1 = 133, y1 = 681;

    // Retrieve the device context handle to get the client area of war3 window.
    // hdcScreen = GetDC(hWndScreen);
    hdcScreen = GetWindowDC(hWndScreen);
    // Create a compatible bitmap from the Window DC.
    hbmScreen = CreateCompatibleBitmap(hdcScreen, cx, cy);

    // Create a compatible DC, which is used in a BitBlt from the window DC.
    hdcMemDC = CreateCompatibleDC(hdcScreen);
    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC, hbmScreen);

    hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

    // Get the BITMAP from the HBITMAP.
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = cx;
    //  If biHeight is negative, the bitmap is a top-down DIB
    // and its origin is the upper-left corner.
    bi.biHeight = -cy;
    bi.biPlanes = 1;
    bi.biBitCount = 24; // try 24 and 3 * cx * cy;
    bi.biCompression = BI_RGB;

    // GetDIBits remarks: "The scan lines must be aligned on a DWORD ..."
    // See https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getdibits#remarks
    // 即需要保证 (cx * bi.biBitCount / 32) 为一个整数
    // 当 bi.biBitCount=24 时,要求cx能被4整除
    // 综合考虑 令 cx =256
    dwBmpSize = 3 * cx * cy; // bi.biBitCount/8 =3

    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE, // use paging file
        NULL,                 // default security
        PAGE_READWRITE,       // read/write access
        0,                    // maximum object size (high-order DWORD)
        dwBmpSize,            // maximum object size (low-order DWORD)
        szName);              // name of mapping object
    if (hMapFile == NULL)
    {
        printf("Could not create file mapping object (%d).\n", GetLastError());
    }

    lpwar3image = (char *)MapViewOfFile(hMapFile,            // handle to map object
                                        FILE_MAP_ALL_ACCESS, // read/write permission
                                        0,
                                        0,
                                        dwBmpSize);

    if (lpwar3image == NULL)
    {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
    }

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

    UnmapViewOfFile(lpwar3image);
    CloseHandle(hMapFile);

    DeleteObject(hPen);

    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(hWndScreen, hdcScreen);
}

void reagrrange()
{
    int i, j, k;

    HANDLE hMemTmp = GlobalAlloc(GHND, dwBmpSize);
    char *mTmp = (char *)GlobalLock(hMemTmp);
    printf("\n(%x) (%x) ", hMemTmp, mTmp);

    memcpy(mTmp, lpbitmap, dwBmpSize);

    for (k = 0; k < 3; k++)
        for (j = 0; j < cy; j++)
            for (i = 0; i < cx; i++)
            {
                lpbitmap[i * cy + j + k * cx * cy] = mTmp[(j * cx + i) * 3 + 2 - k];
            }

    FILE *file;
    fopen_s(&file, "data2.bin", "wb");
    fwrite(lpbitmap, 1, dwBmpSize, file);
    fclose(file);

    GlobalUnlock(hMemTmp);
    GlobalFree(hMemTmp);
}

//   FUNCTION: CaptureAnImage(HWND hWnd)
//   PURPOSE: Captures a screenshot into a window ,and then saves it in  file.
int CaptureAnImage(HWND hWndxxx)
{
    initCapture();
    DWORD dwBytesWritten = 0;

    hPenOld = SelectObject(hdcScreen, hPen);
    MoveToEx(hdcScreen, x1 - 1, y1 - 1, (LPPOINT)NULL);
    LineTo(hdcScreen, x1 - 1, y1 + 1 + cy);
    LineTo(hdcScreen, x1 + 1 + cx, y1 + 1 + cy);
    LineTo(hdcScreen, x1 + 1 + cx, y1 - 1);
    LineTo(hdcScreen, x1 - 1, y1 - 1);
    SelectObject(hdcScreen, hPenOld);

    // Bit block transfer into our compatible memory DC.
    // so the compatible bitmap contain the region of war3 screen
    BitBlt(hdcMemDC, 0, 0, cx, cy, hdcScreen, x1, y1, SRCCOPY);
    // Gets the "bits" from the bitmap, and copies them into a buffer
    // that's pointed to by lpbitmap.
    GetDIBits(hdcMemDC, hbmScreen, 0, (UINT)cy,
              lpwar3image, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    GetDIBits(hdcMemDC, hbmScreen, 0, (UINT)cy,
              lpbitmap, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    printf("\n # # # bi.biBitCount(%d)  bi.biSizeImage(%d).", bi.biBitCount, bi.biSizeImage);

    FILE *file;
    fopen_s(&file, "data.bin", "wb");
    fwrite(lpbitmap, 1, dwBmpSize, file);
    fclose(file);

    // A file is created, this is where we will save the screen capture.
    HANDLE hFile = CreateFile(L"windata.bin",
                              GENERIC_WRITE,
                              0,
                              NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
    // Close the handle for the file that was created.
    CloseHandle(hFile);
    printf("\n # # # (%d)Bytes Written in file windata.bin.", dwBytesWritten);

    reagrrange();

    return 0;
}

void AutoPilot(WORD x, WORD y, UINT WinM)
{
    PostMessage(hWndScreen, WinM, 0, MAKELPARAM(x, y));
    SendMessage(hWndScreen, WM_LBUTTONDOWN, 0, MAKELPARAM(x, y));
    SendMessage(hWndScreen, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
}

void CaptureAnImage2()
{
    int recnum;
    int i, j, k;
    char *pchar, *mloc;

    HWND hwin;
    HDC dc, memdc;
    HBITMAP hbitm;

    hwin = GetDesktopWindow();
    dc = GetWindowDC(hwin);
    printf("\ncx: (%d) cy: (%d)", cx, cy);
    hbitm = CreateCompatibleBitmap(dc, cx, cy);

    memdc = CreateCompatibleDC(dc);
    SelectObject(memdc, hbitm);

    //Copy color data for the entire display into a
    //bitmap that is selected into a compatible DC.
    BitBlt(memdc, 0, 0, cx, cy, dc, 0, 0, SRCCOPY);

    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    mloc = (char *)GlobalLock(hDIB);

    recnum = GetDIBits(memdc, hbitm, 0, cy, mloc, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    printf("\n # # # bi.biBitCount(%d)  bi.biSizeImage(%d).", bi.biBitCount, bi.biSizeImage);
    printf("Copyed %d lines !\n", recnum);

    FILE *file;
    fopen_s(&file, "data.txt", "wb");
    fwrite(mloc, 1, dwBmpSize, file);
    fclose(file);
    // for (k = 0; k < 3; k++)
    //     for (j = 0; j < cy; j++)
    //         for (i = 0; i < cx; i++)
    //         {
    //             pchar[i * cy + j + k * cx * cy] = mloc[(j * cx + i) * 3 + 2 - k];
    //         }

    DeleteDC(memdc);
    DeleteObject(hbitm);
    ReleaseDC(hwin, dc);
}
