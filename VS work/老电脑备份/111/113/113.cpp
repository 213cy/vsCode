// 113.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#include "stdlib.h"

int windowcount, printcount;

POINT ChooseWindow()
{
	POINT Point;

	HANDLE hStdin; 
	DWORD fdwMode, fdwSaveOldMode;
	DWORD cNumRead, i; 
	INPUT_RECORD irInBuf[128]; 

	// Get the standard input handle. 
	hStdin = GetStdHandle(STD_INPUT_HANDLE); 
	// Save the current input mode, to be restored on exit. 
	GetConsoleMode(hStdin, &fdwSaveOldMode);

	// Enable the mouse input events. 
	fdwMode = ENABLE_MOUSE_INPUT; 
	SetConsoleMode(hStdin, fdwMode);

	printf("点击,拖动,并释放鼠标!\n");
	// Loop to read and handle the next 100 input events. 
	while (TRUE) 
	{ 
		// Wait for the events. 
		ReadConsoleInput( 
			hStdin,      // input buffer handle 
			irInBuf,     // buffer to read into 
			128,         // size of read buffer 
			&cNumRead); // number of records read 


		// Dispatch the events to the appropriate handler. 
		for (i = 0; i < cNumRead; i++) 
		{
			if (MOUSE_EVENT==irInBuf[i].EventType)
			{ 
				if ( 0==irInBuf[i].Event.MouseEvent.dwEventFlags)
				{
					printf("Mouse event: ");
					if(irInBuf[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
						printf("left button press, 进入选点模式! \n");
					else if(irInBuf[i].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
					{
						printf("right button press, 退出选点!\n\n");
						// Restore input mode on exit.
						//FlushConsoleInputBuffer( hStdin);
						SetConsoleMode(hStdin, fdwSaveOldMode);
						return Point;
					}
					else
					{
						printf("button released\n");
						GetCursorPos( &Point);
						printf("鼠标屏幕坐标: x:%4d,y:%4d\n", Point.x, Point.y);
					}
				}
			} 
		}
	} 

}
void BlinkWindow(HWND hWndhex)
{
	ShowWindow(  hWndhex,	SW_HIDE);
	printf(	"%d--", IsWindowVisible( hWndhex));
	printf("0x%p\n",GetWindowLongPtr(hWndhex, GWL_STYLE));
	Sleep(5000);
	Beep( 750, 300 );
	ShowWindow(  hWndhex,	SW_SHOW);
	printf(	"%d--", IsWindowVisible( hWndhex));
	printf("0x%p\n",GetWindowLongPtr(hWndhex, GWL_STYLE));

}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	++windowcount;
	LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);//GWL_HWNDPARENT
	if (lStyle & lParam)
	{
		wchar_t szBuffer[5*100];
		wchar_t szTemp[101];
		int index=0;

		printf("      - - - - 第 %d 个窗体 - - - -\n", ++printcount);
		GetWindowText(hwnd, szTemp, 80);
		index += swprintf( szBuffer + index, 150, L"window:0x%p name:%s\n", hwnd , szTemp ); 

		RECT rect;
		GetWindowRect(hwnd, &rect);
		index += swprintf( szBuffer + index, 80, L"rectangle:(%4d,%4d) - (%4d,%4d) ",
			rect.left, rect.top, rect.right, rect.bottom);

		GetClassName(hwnd, szTemp, 80);
		index += swprintf( szBuffer + index, 80, L"class:\"%s\"\n", szTemp );


		index += swprintf( szBuffer + index, 80, L"lStyle: %#010lx\n", lStyle);

		HMENU hMenu = GetMenu( hwnd);
		int num = GetMenuItemCount( hMenu);
		int num2 = SendMessage( hwnd, WM_USER + 24, 0, 0); // #define TB_BUTTONCOUNT  (WM_USER + 24) in commctrl.h
		index += swprintf( szBuffer + index, 80, L"Menu:%#010lx(%d) TB_BUTTONCOUNT:%d\n", hMenu, num, num2);


		HWND hwndParent = GetParent(hwnd);
		GetWindowText(hwndParent, szTemp, 80);
		index += swprintf( szBuffer + index, 80, L"Parent:0x%p name:%s\n", hwndParent, szTemp);
		//
		wprintf( szBuffer);
	}
	return ( 1 >= 0 ) ? TRUE : FALSE;
}
BOOL CALLBACK EnumChildProc(  HWND hwnd, LPARAM lParam)
{
	++windowcount;
	char szTemp[101];
	GetClassNameA(hwnd, szTemp, 80);
	if ( 0==_stricmp( szTemp,	"MsoCommandBar") || TRUE )
	{
		wchar_t szBuffer[5*100];
		wchar_t szTemp[101];
		int index=0;

		printf("      - - - - 第 %d 个窗体 - - - -\n", ++printcount);
		GetWindowText(hwnd, szTemp, 80);
		index += swprintf( szBuffer + index, 150, L"window:0x%p name:%s\n", hwnd , szTemp ); 

		RECT rect;
		GetWindowRect(hwnd, &rect);
		index += swprintf( szBuffer + index, 80, L"rectangle:(%4d,%4d) - (%4d,%4d) ",
			rect.left, rect.top, rect.right, rect.bottom);

		GetClassName(hwnd, szTemp, 80);
		index += swprintf( szBuffer + index, 80, L"class:\"%s\"\n", szTemp );

		HMENU hMenu = GetMenu( hwnd);
		int num = GetMenuItemCount( hMenu);
		int num2 = SendMessage( hwnd, WM_USER + 24, 0, 0); // #define TB_BUTTONCOUNT  (WM_USER + 24) in commctrl.h
		index += swprintf( szBuffer + index, 80, L"Menu:%#010lx(%d) TB_BUTTONCOUNT:%d\n", hMenu, num, num2);
		//
		wprintf( szBuffer);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////
void CurrentWindow()
{
	POINT Point=ChooseWindow();

	HWND hWnd = WindowFromPoint( Point);
	EnumWindowsProc(hWnd, WS_VISIBLE);

	//SetCursorPos( Point.x, Point.y);

}

void SysfaderWindow()
{
	char szBuffer[120]; 

	//HWND hwnd = FindWindowA( "SysFader", "SysFader");
	HWND hwnd = (HWND)0x000504c0;
	printf("SysFader 窗口句柄: 0x%p\n",hwnd);

	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

	//HANDLE handle1 = (HANDLE)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
	printf("SysFader 实例句柄: 0x%p\n",hInstance);


	if (GetModuleFileNameA(hInstance, szBuffer, 120))
		printf("SysFader 模块文件: %s\n\n", szBuffer);
	else printf("SysFader 模块文件: 未找到\n"); 

	printf("\n"); 
}
void OneWindow()
{
	HWND hWndhex=(HWND)0x000504c0;
	hWndhex = FindWindowA( "Notepad", NULL);
	//////hWndhex = FindWindowExA( hWndhex, NULL, "Edit", NULL);

	hWndhex = FindWindowA( "wndclass_desked_gsk", NULL);

	hWndhex=(HWND)0x00030500;
	windowcount=0;
	printcount=0;
	printf("筛选条件: 0x%p\n",WS_VISIBLE);
	EnumChildWindows(  hWndhex, EnumChildProc, WS_VISIBLE);
	printf("一共枚举了 %d 个窗体,显示了个 %d 窗体信息,枚举完毕!\n\n", windowcount, printcount);

	////if (IsWindow( hWndhex))
	////{
	////	SetForegroundWindow( hWndhex);
	////	BlinkWindow( hWndhex);
	////}
	////else printf("窗体不存在\n\n");
}
void TopLevelWindows()
{
	windowcount=0;
	printcount=0;
	printf("筛选条件: 0x%p\n",WS_VISIBLE);
	EnumWindows(EnumWindowsProc, WS_VISIBLE);//(LPARAM)
	printf("一共枚举了 %d 个窗体,显示了个 %d 窗体信息,枚举完毕!\n\n", windowcount, printcount);
}
void test()
{

	printf( " ========= test mode =============\n");
	char stocknum[10];
	UINT err=GetDlgItemTextA( (HWND)0x1302c0, 12005, stocknum, 9); 
	DWORD eNum = GetLastError( );
	TCHAR sysMsg[256];

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default lang.
		sysMsg, 256, NULL );

	wprintf( L" WARNING:  failed with error %d : %s", eNum, sysMsg );
	printf( "test ruselt:\n");
	//
	//TCHAR szBuffer[8];
	wchar_t szBuffer[80];
	//memset(szBuffer, 0, 80);
	int index;
	index  = swprintf( szBuffer,  9, L"%s", L"123456789"  ); 
	printf( "%d\n",index);
	wprintf( L"%s\n",&szBuffer[6]);
	//
	printf( " ========= test  end =============\n\n");

}

///////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "chs");

	DWORD nMode;
	HANDLE hConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hConsoleHandle, &nMode);
	SetConsoleMode(hConsoleHandle, nMode & ~ENABLE_MOUSE_INPUT );//| ENABLE_PROCESSED_INPUT

	char input_chars[21]; // room for 20 chars + '\0'
	int input_opt = -1;

	enum { inst1=101,inst2,inst3,inst4='4tni' };

	while( true )
	{
		printf( "Choose an option:\n" );
		printf( "1 - current process info (default)\n" );
		printf( "2 - sysfader window info\n" );
		printf( "3 - specified window info\n" );
		printf( "4 - enumerates all top-level windows\n" );
		printf( "9 - exit\n" );
		printf( "or type a command : sleep , quit , test\n" );
		printf( "others : 101 , int4\n" );
		printf( "\nenter command:" );


		//char *aaa=gets( input_chars );
		//if( gets_s( input_chars,20 ) == NULL )
		//	printf( "输入为NULL\n" );
		gets_s( input_chars,20 );
		input_opt = atoi( input_chars );	

		if( input_opt == 0 )//如果input_chars 包含字母
		{
			//printf( "%d\n" ,*((int *)input_chars));
			//printf( "%d\n" ,*(unsigned int *)("slee"));
			//const char aaa[13]="slee";
			//printf( "%d\n" ,*(int *)aaa );
			input_opt = *((int *)input_chars);
		}


		switch( input_opt )
		{
		case 1:
			{
				CurrentWindow();
			}
			break;
		case 2:
			{
				SysfaderWindow();
			}
			break;
		case 3:
			{
				OneWindow();
			}
			break;
		case 4:
			{
				TopLevelWindows();
			}
			break;
		case 9:
			{
				return 0;
			}
			break;
		case 'eels'://inverse the first 4 letters of 'sleep'
			{
				int MS;
				MS=atoi(input_chars+6);
				printf( "sleeping (%dms) ...\n" ,MS);
				Sleep(MS);
				printf( "wake up!\n\n");
			}
			break;
		case 'tset': //inverse the first 4 letters of 'test'
			{
				test();
			}
			break;
		case 'tiuq': //inverse the first 4 letters of 'quit'
			{
				return 0;
			}
			break;
		case inst1: //inverse the first 4 letters of 'quit'
			{
				printf( " ========= test  end =============\n\n");
				HWND hwnd = (HWND)0xb05ba;
				printf( "HWND: 0x%p === ID: %d \n" , hwnd , GetDlgCtrlID( hwnd ) );

				printf( " ========= test  end =============\n\n");
			}
			break;
		case inst4: //inverse the first 4 letters of 'quit'
			{
				printf( " ========= test  end =============\n\n");
				HWND hWndTmp = FindWindowA( "#32770", "买入交易确认");
				printf( "HWND: 0x%p \n" , hWndTmp );
				printf( " ========= test  end =============\n\n");
			}
			break;
		default:
			printf( "无效的选项: %d 或无效的命令: %s\n" ,atoi( input_chars ), input_chars);
			printf( "请重新选择或输入命令!\n\n" );


			/*
			_locale_t locale,locale2;

			locale = _create_locale( lc_numeric ,"chs");//lc_all
			locale2 = _get_current_locale();

			_wprintf_l( l"请重新选择dfasdf入命令\n\n",locale);
			_wprintf_l( l"请重新选择dfasdf入命令\n\n",locale2);
			*/



		} // switch
	}
}

