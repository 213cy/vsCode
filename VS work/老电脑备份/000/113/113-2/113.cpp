// 113.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#include "stdlib.h"


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	//printf("hwnd:%d,lParam:%d\n",sizeof(hwnd),sizeof(lParam));
	//printf("hwnd:%p,lParam:%p\n",hwnd,lParam);
	//printf("hwnd:%p,lParam:%p\n",&hwnd,&lParam);

	/*
	char szTemp2[80];
	GetWindowTextA(hwnd, szTemp2, 80);
	printf( "0x%p: %s\n" ,hwnd, szTemp2);
	*/

	TCHAR szTemp[80];
	GetWindowText(hwnd, szTemp, 80);
	wprintf( L"0x%p: %s\n" ,hwnd, szTemp);

	/*

	::GetClassName(hwnd, szBuffer, sizeof(szBuffer));

	HINSTANCE hInstance = (HINSTANCE)::GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
	::GetModuleFileName(hInstance, szBuffer, sizeof(szBuffer));

	HWND hwndParent = ::GetParent(hwnd);
	::GetWindowText(hwndParent, szTemp, sizeof(szTemp));
	_stprintf_s(szBuffer, 256, szFormat, hwndParent, szTemp);

	RECT rect;
	::GetWindowRect(hwnd, &rect);
	_stprintf_s(szBuffer, 256, _T("(%d,%d) - (%d,%d)"), rect.left, rect.top,
	rect.right, rect.bottom);

	LONG lStyle = ::GetWindowLong(hwnd, GWL_STYLE);
	_stprintf_s(szBuffer, 256, _T("%08lX"), lStyle);
	*/

	return ( 1 >= 0 ) ? TRUE : FALSE;
}

void WalkWindowList()
{

	EnumWindows(EnumWindowsProc, (LPARAM)NULL);

}


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "chs");

	char input_chars[21]; // room for 20 chars + '\0'
	int input_opt = -1;

	while( true )
	{
		printf( "Choose an option:\n" );
		printf( "1 - as ISequentialStream/IStream (default)\n" );
		printf( "2 - enumerates all top-level windows\n" );
		printf( "3 - skip BLOBs\n" );
		printf( "9 - sleep\n" );
		printf( "0 - exit\n" );
		printf( "\nenter command:" );

		if( gets_s( input_chars,20 ) == NULL )
			printf( "输入为NULL\n" );
		input_opt = atoi( input_chars );	

		if( input_opt > 9 )
			input_opt = atoi( input_chars );
		if( input_opt == 0 )
		{
			int * pint;
			printf( "%d\n" ,*((int *)input_chars));


			const char aaa[13]="slee";
			printf( "%d\n" ,aaa);
			printf( "%d\n" ,(int)("sleg"));
			printf( "%d\n" ,(int)("slee"));
			input_opt = (int)(input_chars);
		}


		switch( input_opt )
		{
		case 1:
			{
				WalkWindowList();
				printf( "枚举完毕!\n\n" );
			}
			break;
		case 2:
			{
				EnumWindows(EnumWindowsProc, NULL);
				printf( "枚举完毕!\n\n" ); 
			}
			break;
		case 9:
			{
				Sleep(2000);
			}
			break;
		case 10:
			{
				return 0;
			}
			break;

		default:
			printf( "无效的选项: %d 或无效的命令: %s\n" ,atoi( input_chars ), input_chars);
			printf( "请重新选择或输入命令!\n\n" );


			/*
			_locale_t locale,locale2;

			locale = _create_locale( LC_NUMERIC ,"chs");//LC_ALL
			locale2 = _get_current_locale();

			wprintf( L"请重新选择dfasdf入命令\n\n",locale);
			wprintf( L"请重新选择dfasdf入命令\n\n",locale2);
			*/



		} // switch
	}




}

