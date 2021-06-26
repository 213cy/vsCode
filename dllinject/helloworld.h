#include "windows.h"

#ifndef _MYCODE_H_
#define _MYCODE_H_

#ifdef DLLDEMO1_EXPORTS
#define EXPORTS_DEMO __declspec( dllexport )
#else
#define EXPORTS_DEMO __declspec( dllimport )
#endif

int APIENTRY EXPORTS_DEMO DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved);
#endif