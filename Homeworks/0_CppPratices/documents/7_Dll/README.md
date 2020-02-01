# 7. 动态库

动态链接库（Dynamic Link Library 或者 Dynamic-link Library，缩写为 DLL），是微软公司在微软 Windows 操作系统中，实现共享函数库概念的一种方式。这些库函数的扩展名是 ”.dll"、".ocx"（包含ActiveX控制的库）或者 ".drv"（旧式的系统驱动程序）。

## 生成动态库

参照 [CMake 教程](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)  的 step 9

VS 在生成 dll 时会预定义一个宏，`<target-name>_EXPORTS`，本作业需要用到这个宏，用法如下

```c++
#ifdef _WIN32
#  ifdef DArray_EXPORTS
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif
```

对于需要输出的类，在关键字 `class` 后添加 `DECLSPEC`，如下

```C++
class DECLSPEC DArray{ /*...*/ };
```

在使用 CMake 时，需要用到 [project/cmake/UbpaTool.cmake](../../project/cmake/UbpaTool.cmake) 的函数 `Ubpa_SetupTarget`，调用方法为

```cmake
Ubpa_SetupTarget(MODE "DLL" NAME <target-name> SOURCES <sources> LIBS <libs>)
```

## 使用动态库

参照 [CMake 教程](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)  的 step 9

## 作业要求

- 学会生成动态库及使用动态库；
- 你可在网上查找到更多有关动态库的知识，稍微学习和体会一下；
- 将作业 1.2 的 `DArray` 编写成动态库，测试使用后将工程发给我检查。

## 目的

- 务必体会和学会生成与使用动态库；

