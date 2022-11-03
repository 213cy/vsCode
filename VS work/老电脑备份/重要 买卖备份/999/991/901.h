#pragma once
//
void BuyStock(char* argv[]);
void SellStock(char* argv[]);
void ProgramTest(int argc, char* argv[]);
BOOL CALLBACK EnumControl( HWND hwnd, LPARAM lParam);

//
extern HWND hWndMain , hWndMenu , hWndContainer;
extern HWND hWndBuy ,hWndSell ,hButtonBuy ,hButtonSell;