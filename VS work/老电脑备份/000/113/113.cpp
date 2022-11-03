// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include <Windows.h>
#include <locale.h>
//#include <locale>


// 113.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#include "stdlib.h"

int windowcount;
void BlinkWindow(HWND hWndhex)
{
	ShowWindow(  hWndhex,	SW_HIDE);
	printf(	"%d", IsWindowVisible( hWndhex));
	printf("0x%p",GetWindowLongPtr(hWndhex, GWL_STYLE));
	Sleep(500);
	
	ShowWindow(  hWndhex,	SW_SHOW);
	printf(	"%d", IsWindowVisible( hWndhex));
	printf("0x%p",GetWindowLongPtr(hWndhex, GWL_STYLE));
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	//printf("hwnd:%d,lParam:%d\n",sizeof(hwnd),sizeof(lParam));
	//printf("hwnd:%p,lParam:%p\n",hwnd,lParam);
	//printf("hwnd:%p,lParam:%p\n",&hwnd,&lParam);

	LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);//GWL_HWNDPARENT
	if (lStyle & lParam)
	{
		wchar_t szBuffer[5*100];
		wchar_t szTemp[101];
		int index=0;


		printf("0x%p\n",lParam);
		printf("第 %d 个窗体:\n", ++windowcount);

		GetWindowText(hwnd, szTemp, 80);
		index += swprintf( szBuffer + index, 100, L"0x%p: %s\n", hwnd , szTemp ); 

		RECT rect;
		GetWindowRect(hwnd, &rect);
		index += swprintf( szBuffer + index, 80, L"(%4d,%4d) - (%4d,%4d): ",
			rect.left, rect.top, rect.right, rect.bottom);

		GetClassName(hwnd, szTemp, 80);
		index += swprintf( szBuffer + index, 80, L"\"%s\"\n", szTemp );


		index += swprintf( szBuffer + index, 80, L"%#010lx\n", lStyle);

		HWND hwndParent = GetParent(hwnd);
		GetWindowText(hwndParent, szTemp, 80);
		index += swprintf( szBuffer + index, 80, L"0x%p: %s\n", hwndParent, szTemp);

		wprintf( szBuffer);
	}

	/*

	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
	GetModuleFileName(hInstance, szBuffer, sizeof(szBuffer));



	*/

	return ( 1 >= 0 ) ? TRUE : FALSE;
}

void CurrentWindow()
{
	char szBuffer[120]; 
	HWND hWndhex = GetForegroundWindow();
	EnumWindowsProc(hWndhex, 0xffffffff);
	HINSTANCE hIns=(HINSTANCE)GetModuleHandle(NULL);
	GetModuleFileNameA(NULL, szBuffer, 120);
	printf("%s\n\n", szBuffer);
}

void SysfaderWindow()
{
	char szBuffer[120]; 

	//HWND hwnd = FindWindowA( "SysFader", "SysFader");
	HWND hwnd = (HWND)0x000c01fe;//FindWindowA( "SysFader", "SysFader");
	printf("SysFader 窗口句柄: 0x%p\n",hwnd);

	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
	printf("SysFader 实例句柄: 0x%p\n",hInstance);


	if (GetModuleFileNameA(hInstance, szBuffer, 120))
		printf("SysFader 模块文件: %s\n\n", szBuffer);
	else printf("SysFader 模块文件: 未找到\n"); 

	printf("\n"); 
}
void OneWindow(HWND hWndhex)
{
	hWndhex = FindWindowA( "Notepad", NULL);
	hWndhex = FindWindowExA( hWndhex, NULL, "Edit", NULL);

	if (IsWindow( hWndhex))
	{

		//EnumWindowsProc(hWndhex, 0xffffffff);
		//printf("\n");

		SetForegroundWindow( hWndhex);
		PostMessage( hWndhex, WM_SYSCOMMAND, SC_MOVE, NULL);
		Beep( 750, 300 );
		Sleep(500);
		//WM_SETCURSOR
		//SendMessage( hWndhex, WM_CAPTURECHANGED, NULL, (LPARAM)hWndhex);


		SendMessage( hWndhex, WM_EXITSIZEMOVE, NULL, (LPARAM)hWndhex);

		

		//BlinkWindow( hWndhex);

		/*		SetWindowPos( hWndhex, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		long dwNewLong=GetWindowLongPtr(hWndhex, GWL_STYLE);
		long flagLong=WS_VISIBLE;
		SetWindowLongPtr(hWndhex, GWL_STYLE, dwNewLong & ~flagLong);
		SetWindowPos( hWndhex, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_HIDEWINDOW);

		printf(	"%d", IsWindowVisible( hWndhex));
		printf("0x%p",GetWindowLongPtr(hWndhex, GWL_STYLE));
		UpdateWindow(hWndhex);
		Sleep(500);
		SetWindowLongPtr(hWndhex, GWL_STYLE, dwNewLong );
		SetWindowPos( hWndhex, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		printf(	"%d", IsWindowVisible( hWndhex));
		printf("0x%p",GetWindowLongPtr(hWndhex, GWL_STYLE));
		//UpdateWindow(hWndhex);

		*/


	}
	else printf("窗体不存在\n\n");
}
void AllWindow()
{
	windowcount=0;
	EnumWindows(EnumWindowsProc, WS_VISIBLE);//(LPARAM)
	printf("一共枚举了 %d 个窗体,枚举完毕!\n\n", windowcount);

}


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "chs");

	char input_chars[21]; // room for 20 chars + '\0'
	int input_opt = -1;

	enum { sun=99,mon,tue,wed,thu,fri,sat='tas' };

	while( true )
	{
		printf( "Choose an option:\n" );
		printf( "1 - current process info (default)\n" );
		printf( "2 - sysfader window info\n" );
		printf( "3 - specified window info\n" );
		printf( "4 - enumerates all top-level windows\n" );
		printf( "9 - exit\n" );
		printf( "or type a command : sleep , quit , test\n" );
		printf( "\nenter command:" );

		if( gets_s( input_chars,20 ) == NULL )
			printf( "输入为NULL\n" );
		input_opt = atoi( input_chars );	

		if( input_opt == 0 )
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
				HWND hwndhex=(HWND)0x001D050C;
				OneWindow( hwndhex);
			}
			break;
		case 4:
			{
				AllWindow();
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
				printf( " ========= test mode =============\n");
				printf( "test ruselt:\n");
				//
				//TCHAR szBuffer[8];
				wchar_t szBuffer[80];
				//memset(szBuffer, 0, 80);
				int index;
				index  = swprintf( szBuffer,  9, L"%s", L"123456789"  ); 
				printf( "%d\n",index);
				wprintf( L"%s\n",&szBuffer[6]);


				DWORD aProcesses[1024], cbNeeded, cProcesses;
				EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);

				//
				printf( " ========= test  end =============\n\n");
			}
			break;
		case 'tiuq': //inverse the first 4 letters of 'quit'
			{
				return 0;
			}
			break;
		case sat: //inverse the first 4 letters of 'quit'
			{
				printf( " ========= test  end =============\n\n");
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

