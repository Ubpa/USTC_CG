# 软件使用说明

这里提供本课程中所使用的一些软件的简单介绍和使用说明等。

## Vistual Studio (VS)

微软的Visual Studio (VS)是Windows 操作系统下最佳的 C++ 集成开发环境。本课程使用微软的`VS 2019`版本来进行代码编写及调试。

- Visual Studio Community 2019可从官网免费下载: [官网](https://visualstudio.microsoft.com/zh-hans/vs/) 
- Visual Studio 2019家庭版/企业版可从科大内网中免费下载（科大的OEM版）

## CMake

本课程要求使用`CMake 3.16.3`版本来构建项目。CMake是众多开源项目采用的方式，如 [pbrt-v3](https://github.com/mmp/pbrt-v3) 。学会使用CMake，就能够很容易使用他人的开源项目。

CMake是一个跨平台的安装（编译）工具，可以用简单的语句来描述所有平台的安装（编译过程），开发者通过编写一种与平台无关的 CMakeList.txt 文件来定制整个编译流程，然后再根据目标用户的平台进一步生成所需的本地化 Makefile 和工程文件。CMake能够有效地描述这些文件之间的依赖关系以及处理命令，当个别文件改动后仅执行必要的处理，而不必重复整个编译过程，可以大大提高软件开发的效率。

- CMake 3.16.3：[官网](https://cmake.org/)，[下载链接](https://github.com/Kitware/CMake/releases/download/v3.16.3/cmake-3.16.3-win64-x64.msi)，[视频教程](https://www.bilibili.com/video/av85644125/)，[官方教程](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)，[官方文档](https://cmake.org/documentation/) 
- 可详细阅读 [官方教程](https://cmake.org/cmake/help/latest/guide/tutorial/index.html) 和 [0. C++ 热身训练](0_CppPratices/)，模仿使用
- CMake 内容比较杂，一下子掌握所有内容不太现实，在日后的使用过程中结合搜索和[官方文档](https://cmake.org/documentation/)来解决实际问题

## QT 

QT是一个跨平台的 C++ 开发库，主要用来开发图形用户界面（Graphical User Interface，GUI）程序，除了可以绘制漂亮的界面（包括控件、布局、交互），还包含很多其它功能，比如多线程、访问数据库、图像处理、音频视频处理、网络通信、文件操作等。本课程使用`QT 5.12.0`版本。

- QT 5.12.0：[官网](https://www.qt.io/)，[官方文档 | Class](https://doc.qt.io/qt-5.12/classes.html) ，[简易说明](Softwares/Qt.md) 
- 简略使用说明[->](Qt.md) 

## GitHub 

本课程要求使用GitHub来管理和维护作业的代码，须设为私有库。

- GitHub Desktop：[官网](https://desktop.github.com/)，[下载链接](https://central.github.com/deployments/desktop/desktop/latest/win32)，[官方文档](https://help.github.com/en/desktop)，[简易说明](Softwares/Github.md) 
- 简略使用说明 [->](Github.md) 

## Typora

Typora是一个所见即所得的Markdown格式文本编辑器，支持Windows、macOS和GNU/Linux操作系统，用于浏览和编辑本地版的GitHub的`*.md`文件。

- Typora：[官网](https://www.typora.io/)，[下载链接](https://www.typora.io/windows/typora-setup-x64.exe?)，[官方文档](http://support.typora.io/) 




