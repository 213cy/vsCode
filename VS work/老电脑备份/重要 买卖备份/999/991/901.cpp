#include "stdafx.h"
#include "901.h"
#include "981.h"


void main(int argc, char* argv[])
{
	setlocale(LC_ALL, "chs");
	//
	DWORD nMode;
	HANDLE hConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hConsoleHandle, &nMode);
	SetConsoleMode(hConsoleHandle, nMode & ~ENABLE_MOUSE_INPUT );//| ENABLE_PROCESSED_INPUT
	//[][][][][][][][][][][][][][][][][][][][][][][][]
	if ( InitWindHandle() ){ 
		printf("\n||||||||||||||||||||||\n");
		printf("初始化成功!继续操作!");
		printf("\n||||||||||||||||||||||\n");
		////////////////////////////
		if (argc==1)
			argv[1]="test";
		switch( *argv[1] )
		{
		case 'b':
			{
				BuyStock(argv);
			}
			break;
		case 's':
			{
				SellStock(argv);
			}
			break;
		case 'r':
			{
				printf("enter reserved choice\n");
				printf("leave reserved choice\n");
			}
			break;
		default:
			printf( "WRONG! invalid argument choice: (%c)%s\n" , *argv[1],&argv[1][1]);
			printf( "   >>> enter test code >>>\n");
			ProgramTest(argc, argv);
			printf( "   <<< leave test code <<<\n");
		}
	}
	else{
		printf("\n----------------------------------\n");
		printf( "initialization failed!operation abandoned." );
		printf("\n----------------------------------\n");
	}


	printf( "\nprogram end!press 'enter' key to exit.\n" );
	getchar();
	return;
}
	//[][][][][][][][][][][][][][][][][][][][][][][][]
void BuyStock(char* argv[])
{
	printf("   >>> buy process >>>\n");
	SendMessage( hWndContainer, WM_COMMAND, 10001 , 0);
	//PostMessage(hWndMenu, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG( 32, 16));
	//PostMessage(hWndMenu, WM_LBUTTONUP, 0, MAKELONG( 32, 16));

	Sleep(200);
	//12005 证券代码
	SendMessageA( GetDlgItem( hWndBuy, 12005), WM_SETTEXT, NULL, (LPARAM) argv[2]);  
	//12006 买入价格
	SendMessageA( GetDlgItem( hWndBuy, 12006), WM_SETTEXT, NULL, (LPARAM) argv[3]); 
	//12007 买入数量
	//Sleep(500);
	SendMessageA( GetDlgItem( hWndBuy, 12007), WM_SETTEXT, NULL, (LPARAM) argv[4]); 

	char szTemp[21];
	/*
	SendMessageA( GetDlgItem( hWndBuy, 12005), WM_GETTEXT, (WPARAM) 11, (LPARAM) szTemp );  
	printf("current stock number: %s\n", szTemp);			
	SendMessageA( GetDlgItem( hWndBuy, 12006), WM_GETTEXT, (WPARAM) 11, (LPARAM) szTemp );  
	printf("buy price : %s\n", szTemp);
	SendMessageA( GetDlgItem( hWndBuy, 12007), WM_GETTEXT, (WPARAM) 11, (LPARAM) szTemp );  
	printf("buy volume : %s\n", szTemp);
	SendMessageA( GetDlgItem( hWndBuy, 12028), WM_GETTEXT, (WPARAM) 11, (LPARAM) szTemp );  
	printf("available funds : %s\n", szTemp);
	SendMessageA( GetDlgItem( hWndBuy, 2022), WM_GETTEXT, (WPARAM) 11, (LPARAM) szTemp );  
	printf("max buy volume : %s\n", szTemp);
	Sleep(200);
	*/
	//2010 buy button id
	PostMessage(hWndBuy, WM_COMMAND, 2010, (LPARAM)hButtonBuy);

	//findwindow
	Sleep(200);
	HWND hWndTmp = FindWindowA( "#32770", "买入交易确认");
	if ( hWndTmp == NULL ){
		printf("ERROR! can not find deal confirm window!\n");
	}
	GetWindowTextA( hWndTmp , szTemp , 21);
	printf("find %s window!\n", szTemp);
	char szDealInfo[201];
	//7013 edit info id
	SendMessageA( GetDlgItem( hWndTmp, 7013), WM_GETTEXT, (WPARAM) 200, (LPARAM) szDealInfo );  
	printf("confirm info:\n%s\n", szDealInfo );
	// 7016 cancel button id
	PostMessage( hWndTmp , WM_COMMAND, MAKELONG( 7016, BN_CLICKED), (LPARAM)GetDlgItem( hWndTmp, 7013));
	// 7015 buy confirm button id
	printf("   <<< buy process end <<<\n");
}
	//[][][][][][][][][][][][][][][][][][][][][][][][]
void SellStock(char* argv[])
{
	printf("   >>> sell process >>>\n");
	SendMessage( hWndContainer, WM_COMMAND, 10002 , 0 );
	//PostMessage(hWndMenu, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG( 96, 16));
	//PostMessage(hWndMenu, WM_LBUTTONUP, 0, MAKELONG( 96, 16));
	///////////////////////////////////
	SendMessage(hWndBuy, WM_COMMAND, MAKELONG( 32, 16), (LPARAM)hButtonBuy);
	////////////////////////////////////
	printf("   <<< sell process end <<<\n");
}
//[][][][][][][][][][][][][][][][][][][][][][][][]

void ProgramTest(int argc, char* argv[])
{
	char input_chars[21]; // room for 20 chars + '\0'
	int input_opt = -1;
	while( true )
	{
		printf( "\n--------------- Command list: -------------\n" );
		printf( "  i - argument list display\n" );
		printf( "  b - buy process \n" );
		printf( "  s - sell process \n" );
		printf( "  t - controls info (default)\n" );
		printf( "  q - exit\n" );
		printf( "  or type a command : new , quit , test\n" );

		printf( "---------------------------- ");
		printf( "enter command: ___ " );
		//memset( input_chars , NULL ,21);
		gets_s( input_chars,21 );
		if ( 0 == input_chars[1] )
			input_opt = input_chars[0];
		else
			input_opt = *((int *)input_chars);

		printf( "---------------------------- ");
		printf( "command value: ___ %d \n" ,input_opt );

		switch( input_opt )
		{
		case 'b':
			{
				BuyStock(argv);
			}
			break;
		case 's':
			{
				SellStock(argv);
			}
			break;
		case 't': 
			{
				printf(" ====== test start ====== \n");
				int i;
				for (i=12000;i<12010;i++){
					printf( "%d : 0x%p \n" , i , GetDlgItem( hWndBuy, i));
				}
				printf(" --------------- \n");
				EnumChildWindows(  hWndBuy, EnumControl, NULL);
				printf(" ====== test end ====== \n");
			}
			break;
		case 'i': //reverse of slee(sleep)
			{
				printf(" ====== argument display ====== \n");
				int k;
				printf("输入参数列表:\n");
				for (k=0;k<argc;k++){
					printf("%s\n",argv[k]);
				}

				printf(" ====== argument display end ====== \n");

			}
			break;
		case 'q': //reverse of slee(sleep)
			{
				printf("quit test in 2 seconds\n");
				Sleep(2000);
				return;
			}
			break;

		case 'wen': //reverse of slee(new)
			{
				printf("\n");
				printf("     XXXXX     XXXXX");
				printf("     XXXXX     XXXXX");
				printf("     XXXXX     XXXXX");
				printf("\n");
			}
			break;

		default:
			printf( "无效的命令: %s\n" , input_chars);
			printf( "请重新选择或输入命令!\n" );
		} // switch
	}// while
}

BOOL CALLBACK EnumControl( HWND hwnd, LPARAM lParam)
{
	printf( "HWND: 0x%p === ID: %d \n" , hwnd , GetDlgCtrlID( hwnd ) );
	return TRUE;
}
