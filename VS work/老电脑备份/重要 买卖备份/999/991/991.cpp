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
	//"����֤ȯȪ��ͨרҵ��V6.47 - [ƽ̨��ҳ]"
	//"����֤ȯȪ��ͨרҵ��V6.47 - [���ͼ-��ָ֤��]"
	if ( hWndMain == NULL )
	{
		printf("û���ҵ� TdxW.exe ������! ���򷵻�!\n");
		return FALSE;
	}

	printf("�ҵ�  ������( 0x%p ) TdxW.exe ! ��ʼö���Ӵ���...\n" , hWndMain );
	hWndTmp = FindWindowExA( hWndMain , NULL, "AfxControlBar42", NULL);
	hWndTmp = FindWindowExA( hWndMain , hWndTmp , "AfxControlBar42", NULL);
	//
	hWndMenu = FindAllWindow( hWndTmp , "MHPToolBar", "MainViewBar");
	if ( hWndMenu == NULL )
	{
		printf("û���ҵ� hWndMenu Ŀ��˵�����!\n");
		return FALSE;
	}

	printf("�ҵ�Ŀ�괰��( 0x%p ) hWndMenu ! ����Ѱ��...\n" , hWndMenu );
	hWndTmp = GetParent( hWndMenu);
	hWndContainer = GetParent( hWndTmp );
	printf("�ҵ���������( 0x%p ) hWndContainer ! ����Ѱ��...\n" , hWndContainer);
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
	hButtonBuy = FindAllWindow( hWndContainer , "Button", "�����µ�");
	if ( hButtonBuy == NULL )
	{
		printf("û���ҵ� hButtonBuy ���밴ť!\n");
		return FALSE;
	}
	printf( "�ҵ����밴ťID: %d \n" , GetDlgCtrlID( hButtonBuy));
	hWndBuy = GetParent( hButtonBuy);
	printf("�ҵ���������( 0x%p ) hWndBuy ! ����Ѱ��...\n" , hWndBuy);
	//////////////////////////////////////////
	SendMessage( hWndContainer, WM_COMMAND, 10002 , 0 );
	//SendMessage( hWndMenu, WM_MOUSEACTIVATE, (WPARAM)hWndMain, MAKELONG( HTCLIENT,WM_LBUTTONDOWN));
	//SendMessage( hWndMenu, WM_SETCURSOR, (WPARAM)hWndMenu, MAKELONG( HTCLIENT,WM_LBUTTONDOWN));
	//PostMessage(hWndMenu, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG( 96, 16));
	//Sleep(500);
	//PostMessage(hWndMenu, WM_LBUTTONUP, 0, MAKELONG( 96, 16));
	//Sleep(200);
	//////////////////////////////////////////
	hButtonSell = FindAllWindow( hWndContainer , "Button", "�����µ�");
	if ( hButtonSell == NULL )
	{
		printf("û���ҵ� hButtonSell ������ť!\n");
		return FALSE;
	}
	printf( "�ҵ�������ťID: %d \n" , GetDlgCtrlID( hButtonSell));
	hWndSell = GetParent( hButtonSell);
	printf("�ҵ���������( 0x%p ) hWndSell ! \n" , hWndSell );


	printf("search complete!\n" );

	return TRUE;
}

