#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <stdio.h>
#include <io.h>
//#include <string.h>
// #include <stdlib.h>
// #include <tchar.h>
// #include <process.h>
const WCHAR *music[3] = {L"C:\\Users\\Administrator\\Videos\\MagicSword.mp3",
                         L"C:\\Users\\Administrator\\Videos\\NineSwords.mp3",
                         L"C:\\Users\\Administrator\\Videos\\RedFlower.mp3"};

VOID playmusic(UINT message)
{
    WCHAR buf[128];
    DWORD mciError = mciSendString(L"status currentMusic mode", buf, _countof(buf), NULL);
    printf("\n[  log  ] currentMusic status: %s%s%s%s%s%s%s",
           buf, buf + 1, buf + 2, buf + 3, buf + 4, buf + 5, buf + 6);
    printf("\n[  log  ] currentMusic status: %ls", buf);
    if (mciError)
    {
        mciGetErrorString(mciError, buf, _countof(buf));
        // MessageBox(NULL, buf, L"error", MB_OK);
        printf("\n[Media Control Interface] {%ls}(micError=%d)", buf, mciError);
    }
    if (!wcscmp(buf, L"playing")) // The sound is now playing.| wcscmp(buf, L"")
    {
        printf("\n[Media Control Interface] music is playing, return");
        return;
    }

    //mciSendString(L"play C:\\Users\\Administrator\\Videos\\MagicSword.mp3", NULL, 0, 0);
    //mciSendString(music[rand() % 3], NULL, 0, 0);
    mciSendString(L"close currentMusic", NULL, 0, NULL);
    wsprintf(buf, L"open \"%s\" alias currentMusic", music[rand() % 3]);
    mciSendString(buf, NULL, 0, NULL);
    mciSendString(L"play currentMusic", NULL, 0, NULL);
    printf("\n[Media Control Interface] play currentMusic");
}
void menu_disp()
{
    wprintf(L"\n\n\n -------- -------- MENU  菜单选项 -------- --------");
    printf("\n 命令列表：music sound exit");
    printf("\n please enter an command:  ");
}

int main()
{
    char codestr[80] = {"0"};
    printf("Enter interactive model!\n");
    system("dir");
    while (strcmp(codestr, "exit") != 0)
    {
        menu_disp();
        scanf("%s", codestr);
        if (strcmp(codestr, "music") == 0)
        {
            playmusic(0);
        }
        else if (strcmp(codestr, "sound") == 0)
        {
            //PlaySound(L"C:\\Users\\Administrator\\Videos\\MagicSword.mp3", NULL, SND_FILENAME);
            PlaySound(L"JainaPissed4.wav", NULL, SND_FILENAME | SND_ASYNC);
        }
        else
        {
            printf(" command received (%s).\n", codestr);
        }
    }
    return 0;
}