# 作业项目

## 要求

### 0. CMake

熟读 [CMakeLists.txt](CMakeLists.txt/)，[cmake/UbpaTool.cmake](cmake/UbpaTool.cmake)，理解每一行代码的含义，若不理解则借助注释，搜索和[官方文档](https://cmake.org/documentation/) 

### 1. 基础的动态数组

> 详细说明见于 [documents/1_BasicDArray](../documents/1_BasicDArray) 

完成 [src/executables/1_BasicDArray](src/executables/1_BasicDArray) 

### 2. 高效的动态数组

> 详细说明文档见于 [documents/2_EfficientDArray](../documents/2_EfficientDArray) 

完成 [src/executables/2_EfficientDArray](src/executables/2_EfficientDArray) 

### 3. 模板动态数组

> 详细说明见于 [documents/3_TemplateDArray](../documents/3_TemplateDArray) 

仿照作业 1 和作业 2，在文件夹 [src/executables/](src/executables) 中添加文件夹 `3_TemplateDArray` 并生成一个 executable

### 4. 基于 `list` 的多项式类

> 详细说明见于 [documents/4_list_Polynomial](../documents/4_list_Polynomial) 

完成 [4_list_Polynomial](src/executables/4_list_Polynomial) 

###  5. 基于 `map` 的多项式类

> 详细说明见于 [documents/5_map_Polynomial](../documents/5_map_Polynomial) 

在文件夹 [src/executables/](src/executables) 中添加文件夹 `5_map_DArray` 并生成一个 executable，测试作业 4 和作业 5 的性能

### 6. 静态库

> 详细说明见于 [documents/6_Lib](../documents/6_Lib) 

1. 将作业 4 中的 [PolynomialList.h](src/executables/4_list_Polynomial/PolynomialList.h) 放到 [include/](include/) 中
2. 将作业 4 中的 [PolynomialList.cpp](src/executables/4_list_Polynomial/PolynomialList.cpp) 复制到文件夹 [src/libraris/PolynomialList/](src/libraris/PolynomialList/) 中
3. 编写文件夹 [src/libraris/PolynomialList/](src/libraris/PolynomialList/) 中的 [CMakeLists.txt](src/libraris/PolynomialList/CMakeLists.txt)，使其能生成静态库 PolynomialList.lib
4. 编写文件夹 [src/executables/6_UseLib/](src/executables/6_UseLib/) 中的 [src/executables/6_UseLib/CMakeLists.txt](src/executables/6_UseLib/CMakeLists.txt)，使其生成一个 executable，并链接步骤 2 的静态库

### 7. 动态库

> 详细说明见于 [documents/7_Dll](../documents/7_Dll) 

1. 将作业 2 中的 [DArray.h](src/executables/2_EfficientDArray/DArray.h) 放到文件夹 [include/](include/) 中
2. 将作业 2 中的 [DArray.cpp](src/executables/2_EfficientDArray/DArray.cpp) 复制到文件夹 [src/libraris/DArray/](src/libraris/DArray/) 中
3. 编写文件夹 [src/libraris/DArray/](src/libraris/DArray/) 中的 [CMakeLists.txt](src/libraris/PolynomialList/CMakeLists.txt)，使其能生成动态库 DArray.dll（同时会有 DArray.lib 生成）
4. 编写文件夹 [src/executables/7_UseDll/](src/executables/7_UseDll/) 中的 [src/executables/7_UseDll/CMakeLists.txt](src/executables/6_UseLib/CMakeLists.txt)，使其生成一个 executable，并链接步骤 2 的动态库

## 注意事项

- 程序默认工作路径为 `bin/`，作业 4 读取文件提供的相对路径为 `../data/xxx` 

