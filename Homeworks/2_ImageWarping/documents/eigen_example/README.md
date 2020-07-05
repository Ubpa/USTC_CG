# Eigen 示例

## 目录结构

本项目目录包含以下若干子目录：

- [src/](src/)：源代码
- [include/](include/)：头文件
  - [_deps/](include/_deps/)：依赖

## 其他说明

- Qt：[简易说明](../../../Softwares/Qt.md) 
- CMake-GUI 在 configure 的时候，Optional platform for generator 要选 **x64** 
- CMake 一开始会拉取 [UCMake](https://github.com/Ubpa/UCMake)，所以会稍微久一点，拉取的 UCMake 位于 `build/_deps/`，内含一些自定义函数 `Ubpa_xxx` 

