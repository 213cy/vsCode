#include <windows.h>
#include <stdio.h>

class WindowConnector
{
	WindowConnector()
	{
		HWND hWndTmp;

		if ( !hWndMain = FindWindowA( "TdxW_MainFrame_Class" , "����֤ȯȪ��ͨרҵ��V6.47 - [ƽ̨��ҳ]") )
		{
			printf("û���ҵ� TdxW.exe ������! ��ʼ��ʧ��!");
			bState = FALSE;
			return;
		}
		hWndTmp = FindWindowExA( hWndMain , NULL, "AfxControlBar42", NULL);
		hWndTmp = FindWindowExA( hWndMain , hWndTmp , "AfxControlBar42", NULL);
		//
		hWndMenu = FindAllWindow( hWndTmp , "MHPToolBar", "MainViewBar");
	}

	HWND FindAllWindow( HWND hwndParent , char * lpszClass, char * lpszWindow )
	{
		szClassObj=lpszClass;
		szWindowObj=lpszWindow;
		EnumChildWindows( hwndParent, &WindowConnector::EnumChildProc, NULL);
		return hWndObj;
	}
	BOOL CALLBACK EnumChildProc( HWND hwndParent, LPARAM lParam)
	{
		if( hWndObj = FindWindowExA( hwndParent, NULL, szClassObj, szWindowObj) )
			return FALSE;
		else
			return TRUE;
	}

	WindowConnector( char * lpszClassMain, char * lpszWindowMain, char * lpszClassObj, char * lpszWindowObj)
	{
	}

	bool bState;

	HWND hWndMain;
	HWND hWndObj;
	HWND hWndMenu;
	char szClassMain;
	char szWindowMain;
	char *szClassObj;
	char *szWindowObj;
};