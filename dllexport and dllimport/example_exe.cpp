#include <stdio.h>
#include<Windows.h>
#include "example_dll.h"

int main(void)
{
        hello("World");
        printf("%d\n", Double(333));
        CppFunc();

        MyClass a;
        a.func();
        MessageBoxW(NULL, L"Visual C++" , L"窗口消息", 0);

        //MessageBox(NULL, (LPCSTR)"Visual C++" , (LPCSTR) "窗口消息", 0);
        //MessageBox(NULL, _T"窗Visual C++" , _T"窗口dddd消息", 0);
        return 0;
}