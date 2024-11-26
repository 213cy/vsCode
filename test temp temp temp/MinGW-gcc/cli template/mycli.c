/*
 * usage: mycmd.exe --help
 *        mycmd.exe --block [filter]
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE console;
const int MAX_CMD_ITEM = 9;
// ========================

typedef enum
{
    CMD_A = 0,
    CMD_B,
    CMD_C,
    CMD_HASH,
    CMD_HELP,
    CMD_ABOUT,
    CMD_EXIT,
    CMD_INVALID
} cmd_t;

struct cmd_entry
{
    const char *name;
    const char *alias;
    cmd_t id;
    int hash;
};

static struct cmd_entry const cmd_list[] = {
    {"hash", "0", CMD_HASH, '0'},
    {"aaa", "1", CMD_A, '1'},
    {"bbb", "2", CMD_B, '2'},
    {"ccc", "3", CMD_C, '3'},
    {"exit", "9", CMD_EXIT, '9'},
    {"exit", "e", CMD_EXIT, 'e'},
    {"help", "h", CMD_HELP, 'h'},
    {"version", "v", CMD_ABOUT, 'v'}};

// Get command list size for scalability
int CMD_LIST_SIZE = sizeof(cmd_list) / sizeof(cmd_list[0]);
// =====================================

union CMDParse
{
    int cmdInt;
    char cmdStr[80]; // = {"0"}
};

void help()
{
    printf("--------- rrrrrrrrrrrrr --------------\n");
    printf("Usage:  [OPTIONS] SOURCE PORT\n");
    printf("\nOptions:\n");
    printf("   -c, --config=FILENAME   Read config from the config file.\n");
    printf("   -v, --verbose           Explain what is being done.\n");
    printf("       --color             Haracteristic of visual perception described.\n");
    printf("       --output=TYPE       Output data type, text, json, table.\n");
    printf("   -h, --help              Display this help and exit.\n");
    printf("       --version           Output version information and exit.\n");
    printf("See the manpage for full options, descriptions and usage examples.");
    printf("\n");
}

void version(void)
{
    printf("\n");
    printf("%s\n", "Copyright (C) 2019 Free Software Foundation, Inc.");
    printf("%s", "License GPLv3+: GNU GPL version 3 or later");
    printf("%s\n", " <https://gnu.org/licenses/gpl.html>.");
    printf("%s\n", "This is free software: you are free to change and redistribute it.");
    printf("%s\n", "There is NO WARRANTY, to the extent permitted by law.");
    printf("\n");
}

// ========================

void menu_disp()
{
    printf("\n -------- 菜单选项(MENU) --------");
    printf("\n -------- (hooking PID: %d) -------- ", 333);
    printf("\n 1 - Rehook");
    printf("\n 2 - Unhook");
    printf("\n 3 - hash sdfsdfsdfsdf");
    printf("\n 4 - Display infomation");
    printf("\n 9 - Exit");
    printf("\n请输入操作码(opcode)：  ");
    printf("\n");
}

void error_message(const char *format, void *value)
{
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
    fprintf(stderr, "ERROR : ");
    fprintf(stderr, format, value);
    fprintf(stderr, "     (last error code = %d)\n", GetLastError());
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN |
                                         FOREGROUND_BLUE);
}
int __cdecl main(int argc, char **argv)
{

    union CMDParse cmdInput;

    const char *filter = "true", *err_str;
    int temp_int;
    char temp_str[MAX_PATH + 1];
    BOOL block = FALSE;

    // get and check parameters
    switch (argc)
    {
    case 1:
        break;
    case 2:
        if (strcmp(argv[1], "--block") == 0)
        {
            block = TRUE;
        }
        else
        {
            filter = argv[1];
        }
        break;
    case 3:
        if (strcmp(argv[1], "--block") == 0)
        {
            block = TRUE;
            filter = argv[2];
            break;
        }
        // Fallthrough:
    default:
        fprintf(stderr, "usage: %s [filter]\n", argv[0]);
        fprintf(stderr, "       %s --block [filter]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // initialization and feed back state:
    console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(936);
    // SetConsoleOutputCP(437);
    if (console == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "last error code: (%d)\n",
                GetLastError());
        return EXIT_FAILURE;
    }

    // Main loop:
    while (TRUE)
    {

        menu_disp();

        int a;

        char sep;
        int i = 0;
        char *items[MAX_CMD_ITEM];
        //     for (int j = 0; j < MAX_CMD_ITEM; j++) {
        //         free(item[j]);
        //     }
        printf("enter command >>> ");
        while (i < MAX_CMD_ITEM && (temp_int = scanf("%22s%c", items[i] = malloc(23), &sep)) == 2)
        {
            printf("%d_%d", i, temp_int);
            printf("[%s]%d(0x%X)\n", items[i], strlen(items[i]), sep);
            if (sep == '\n')
            {
                break;
            }
            i++;
        }

        int currentCmdLen = strlen(items[0]);
        cmd_t currentCmdId = CMD_INVALID;
        if (!currentCmdLen)
        {
            fprintf(stderr, "\nno command(%d)\n", GetLastError());
            continue;
        }
        else
        {
            // Remove case sensitivity
            for (size_t i = 0; i < currentCmdLen; i++)
            {
                // Buffer for the character
                char c = items[0][i];
                // Convert to lower and save
                items[0][i] = tolower(c);
            }

            if (currentCmdLen == 1)
            {
                // Search for command
                for (int i = 0; i < CMD_LIST_SIZE; i++)
                {
                    if (!strncmp(items[0], cmd_list[i].alias, 63))
                    {
                        currentCmdId = cmd_list[i].id;
                    }
                }
            }
            else
            {
                // Search for command
                for (int i = 0; i < CMD_LIST_SIZE; i++)
                {
                    if (!strncmp(items[0], cmd_list[i].name, 63))
                    {
                        currentCmdId = cmd_list[i].id;
                    }
                }
            }
        }

        switch (currentCmdId)
        {
        case CMD_HASH:
            printf("  ### %s ###\n", items[1]);
            strcpy(cmdInput.cmdStr, items[1]);
            memset(cmdInput.cmdStr + strlen(items[1]), '\0', 3);

            i = 0;
            unsigned int tempInt;
            memcpy(&tempInt, cmdInput.cmdStr, 4);
            while (tempInt)
            {
                printf("Index %3u: 0x%08X\n", i, tempInt);
                i += 4;
                memcpy(&tempInt, cmdInput.cmdStr + i, 4);
            }

            int cmdDigest = 0;
            int i=0;
            int a;
            while ((a = *(int *)(cmdInput.cmdStr + i)) != 0)
            {
                printf("Index %3u: [%c%c] 0x%08X\n", i, (char *)&a, (char *)&a+1, a);
                i += 4;
                cmdDigest = cmdDigest ^ a;
                printf("digest : 0x%X\n", cmdDigest);
            }
            break;
        case CMD_HELP:
            help();
            break;
        case CMD_ABOUT:
            version();
            break;
        case CMD_EXIT:
            return EXIT_FAILURE;
            break;
        default:
            error_message("unknow command : ??? %s ???", items[0]);
            break;
        }

        putchar('\n');
    }

    return 0;
}
