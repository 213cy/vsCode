#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <Windows.h>
#include <stdio.h>
#include <locale.h>

HANDLE console;
DWORD threadId;
DWORD processId;

void error_message(const char *msg)
{
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
    fprintf(stderr, "ERROR : ");
    fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "     (last error code = %d)\n", GetLastError());
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN |
                                         FOREGROUND_BLUE);
}


int newNotepadProcess()
{
    const char *command = "notepad.exe";
    if ((intptr_t)ShellExecuteA(NULL, "open", command, NULL, NULL, SW_SHOWNORMAL)<=32)
    {
        error_message("failed to create Notepad process.");
        return 1;
    }

    HWND hWnd;
    const wchar_t *windowTitle = L"无标题 - 记事本"; // Specific to Chinese version of Notepad
    for (int i = 0; i < 5; ++i) { // Try 10 times with a delay
        Sleep(100); // Wait for 500 milliseconds
        hWnd = FindWindowW(L"Notepad", windowTitle);
        if (hWnd != NULL) {
            threadId = GetWindowThreadProcessId(hWnd, &processId);
            return 0;
        }
    }
    
    error_message("Unable to get the Notepad process id.");
    return 1;
}

int hookdll()
{
    TCHAR szModuleName[MAX_PATH];
    wchar_t dllFileName[MAX_PATH];
    HMODULE hModule = GetModuleHandle(NULL); // NULL means the current process
    if (hModule != NULL)
    {
        // Get the full path of the executable
        if (GetModuleFileName(hModule, szModuleName, sizeof(szModuleName) / sizeof(TCHAR)))
        {
            // Print the path
            wcscpy(dllFileName, szModuleName);
            wchar_t *pwc = wcsrchr(dllFileName, L'\\');
            *pwc = L'\0';
            wcscat(dllFileName, L"\\injectdll.dll");
            printf("Executable path : %ls\n", szModuleName);
            printf("dll path : %ls\n", dllFileName);
        }
    }

    SIZE_T dwSize = wcslen(dllFileName) * sizeof(wchar_t);
    // Get process handle passing in the process ID.
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    printf("Target process handle : %d\n", process);

    //   Get address of the LoadLibrary function.
    LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
    printf("Inside kernel32.dll the LoadLibraryA function address : %#p\n", addr);

    //   Allocate new memory region inside the process's address space.
    DWORD flAllocationType = MEM_RESERVE | MEM_COMMIT;
    LPVOID arg = (LPVOID)VirtualAllocEx(process, NULL, dwSize, flAllocationType, PAGE_READWRITE);
    printf("inside target process the allocated memory address : %#p\n", arg);

    //  Write the argument to LoadLibraryA to the process's newly allocated memory region.
    WriteProcessMemory(process, arg, dllFileName, dwSize, NULL);

    // Inject our DLL into the process's address space.
    HANDLE threadID = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)addr, arg, 0, NULL);
    printf("Created Remote thread TID : %d\n", threadID);

    //   Close the handle to the process, becuase we've already injected the DLL.
    CloseHandle(process);
}

int injectFun(HANDLE process, LPVOID arg)
{
    //   Get address of the LoadLibrary function.
    LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
    printf("inside kernel32.dll library the LoadLibraryA function address : %p.\n", addr);
    printf("---------------------------");
    return 0;
}

int hookfun()
{
    // Get process handle passing in the process ID.
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    printf("Target process handle : %#p\n", process);

    // The function that lpStartAddress pointed to must exist in the remote process.
    HANDLE threadID = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)injectFun, NULL, 0, NULL);
    printf("Created Remote thread TID : %#p\n", threadID);

    //   Close the handle to the process, becuase we've already injected the DLL.
    CloseHandle(process);
}

void menu_disp()
{
    printf("\n -------- 菜单选项(MENU) --------");
    printf("\n -------- (hooking target process PID: %d) -------- ", processId);
    printf("\n 1 - new target process and set PID");
    printf("\n p - (pxxxx) manually input the PID ");
    printf("\n 2 - hook dll");
    printf("\n 3 - hook function");
    printf("\n 4 - load to myself for debug");
    printf("\n 5 - null slot");
    printf("\n 6 - Display information");
    printf("\n 9 - Exit");
    printf("\n请输入操作码(opcode):  ");
    printf("\n");
}


int main()
{
    // setlocale(LC_ALL, "en_US.UTF-8");
    // SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    // SetConsoleOutputCP(936);

    // initialization and feed back state:
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    // wprintf(L"你好，世界！\n");
    printf("你好，世界！\n");

    char buffer[128];
    char input[128]; // Array to store the input string

    // Main loop:
    while (TRUE)
    {
        menu_disp();
        printf("enter command >>> ");

        // scanf("%s", input);
        // fgets reads a string from stdin until it encounters a newline character
        fgets(input, sizeof(input), stdin);
        unsigned len = strlen(input);
        *(input + len - 1) = '\0';
        char com;
        char param[64];
        switch (len)
        {
        case 0:
            com = ' ';
            break;
        case 1:
            com = ' ';
            break;
        default:
            // char *newline = strchr(input, '\n');
            com = input[0];
            strcpy(param, input + 1);
            break;
        }

        printf("You entered: _%s_(%d)\n", input, len - 1);

        switch ((int)com)
        {
        case '1':
            newNotepadProcess();
            break;
        case 'p':
            processId = (DWORD)atoi(param);
            break;
        case '2':
            hookdll();
            break;
        case '3':
            hookfun();
            break;
        case '4':
            LoadLibraryA("injectdll.dll");
            break;
        case '9':
            return 0;
            break;
        default:
            sprintf(buffer, "unknow command : ??? %c ???", com);
            error_message(buffer);
            break;
        }
    }

    getchar();

    return 0;
}


