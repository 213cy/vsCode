#include "stdafx.h"
#include "981.h"
#include "991.h"


//const char strPath[14]="winhandle.txt";

BOOL InitWindHandle()
{
	HANDLE hFileOpr;
	HWND hHandles[10];
	//
	hFileOpr = CreateFileA("winhandle.txt", 
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFileOpr == INVALID_HANDLE_VALUE){
		printf("WRONG! file open failed !\n");
		CloseHandle(hFileOpr);
		return FALSE;
	}
	printf("file opened successfully\n");
	DWORD nBytesRead;
	BOOL bResult = ReadFile(hFileOpr, hHandles, 40, &nBytesRead, NULL);
	//ShowError();
	printf("##  read %d bytes data from winhandle.txt  ##\n",nBytesRead);
	if (nBytesRead > 4){
		hWndMain=hHandles[0];
		char szClasaName[51];
		GetClassNameA(hWndMain, szClasaName, 50);
		if ( !strcmp( szClasaName,"TdxW_MainFrame_Class")){
			printf("record file not obsoleted, Assign windows handle values...\n");

			hWndContainer =  hHandles[1];
			hWndMenu      =  hHandles[2];
			hWndBuy	      =  hHandles[3];
			hButtonBuy	  =  hHandles[4];
			hWndSell	  =  hHandles[5];
			hButtonSell   =  hHandles[6];
			printf("window handles loaded successfully!can be continue!\n" );
			CloseHandle(hFileOpr);
			return TRUE;

		}
	}
	////////////////////////////////////////////
	printf("WARNING! not read enough data !\n");
	printf("not load valid window handles,find handles>>>\n");
	////////////////////////////////////////////
	printf("\n   >>> enter search process >>>\n"  );
	if (!FindWinHandle( )){
		printf(" WRONG! windows search failed !\n");
		CloseHandle(hFileOpr);
		printf("   <<< leave search process <<<\n\n"  );
		return FALSE;
	}
	printf("   <<< leave search process <<<\n\n"  );
	////////////////////////////////////////////
	printf("window handles loaded successfully!can be continue!\n" );
	DWORD nBytesWritten;
	hHandles[0] = hWndMain;
	hHandles[1] = hWndContainer;
	hHandles[2] = hWndMenu;
	hHandles[3] = hWndBuy;
	hHandles[4] = hButtonBuy;
	hHandles[5] = hWndSell;
	hHandles[6] = hButtonSell;
	hHandles[7] = (HWND)EOF;

	SetFilePointer( hFileOpr, 0, NULL, FILE_BEGIN);
	//ShowError();
	WriteFile(hFileOpr, hHandles, 40, &nBytesWritten, NULL);
	//ShowError();
	printf("##  write %d bytes data to winhandle.txt  ##\n",nBytesWritten);
	if (nBytesWritten <10){
		printf("WARNING! write wrong data! handles record failed! \n");
	}
	else{
		printf("handles write complete! please continue !\n");
	}
	CloseHandle(hFileOpr);
	return TRUE;

}
void ShowError()
{
	DWORD eNum = GetLastError( );
	TCHAR sysMsg[256];

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default lang.
		sysMsg, 256, NULL );

	wprintf( L" WARNING:  failed with error %d : %s", eNum, sysMsg );

}