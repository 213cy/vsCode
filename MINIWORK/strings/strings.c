#include <windows.h>
#include <stdio.h>
#include <ctype.h>

CHAR buf[64];
WCHAR buffer[64];

int main()
{
    system("chcp 65001"); // C文件直接编译,可以再 CP=65001 时显示中文
    printf(" # # #  C文件直接编译,可以在 CP=65001 时显示中文  # # # \n");

    printf(" \n\n==================== 输出宽字符 ==================== \n\n");

    // WCHAR *szBuf = L"找不到记事本\n";
    const WCHAR *szBuf = L"找不到记事本\n";
    printf(" # # # #### printf 不行 ######################\n");
    printf("[%s]    ###    [%c]    ###    [%ls] \n", szBuf, szBuf, szBuf);
    printf("\n # # # #### wprintf 不行 #####################\n");
    wprintf(szBuf);
    printf(" # # # #### 只能正确输出非汉字 ################\n");
    wprintf(L"abcdefg\n");
    printf("%ls", L"abcdefg\n");

    printf(" \n\n==================== 初始化 ==================== \n\n");

    char aaa[7] = "abcdef";
    // char ccc[] = {*"a", *"b", *"c", *"d", *"e", *"f", 0};
    char ccc[] = {'a', 'b', 'c', 'd', 'e', 'f', 0};
    char bbb[32] = {0};
    memset(bbb, 97, 32);
    bbb[31] = 0;

    printf("%d ( 0 : the contents of both strings are equal )\n", strcmp(aaa, ccc));
    sprintf(bbb, "%s", aaa);
    printf("%p  %s\n", bbb, bbb);
    printf("%p  %s\n", &bbb, &bbb);
    printf("%p  %c\n", &bbb[2], bbb[2]);
    printf("%p  %c\n", bbb + 4, *(bbb + 4));

    // printf("%d\n", strlen(ccc) );
    // strcpy(buf, bbb + strlen("abcdef"));
    strcpy(buf, bbb + strlen(ccc) + 1);
    // strcpy(buf, bbb + 7);
    printf("%s\n", buf);

    printf(" \n\n==================== 字符窜数值转换 ==================== \n\n");
    int temp;
    printf("sizeof(int)=%d , sizeof(\"a\")=%d\n", sizeof(int), sizeof("a"));

    for (int n = 6; n > 0; n--)
    {
        buf[n] = 0;
        sscanf(buf, "%s", &temp);
        printf("%d %10s %10d %10d %d\n", n, buf, temp, *(int *)buf, (int *)buf);
    }
    printf("[[]] %d %d %d \n", *(int *)"aaaa", *(int *)"aaaaa", *(int *)"aaaaaa");
    printf("[[]] %d %d %d  <--这些都越界了,所以结果不确定!\n", *(int *)"a", *(int *)"aa", *(int *)"aaa");
    printf("[[]] %d %d %d \n", 'a', 'aa', 'aaa');
    printf("[[]] %d %d %d \n", *"a", *"aa", *"aaa");
    printf("[[]] %d %d %d \n", *"aaaa", *"aaaaa", *"aaaaaa");

    printf(" \n\n==================== 字母数值转换 ==================== \n\n");

    // int __toascii(   int c );
    // 'a' == __toascii('a' + 128)
    int index = printf("char 2 num: %d  %d\nnum 2 char:  %c   %c\n",
                       *"a", __toascii(buf[0]), (char)97, 97);
    if (index)
    {
        printf("the total number of characters written is %d\n", index);
    }
    else
    {
        printf("a writing error occurs");
    }

    printf(" \n\n==================== 输入 ==================== \n\n");

    char codestr[32] = {"0"};
    int code1, code2, code3;

    while (strcmp(codestr, "exit") != 0)
    {
        printf("Enter strings to scanf.\n");
        scanf("%s", codestr);
        code1 = getchar();
        printf("Enter a character to getchar.\n");
        code2 = getchar();
        code3 = getchar();
        printf("Characters been entered: %s  %d  %c  %d  ----- \n\n\n", codestr, code1, code2, code3);

        Sleep(3000);
    }
}