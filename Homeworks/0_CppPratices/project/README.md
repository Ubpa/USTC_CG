# 作业项目

## 目录结构

本项目目录包含以下若干子目录：
- `\cmake`: 编译配置文件。须熟读 [CMakeLists.txt](CMakeLists.txt/)，[cmake/UbpaTool.cmake](cmake/UbpaTool.cmake)，理解每一行代码的含义，若不理解则借助注释，搜索和[官方文档](https://cmake.org/documentation/) 
- `\bin`: 为项目程序默认的工作路径，及编译生成的可执行文件`*.exe`放于该目录中
- `\data`: 存放本次作业项目的数据；注：第 4 个小练习读取文件提供的相对路径为 `../data/xxx` 
- `\include`: 一般放置公用的库头文件，在代码中通过`#include <*.h>`来引用，须通过头文件路径设置才能找到
- `\src`：本次作业项目的源代码

## 代码要求

本次热身练习包含的7个小练习的代码编写要求如下：


### 小练习 1. 基础的动态数组

> 详细说明见于 [documents/1_BasicDArray](../documents/1_BasicDArray) 

完成 [src/executables/1_BasicDArray](src/executables/1_BasicDArray) 

### 小练习 2. 高效的动态数组

> 详细说明文档见于 [documents/2_EfficientDArray](../documents/2_EfficientDArray) 

完成 [src/executables/2_EfficientDArray](src/executables/2_EfficientDArray) 

### 小练习 3. 模板动态数组

> 详细说明见于 [documents/3_TemplateDArray](../documents/3_TemplateDArray) 

仿照小练习 1 和小练习 2，在文件夹 [src/executables/](src/executables) 中添加文件夹 `3_TemplateDArray` 并生成一个 executable

### 小练习 4. 基于 `list` 的多项式类

> 详细说明见于 [documents/4_list_Polynomial](../documents/4_list_Polynomial) 

完成 [4_list_Polynomial](src/executables/4_list_Polynomial) 

###  小练习 5. 基于 `map` 的多项式类

> 详细说明见于 [documents/5_map_Polynomial](../documents/5_map_Polynomial) 

在文件夹 [src/executables/](src/executables) 中添加文件夹 `5_map_DArray` 并生成一个 executable，测试小练习 4 和小练习 5 的性能

### 小练习 6. 静态库

> 详细说明见于 [documents/6_Lib](../documents/6_Lib) 

1. 将小练习 4 中的 [PolynomialList.h](src/executables/4_list_Polynomial/PolynomialList.h) 放到 [include/](include/) 中
2. 将小练习 4 中的 [PolynomialList.cpp](src/executables/4_list_Polynomial/PolynomialList.cpp) 复制到文件夹 [src/libraries/PolynomialList/](src/libraries/PolynomialList/) 中
3. 编写 [src/libraries/PolynomialList/CMakeLists.txt](src/libraries/PolynomialList/CMakeLists.txt)，使其能生成静态库 PolynomialList.lib
4. 编写 [src/executables/6_UseLib/CMakeLists.txt](src/executables/6_UseLib/CMakeLists.txt)，使其能生成一个 executable，并链接步骤 3 的静态库

### 小练习 7. 动态库

> 详细说明见于 [documents/7_Dll](../documents/7_Dll) 

1. 将小练习 2 中的 [DArray.h](src/executables/2_EfficientDArray/DArray.h) 放到文件夹 [include/](include/) 中
2. 将小练习 2 中的 [DArray.cpp](src/executables/2_EfficientDArray/DArray.cpp) 复制到文件夹 [src/libraries/DArray/](src/libraries/DArray/) 中
3. 编写 [src/libraries/DArray/CMakeLists.txt](src/libraries/PolynomialList/CMakeLists.txt)，使其能生成动态库 DArray.dll（同时会有 DArray.lib 生成）
4. 编写 [src/executables/7_UseDll/CMakeLists.txt](src/executables/6_UseLib/CMakeLists.txt)，使其能生成一个 executable，并链接步骤 3 的动态库

