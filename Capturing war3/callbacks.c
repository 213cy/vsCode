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
int cx1, cy1;

HWND hWndScreen;
HDC hdcScreen;
HDC hdcMemDC;
HBITMAP hbmScreen;

HGDIOBJ hPen;

BITMAPINFOHEADER bi = {0};
DWORD dwBmpSize;
// SIZE_T dwBmpSize;

WCHAR *szName = L"War3ImageFileMapping";
HANDLE hMapFile;
char *lpwar3image;

HANDLE hCaptureDoneEvent;

HWND initCapture()
{
    hWndScreen = FindWindow(L"OsWindow", L"Warcraft III");
    // MessageBox(NULL, L"Found Warcraft III", L"Succeed", MB_OK);
    if (hWndScreen == NULL)
    {
        // hWndScreen = GetDesktopWindow();
        cx1 = 133, cy1 = 581;
    }
    else
        cx1 = 133, cy1 = 681;

    // Retrieve the device context handle to get the client area of war3 window.
    hdcScreen = GetWindowDC(hWndScreen); // hdcScreen = GetDC(hWndScreen);
    printf("\n [init] GetWindowDC : (0x%x)", hdcScreen);
    ReleaseDC(hWndScreen, hdcScreen);
    hdcScreen = GetDC(hWndScreen);
    printf("\n [init] GetDC : (0x%x)", hdcScreen);
    ReleaseDC(hWndScreen, hdcScreen);
    hdcScreen = GetWindowDC(hWndScreen); // hdcScreen = GetDC(hWndScreen);
    printf("\n [init] GetWindowDC : (0x%x)", hdcScreen);

    // Create a compatible bitmap from the Window DC.
    hbmScreen = CreateCompatibleBitmap(hdcScreen, cx, cy);
    // Create a compatible DC, which is used in a BitBlt from the window DC.
    hdcMemDC = CreateCompatibleDC(hdcScreen);
    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC, hbmScreen);

    hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = cx;
    //  If biHeight is negative, the bitmap is a top-down DIB
    // and its origin is the upper-left corner.
    bi.biHeight = -cy; // -cy
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
    printf("\n [init] Create file mapping object :(0x%x).", hMapFile);

    lpwar3image = (char *)MapViewOfFile(hMapFile,            // handle to map object
                                        FILE_MAP_ALL_ACCESS, // read/write permission
                                        0,
                                        0,
                                        dwBmpSize);

    hCaptureDoneEvent = CreateEvent(
        NULL,                 // default security attributes
        FALSE,                // manual-reset event
        TRUE,                 // initial state 
        L"CaptureDoneEvent"); // object name
    printf("\n [init] CreatedEvent : (0x%x)", hCaptureDoneEvent);

    return hWndScreen;
}

// Clean up.
void endCapture()
{
    CloseHandle(hCaptureDoneEvent);

    UnmapViewOfFile(lpwar3image);
    CloseHandle(hMapFile);

    DeleteObject(hPen);

    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(hWndScreen, hdcScreen);
}
void endCapture2()
{
    UnmapViewOfFile(lpwar3image);
    CloseHandle(hMapFile);
    ReleaseDC(hWndScreen, hdcScreen);
}

int CaptureAnImage(int parm1)
{

    HGDIOBJ hPenOld = SelectObject(hdcScreen, hPen);
    MoveToEx(hdcScreen, cx1 - 1, cy1 - 1, (LPPOINT)NULL);
    LineTo(hdcScreen, cx1 - 1, cy1 + 1 + cy);
    LineTo(hdcScreen, cx1 + 1 + cx, cy1 + 1 + cy);
    LineTo(hdcScreen, cx1 + 1 + cx, cy1 - 1);
    LineTo(hdcScreen, cx1 - 1, cy1 - 1);
    SelectObject(hdcScreen, hPenOld);

    // Bit block transfer into our compatible memory DC.
    // so the compatible bitmap contain the region of war3 screen
    BitBlt(hdcMemDC, 0, 0, cx, cy, hdcScreen, cx1, cy1, SRCCOPY);
    // Gets the "bits" from the bitmap, and copies them into a buffer
    // that's pointed to by lpwar3image.
    int scanline = GetDIBits(hdcMemDC, hbmScreen, 0, (UINT)cy,
                             lpwar3image, (BITMAPINFO *)&bi, DIB_RGB_COLORS);
    printf("\n [getbit] Copyed (%d) lines !(%d) bytes ! (%d) bits per pixel !",
           scanline, bi.biSizeImage, bi.biBitCount);

    SetEvent(hCaptureDoneEvent);

    return 0;
}

void ImageSave()
{
    DWORD dwBytesWritten;
    // save the original capture.
    HANDLE hFile = CreateFile(L"image_original.bin",
                              GENERIC_WRITE,
                              0,
                              NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(hFile, (LPSTR)lpwar3image, dwBmpSize, &dwBytesWritten, NULL);
    // Close the handle for the file that was created.
    CloseHandle(hFile);
    printf("\n [save] (%d)Bytes Written in file image_original.bin", dwBytesWritten);

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc
    // have greater overhead than HeapAlloc.
    HANDLE hMemTmp = GlobalAlloc(GHND, dwBmpSize);
    char *lpTmp = (char *)GlobalLock(hMemTmp);

    int i, j, k;
    for (k = 0; k < 3; k++)
        for (j = 0; j < cy; j++)
            for (i = 0; i < cx; i++)
            {
                lpTmp[i * cy + j + k * cx * cy] = lpwar3image[(j * cx + i) * 3 + 2 - k];
            }
    FILE *file;
    fopen_s(&file, "image_arranged.bin", "wb");
    dwBytesWritten = fwrite(lpTmp, 1, dwBmpSize, file);
    fclose(file);
    printf("\n [save] (%d)Bytes Written in file image_arranged.bin", dwBytesWritten);

    // Unlock and Free the hMemTmp from the heap.
    GlobalUnlock(hMemTmp);
    GlobalFree(hMemTmp);
}
