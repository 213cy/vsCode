#include "windows.h"

/* You should define DLL_EXPORTS *only* when building the DLL. */
#ifdef DLL_EXPORTS
#define EXPORTSAPI __declspec( dllexport )
#else
#define EXPORTSAPI __declspec( dllimport )
#endif

/* Define calling convention in one place, for convenience. */
#define EXPORTSCALL __cdecl
/* Make sure functions are exported with C linkage under C++ compilers. */
#ifdef __cplusplus
extern "C"
{
#endif

int APIENTRY EXPORTSAPI DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved);
// LRESULT CALLBACK EXPORTSAPI  EXPORTSCALL MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

/* Declare function using the above definitions. */
DWORD EXPORTSAPI  EXPORTSCALL sethook();
VOID EXPORTSAPI  EXPORTSCALL unhook();
// int EXPORTSAPI  EXPORTSCALL resetCounter();
VOID EXPORTSAPI  EXPORTSCALL dispToolTip(WCHAR *pText);

extern EXPORTSAPI int actionCounter;

#ifdef __cplusplus
} // __cplusplus defined.
#endif