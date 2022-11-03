#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <conio.h>

CHAR buf[64];
CHAR codestr[64] = {"0"};
INT code;

int main()
{
	SetConsoleOutputCP(936);

	printf("    ====== 第 一 种 ====== \n");

	while (code != 'exit')
	{
		printf("\n ------ ------  MENU  菜单选项 ------ ------");
		printf("\n    命令列表: 3, entry, exit");
		printf("\n    命令列表: %d, %d, %d", '3', 'entry', 'exit');
		printf("\n please enter an command:  ");

		// int __toascii(   int c );
		code = 0;
		memset(buf, 0, 9);
		scanf("%s", codestr);
		int n = strlen(codestr) - 1;
		for (int k = 0; k <= n; k++)
		{
			*(buf + k) = *(codestr + n - k);
			// printf("k 的值： %d\n", k);
		}
		sscanf(buf, "%s", &code);

		switch (code)
		{
		case '3':
			printf("[111111] 33333333333333333\n");
			break;
		case 'entry':
			printf("[111111] entryentryentryentry\n");
			break;
		case 'exit':
			printf("[111111] exitexitexitexit\n");
			break;
		default:
			printf("无效的选项(%d)或无效的命令(%s):  \n", code, codestr);
			printf("请重新选择或输入!\n\n");
		}
	}

	printf("\n\n    ====== 第 二 种 ====== \n");
	*codestr = 0;

	while (strcmp(codestr, "exit") != 0)
	{
		printf("\n ------ ------  MENU  菜单选项 ------ ------");
		printf("\n    命令列表: 3, entry, exit");
		printf("\n please enter an command:  ");
		scanf("%s", codestr);
		if (strcmp(codestr, "3") == 0)
			printf("[222222] 33333333333333333\n");
		else if (strcmp(codestr, "entry") == 0)
			printf("[222222] entryentryentryentry\n");
		else
			printf(" command received (%s).\n", codestr);
	}
	printf("[222222] exitexitexitexit\n");

	printf("\n\n    ====== 第 三 种 ====== \n");

	while (code != 'q')
	{
		printf("\n ------ ------  MENU  菜单选项 ------ ------");
		printf("\n    命令列表: 3, p, TAB, q");
		printf("\n please enter an command:  ");

		code = _getche();
		// switch ( getchar() )
		switch (code)
		{
		case '3':
		{
			printf("\n[333333] 33333333333333333\n");
		}
		break;
		case 'p':
		{
			printf("\n[333333] pppppppppppppp\n");
		}
		break;
		case 9: // TAB
		{
			printf("\n[333333] TABTABTABTABTAB\n");
		}
		break;
		case 'q':
			printf("\n[333333] qqqqqqqqqqqqqq\n");
			break;
		default:
			printf("\n无效的选项(%d) >> 请重新选择或输入命令!\n\n", code);
		}
	}

	printf(" # # # done\n 任意键退出 .\n");
	getchar();
	getchar();
	Sleep(1000);

	return 1;
}