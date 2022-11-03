Building Win32 GUI Applications with MinGW

https://github.com/TransmissionZero/MinGW-Win32-Application/tree/rel/v1.4/src
https://github.com/TransmissionZero/MinGW-Win32-Application/releases
https://www.transmissionzero.co.uk/computing/win32-apps-with-mingw/


In order to get the common controls 6 visual styles, introduced with Windows XP,
you must not only call InitCommonControls (declared in CommCtrl.h), 
but also embed a manifest XML file into your code.
To call InitCommonControls, you must statically link your program to ComCtl32.lib

InitCommonControls
"-lcomctl32",