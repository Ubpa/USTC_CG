# 作业项目

## 目录结构

本项目目录包含以下若干子目录：

- [`src/`](src/)：本次作业项目的源代码

## 其他说明

- Qt：[简易说明](../../../Softwares/Qt.md) 
- CMake-GUI 在 configure 的时候，Optional platform for generator 要选 **x64** 
- CMake 一开始会拉取 [UCMake](https://github.com/Ubpa/UCMake)，所以会稍微久一点，拉取的 UCMake 位于 `build/_deps/`，内含一些 CMake 的自定义函数 `Ubpa_xxx`，详细请看 [UbpaTool.cmake](https://github.com/Ubpa/UCMake/blob/master/cmake/UbpaTool.cmake)，构建好的工程中会自带一些 UCMake 中的[示例项目](https://github.com/Ubpa/UCMake/tree/master/src)，可以查看模仿

