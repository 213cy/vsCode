#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>


void pointerswap()
{
    printf(" [console] fd_stdin=(%d), fd_stdout=(%d), fd_stderr=(%d)\n",
           _fileno(stdin), _fileno(stdout), _fileno(stderr));
    printf(" [console] fd_stdout=(%d), FILE* stdout=(0x%x),  FILE_stdout=(0x%x)\n", _fileno(stdout), stdout, *stdout);

    FILE swap = *stdout;
    // FILE *stream = fopen("outfile.txt", "w");
    // *stdout = *stream;
    *stdout = *fopen("outfile.txt", "w");
    FILE *stream = stdout;

    fprintf(stream, " [fprintf] This will go to the file 'outfile.txt'\n");
    fflush(stream);
    printf(" [printf] fd_stdout=(%d), FILE* stdout=(0x%x),  FILE_stdout=(0x%x)\n", _fileno(stdout), stdout, *stdout);
    // _flushall();
    printf(" [printf] fd_stream=(%d), FILE* stream=(0x%x),  FILE_stream=(0x%x)\n", _fileno(stream), stream, *stream);
    // _flushall();
    fprintf(stdout, " [fprintf] successfully reassigned\n");
    _flushall();

    fclose(stream);

    *stdout = swap;

    printf(" [console] ###############################\n");
    system("type outfile.txt");
    printf(" [console] ###############################\n");

    printf(" [console] fd_stdout=(%d), FILE* stdout=(0x%x)  FILE_stdout=(0x%x)\n", _fileno(stdout), stdout, *stdout);
}

int main()
{
    char codestr[80] = {"0"};

    SYSTEMTIME s;
    GetLocalTime(&s);
    printf("It's %d/%d/%d %02d:%02d:%02d now.\n", s.wYear, s.wMonth, s.wDay, s.wHour, s.wMinute, s.wMilliseconds);

    printf(" \n\n[comment] ======================================== \n\n");
    printf(" \n\n[comment] ======================================== \n\n");

    printf(" [console] fd_stdin=(%d), fd_stdout=(%d), fd_stderr=(%d)\n",
           _fileno(stdin), _fileno(stdout), _fileno(stderr));
    printf(" [console] fd_stdout=(%d), FILE* stdout=(0x%x),  FILE_stdout=(0x%x)\n", _fileno(stdout), stdout, *stdout);

    FILE *stream = freopen("outfile.txt", "w", stdout); // C4996

    fprintf(stream, " [fprintf] This will go to the file 'outfile.txt'\n");
    fflush(stream);
    printf(" [printf] fd_stdout=(%d), FILE* stdout=(0x%x),  FILE_stdout=(0x%x)\n", _fileno(stdout), stdout, *stdout);
    // _flushall();
    printf(" [printf] fd_stream=(%d), FILE* stream=(0x%x),  FILE_stream=(0x%x)\n", _fileno(stream), stream, *stream);
    // _flushall();
    fprintf(stdout, " [fprintf] successfully reassigned %d%d\n",rand(), s.wMilliseconds);
    _flushall();

    fclose(stream);

    stream = freopen("conout$", "w", stdout);
    // *stdout = *fopen( "conout$","w");

    printf(" [console] ###############################\n");
    // system("type outfile.txt");
    printf(" [console] ###############################\n");

    printf(" [console] fd_stdout=(%d), FILE* stdout=(0x%x),  FILE_stdout=(0x%x)\n", _fileno(stdout), stdout, *stdout);
    printf(" [console] fd_stream=(%d), FILE* stream=(0x%x),  FILE_stream=(0x%x)\n", _fileno(stream), stream, *stream);

    getchar();
    printf("Enter interactive model!\n");
    // fflush(stdout);

    while (strcmp(codestr, "exit") != 0)
    {
        scanf("%s", codestr);
        printf(" command received (%s).\n", codestr);
        // fflush(stdout);
        // Sleep(2000);
    }

    return 0;

    printf(" [file pointer] stdout=(0x%x) ,  fd for stdout=(%d)\n", stdout, _fileno(stdout));
    // int old = _dup(_fileno(stdout));
    FILE *stream2 = fopen("outfile.txt", "w");
    // _dup2(_fileno(stream), 1);

    printf("111111 stdout=(0x%x) stream=(0x%x) \n", stdout, stream);
    fprintf(stdout, "222222 successfully reassigned\n");
    fflush(stdout);
    fprintf(stream, "333333 This will go to the file 'outfile.txt'\n");

    // Flush stdout stream buffer so it goes to correct file
    fflush(stdout);
    fflush(stream);
    _flushall();
    fclose(stream);

    // _dup2(old, 1);
    // _close(old);

    // AttachConsole(ATTACH_PARENT_PROCESS);

    system("type outfile.txt");
    // fflush(stdout);
    // _flushall();

    return 0;
}