#include "stdafx.h"

#include "991.h"


HWND hWndMain , hWndMenu , hWndContainer;
HWND hWndBuy ,hWndSell , hButtonBuy ,hButtonSell;

HWND hWndObj;
char *pInfo[2];

BOOL CALLBACK EnumChildProc( HWND hwndParent, LPARAM lParam)
{
	if( hWndObj = FindWindowExA( hwndParent, NULL, pInfo[0], pInfo[1]) )
		return FALSE;
	else
		return TRUE;
}
HWND FindAllWindow( HWND hwndParent , char * lpszClass, char * lpszWindow )
{
	pInfo[0]=lpszClass;
	pInfo[1]=lpszWindow;
	EnumChildWindows( hwndParent, EnumChildProc, NULL);
	return hWndObj;
}

BOOL FindWinHandle( )
{

	HWND  hWndTmp;

	hWndMain = FindWindowA( "TdxW_MainFrame_Class" , NULL);
	//"方正证券泉友通专业版V6.47 - [平台首页]"
	//"方正证券泉友通专业版V6.47 - [组合图-上证指数]"
	if ( hWndMain == NULL )
	{
		printf("没有找到 TdxW.exe 主窗口! 程序返回!\n");
		return FALSE;
	}

	printf("找到  主窗口( 0x%p ) TdxW.exe ! 开始枚举子窗口...\n" , hWndMain );
	hWndTmp = FindWindowExA( hWndMain , NULL, "AfxControlBar42", NULL);
	hWndTmp = FindWindowExA( hWndMain , hWndTmp , "AfxControlBar42", NULL);
	//
	hWndMenu = FindAllWindow( hWndTmp , "MHPToolBar", "MainViewBar");
	if ( hWndMenu == NULL )
	{
		printf("没有找到 hWndMenu 目标菜单窗口!\n");
		return FALSE;
	}

	printf("找到目标窗口( 0x%p ) hWndMenu ! 继续寻找...\n" , hWndMenu );
	hWndTmp = GetParent( hWndMenu);
	hWndContainer = GetParent( hWndTmp );
	printf("找到操作窗口( 0x%p ) hWndContainer ! 继续寻找...\n" , hWndContainer);
	//====================================================================

	//////////////////////////////////////////
	SendMessage( hWndContainer, WM_COMMAND, 10001 , 0);
	//SendMessage( hWndMenu, WM_MOUSEACTIVATE, (WPARAM)hWndMain, MAKELONG( HTCLIENT,WM_LBUTTONDOWN));
	//SendMessage( hWndMenu, WM_SETCURSOR, (WPARAM)hWndMenu, MAKELONG( HTCLIENT,WM_LBUTTONDOWN));
	//PostMessage(hWndMenu, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG( 32, 16));
	//Sleep(500);
	//PostMessage(hWndMenu, WM_LBUTTONUP, 0, MAKELONG( 32, 16));
	//Sleep(200);
	//////////////////////////////////////////
	hButtonBuy = FindAllWindow( hWndContainer , "Button", "买入下单");
	if ( hButtonBuy == NULL )
	{
		printf("没有找到 hButtonBuy 买入按钮!\n");
		return FALSE;
	}
	printf( "找到买入按钮ID: %d \n" , GetDlgCtrlID( hButtonBuy));
	hWndBuy = GetParent( hButtonBuy);
	printf("找到操作窗口( 0x%p ) hWndBuy ! 继续寻找...\n" , hWndBuy);
	//////////////////////////////////////////
	SendMessage( hWndContainer, WM_COMMAND, 10002 , 0 );
	//SendMessage( hWndMenu, WM_MOUSEACTIVATE, (WPARAM)hWndMain, MAKELONG( HTCLIENT,WM_LBUTTONDOWN));
	//SendMessage( hWndMenu, WM_SETCURSOR, (WPARAM)hWndMenu, MAKELONG( HTCLIENT,WM_LBUTTONDOWN));
	//PostMessage(hWndMenu, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG( 96, 16));
	//Sleep(500);
	//PostMessage(hWndMenu, WM_LBUTTONUP, 0, MAKELONG( 96, 16));
	//Sleep(200);
	//////////////////////////////////////////
	hButtonSell = FindAllWindow( hWndContainer , "Button", "卖出下单");
	if ( hButtonSell == NULL )
	{
		printf("没有找到 hButtonSell 卖出按钮!\n");
		return FALSE;
	}
	printf( "找到卖出按钮ID: %d \n" , GetDlgCtrlID( hButtonSell));
	hWndSell = GetParent( hButtonSell);
	printf("找到操作窗口( 0x%p ) hWndSell ! \n" , hWndSell );


	printf("search complete!\n" );

	return TRUE;
}

