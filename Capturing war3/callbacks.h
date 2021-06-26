#include <windows.h>

HWND hWndScreen;

int CaptureAnImage(HWND hWnd);
void CaptureAnImage2();
void initCapture();
void endCapture();
void AutoPilot(WORD x,WORD y, UINT WinM);