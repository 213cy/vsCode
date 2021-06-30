#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <Windows.h>
#include "stdio.h"
#include "mex.h"
#include "mat.h"
#include "matrix.h"
#define EXPORTED_FUNCTION __declspec(dllexport)
// #include "getimage.h"

#define image_width 256
#define image_height 210
int cx = image_width, cy = image_height;
// const mwSize dims[] = {(mwSize)cy, (mwSize)cx, 3};
const size_t dims[3] = {image_height, image_width, 3};

#define WAITFORCAPTUER_THREAD_MSG WM_USER + 100

DWORD captureTID;
HWND hwindowWar3;

WCHAR *szName = (WCHAR *)L"War3ImageFileMapping";
HANDLE hMapFile;
char *pImageMap;

HANDLE hCaptureDoneEvent;

char img[10] = {'\x12', '\x13', '\x14', '\x15', '\x16', 1, 2, 3, 4, 5};


EXPORTED_FUNCTION void commandMove(double x, double y, double z)
{
    SetCursorPos(x, y);
    Sleep(z);
    //SetCursorPos(1440, 900);
    SetCursorPos(700, 400);
}
EXPORTED_FUNCTION void commandClick(double x, double y)
{
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // 按下
    // Sleep(100);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   //弹起
}

EXPORTED_FUNCTION void initFetchImage(double x, double z)
{
    /* get TID */
    OpenClipboard(NULL);
    HGLOBAL hMem = GetClipboardData(CF_TEXT);
    LPDWORD lpMem = (DWORD *)GlobalLock(hMem);
    captureTID = *lpMem;
    hwindowWar3 = *(HWND *)(lpMem + 1);
    GlobalUnlock(hMem);
    CloseClipboard();
    mexPrintf("get captrue thread info from clipboard: TID=(%d), war3 window=(0x%x)\n",
              captureTID, hwindowWar3);

    hMapFile = OpenFileMapping(
        FILE_MAP_READ, //    FILE_MAP_ALL_ACCESS,   // read/write access
        FALSE,         // do not inherit the name
        szName);       // name of mapping object
    if (hMapFile == NULL)
    {
        mexPrintf("Could not open file mapping object (%d).\n", GetLastError());
        mexErrMsgIdAndTxt("MATLAB:mexfunction:OpenFileMapping",
                          "Cannot OpenFileMapping.\n");
    }
    pImageMap = (char *)MapViewOfFile(hMapFile,      // handle to map object
                                      FILE_MAP_READ, //  FILE_MAP_ALL_ACCESS,  // read/write permission
                                      0,
                                      0,
                                      0);
    if (pImageMap == NULL)
    {
        mexPrintf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
        mexErrMsgIdAndTxt("MATLAB:mexfunction:MapViewOfFile",
                          "Cannot MapViewOfFile.\n");
    }

    hCaptureDoneEvent = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, L"CaptureDoneEvent");
    mexPrintf("OpenedEvent : (%x).\n", hCaptureDoneEvent);
}
EXPORTED_FUNCTION void endFetchImage()
{
    CloseHandle(hCaptureDoneEvent);

    UnmapViewOfFile(pImageMap);
    CloseHandle(hMapFile);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int a = mxGetScalar(prhs[0]);
    double *b = mxGetPr(prhs[0]);
    mexPrintf("get parameter from input: double %f %d | int %f %d\n", b[0], b[0], a, a);

    //////////////////////////////////////////
    //
    //////////////////////////////////////////
    if (!ResetEvent(hCaptureDoneEvent))
        MessageBox(NULL, L"Sets event object to nonsignaled failed!",
                   L"title", MB_OK | MB_ICONWARNING);

    PostThreadMessage(captureTID, WAITFORCAPTUER_THREAD_MSG, (WPARAM)NULL, (LPARAM)NULL);

    plhs[0] = mxCreateNumericArray(3, dims, mxUINT8_CLASS, mxREAL);
    char *pimageOut = (char *)mxGetData(plhs[0]);

    DWORD dwWaitResult = WaitForSingleObject(hCaptureDoneEvent, 5000);
    mexPrintf("WaitForSingleObject return code: 0x%x\n", dwWaitResult);

    int i, j, k;
    for (k = 0; k < 3; k++)
        for (j = 0; j < cy; j++)
            for (i = 0; i < cx; i++)
            {
                pimageOut[i * cy + j + k * cx * cy] = pImageMap[(j * cx + i) * 3 + 2 - k];
            }

    mexPrintf("hwindowWar3(end)= %d\n", hwindowWar3);
}
