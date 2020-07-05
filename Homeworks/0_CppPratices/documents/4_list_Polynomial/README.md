# 4. 基于 list 的多项式类

> **注：** 如果你正在接受这些编程训练，表示你已经同意我们之间的版权协议。请严禁将这些练习的代码（包括你自己的代码）在不经允许的情况下在公共网上公布和散播。一经发现，后果自负。谢谢！－刘利刚 (ligang.liu@gmail.com)

## STL

STL 即 Standard Template Library，是 C++ 语言的一部分，是基于 C++ 封装起来的模版类库。见：

http://staff.ustc.edu.cn/~lgliu/Courses/CodingTraining/CodingSkills/InsideSTL.rar

你现在可以开始学习使用 STL 来编程，主要学习容器 `vector`, `list` 即可，其他的容器以后再学习。

你发现了没有，STL 的 `vector` 的用法与你写的 template DArray 的用法非常类似吧？

是的，你实现的 template DArray 就是 `vector` 的基本原理，可以说是你已经写了一个 `vector` 的“雏形”！因此，你使用 STL 的 `vector` 及其他容器没有任何障碍！ 

## 动态库

动态链接库（Dynamic Link Library 或者 Dynamic-link Library，缩写为 DLL），是微软公司在微软 Windows 操作系统中，实现共享函数库概念的一种方式。这些库函数的扩展名是 ”.dll"、".ocx"（包含ActiveX控制的库）或者 ".drv"（旧式的系统驱动程序）。

在 Windows 中，动态库项目默认不导出任何符号（全局变量，函数，类等），需要标记 `__declspec(dllexport)` 才表示导出该符号

当使用动态库时，需要对导出的符号标记上 `__declspec(dllimport)` 才能导入该符号

对于该需求，我们使用一个条件宏，用来表明是否导出/导出

```c++
#if defined(_WIN32) && defined(Ubpa_AsShared)
#  ifdef Ubpa_Export
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else
#  define DECLSPEC
#endif
```

对于需要输出的类，在关键字 `class` 后添加 `DECLSPEC`，如下

```C++
class DECLSPEC DArray{ /*...*/ };
```

## 作业要求

- 学会使用 `vector` 和 `list`，并了解数组和链表的区别和各自的优点及不足；
- 了解生成和使用动态库
- 用 `list` 完成一个多项式的类，具体接口为：[PolynomialList.h](../../project/include/PolynomialList.h) 

**数据格式** 

> 数据文件夹为：[data/](../../project/data/) 

第一行：第 1 个字符为 'P'，然后空格后跟一正整数，表示多项式的项数

第二行开始：每一行表示多项式的某项的次数和系数

注意：多项式每一项输入可以不按顺序，可以次数相重复，读取时候应该做处理，如排序，合并同类相等。

> **示例** 
>
> ```
> P 4
> 0 2
> 5 -3
> 12 5
> 2 6
> ```
>
> 表示多项式：$f(x)=2+6 x^2 -3 x^5 + 5 x^{12}$ 

## 补充阅读资料

继续阅读并提高 C++ 编程的技巧和经验：

- [提高 C++ 编程的 50 项技巧](http://staff.ustc.edu.cn/~lgliu/Courses/CodingTraining/CodingSkills/SpecificWaysC++.rar) 
- [改进 C++ 程序的 6 个技巧](http://staff.ustc.edu.cn/~lgliu/Courses/CodingTraining/CodingSkills/SixWaysToImprove.rar) 

