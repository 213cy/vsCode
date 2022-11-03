#include <windows.h>
#include <stdio.h>

class WindowConnector
{
	WindowConnector()
	{
		HWND hWndTmp;

		if ( !hWndMain = FindWindowA( "TdxW_MainFrame_Class" , "方正证券泉友通专业版V6.47 - [平台首页]") )
		{
			printf("没有找到 TdxW.exe 主窗口! 初始化失败!");
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