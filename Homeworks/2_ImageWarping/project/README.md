# 作业项目

## 目录结构

本项目目录包含以下若干子目录：

- [src/](src/)：源代码
- [include/](include/)：头文件
  - [_deps/](include/_deps/)：依赖（目前为空）

## 使用说明

### Eigen

如需 Eigen，参考 [Eigen 示例项目](../documents/eigen_example) 

### ANN (Optional)

如需 ANN，参考 [ANN 示例项目](../documents/ann_example/) 

对于本项目，在 CMake 进行 Configure 后，配置框中找到 `USE_ANN` 并勾选上，然后重新 Configure 后再 Generate 和 Open Project 即可完成配置

## 其他说明

- Qt：[简易说明](../../../Softwares/Qt.md) 
- CMake-GUI 在 configure 的时候，Optional platform for generator 要选 **x64** 
- CMake 一开始会拉取 [UCMake](https://github.com/Ubpa/UCMake)，所以会稍微久一点，拉取的 UCMake 位于 `build/_deps/`，内含一些 CMake 的自定义函数 `Ubpa_xxx`，详细请看 [UbpaTool.cmake](https://github.com/Ubpa/UCMake/blob/master/cmake/UbpaTool.cmake)，构建好的工程中会自带一些 UCMake 中的[示例项目](https://github.com/Ubpa/UCMake/tree/master/src)，可以查看模仿

