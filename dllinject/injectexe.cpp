#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <Windows.h>
#include <stdio.h>
//#include <string.h>
//#include <tchar.h>

int main()
{
    const char *buffer = "C:\\Users\\Administrator\\injectdll.dll";
    //char *buffer = "C:\\drivers\\dllinject.dll";

    /*
* Get process handle passing in the process ID.
*/
    //int procID = 1056;
    HWND hWnd = FindWindow(L"Notepad",L"Test.txt - 记事本");
    DWORD procID;
    DWORD  aaa = GetWindowThreadProcessId( hWnd,  &procID);
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

    //HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 1056);
    if (process == NULL)
    {
        printf("Error: the specified process couldn't be found.\n");
    }

    /*
* Get address of the LoadLibrary function.
*/
    LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
    if (addr == NULL)
    {
        printf("Error: the LoadLibraryA function was not found inside kernel32.dll library.\n");
    }

    /*
* Allocate new memory region inside the process's address space.
*/
    LPVOID arg = (LPVOID)VirtualAllocEx(process, NULL, strlen(buffer), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (arg == NULL)
    {
        printf("Error: the memory could not be allocated inside the chosen process.\n");
    }

    /*
* Write the argument to LoadLibraryA to the process's newly allocated memory region.
*/
    int n = WriteProcessMemory(process, arg, buffer, strlen(buffer), NULL);
    if (n == 0)
    {
        printf("Error: there was no bytes written to the process's address space.\n");
    }

    /*
* Inject our DLL into the process's address space.
*/
    HANDLE threadID = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, NULL, NULL);
    if (threadID == NULL)
    {
        printf("Error: the remote thread could not be created.\n");
    }
    else
    {
        printf("Success: the remote thread(0x%p) was successfully created.\n",addr);
    }

    /*
* Close the handle to the process, becuase we've already injected the DLL.
*/
    CloseHandle(process);
    getchar();

    return 0;
}