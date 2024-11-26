在Windows命令行中使用cl.exe——如何设置环境变量
https://blog.csdn.net/weixin_43997331/article/details/104764873

即使指定了 libpath ,还是得手动给连接器指定 链接库Library
在源文件中可以用 #pragma comment(lib, "XXX.lib") 给连接器指定参数.

使用不同的生成方法生成应用 - Visual Studio (Windows) | Microsoft Learn
https://learn.microsoft.com/zh-cn/visualstudio/ide/compiling-and-building-in-visual-studio?view=vs-2022  

Visual Studio 中的 C/C++ 项目和生成系统 | Microsoft Learn
https://learn.microsoft.com/zh-cn/cpp/build/projects-and-build-systems-cpp?view=msvc-170  

MSVC 编译器命令行语法 | Microsoft Learn
https://learn.microsoft.com/zh-cn/cpp/build/reference/compiler-command-line-syntax?view=msvc-170  

CL 环境变量 | Microsoft Learn
https://learn.microsoft.com/zh-cn/cpp/build/reference/cl-environment-variables?view=msvc-170 