#pragma once


BOOL CALLBACK EnumChildProc( HWND hwndParent, LPARAM lParam);

HWND FindAllWindow( HWND hwndParent , char * lpszClass, char * lpszWindow );

BOOL FindWinHandle( );


