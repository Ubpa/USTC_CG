# 模板代码

此为 C++ 热身训练的模板代码

## 作业

### 1. 基础的动态数组

完成 [1_BasicDArray](src/exe/1_BasicDArray) 

### 2. 高效的动态数组

完成 [2_EfficientDArray](src/exe/2_EfficientDArray) 

### 3. 模板动态数组

仿照作业 1 和作业 2，在文件夹 [src/exe/](src/exe) 中添加文件夹 `3_TemplateDArray` 并生成一个 executable

### 4. 基于 `list` 的多项式类

完成 [4_list_Polynomial](src/exe/4_list_Polynomial) 

###  5. 基于 `map` 的多项式类

在文件夹 [src/exe/](src/exe) 中添加文件夹 `5_map_DArray` 并生成一个 executable，测试作业 4 和作业 5 的性能

### 6. 静态库

1. 将文件夹 [include/](include/) 中的 [PolynomialList.h](include/PolynomialList.h) 替换成作业 4 中的 [PolynomialList.h](src/exe/4_list_Polynomial/PolynomialList.h) 
2. 将作业 4 中的 [PolynomialList.cpp](src/exe/4_list_Polynomial/PolynomialList.cpp) 复制到文件夹 [src/exe/6_useLib/](src/exe/6_useLib/) 
3. 编写文件夹 [src/lib/PolynomialList/](src/lib/PolynomialList/) 中的 [CMakeLists.txt](src/lib/PolynomialList/CMakeLists.txt)，使其能生成静态库 lib
4. 编写文件夹 [src/exe/6_useLib/](src/exe/6_useLib/) 中的 [src/exe/6_useLib/CMakeLists.txt](src/exe/6_useLib/CMakeLists.txt)，使其生成一个 executable，并链接步骤 2 的静态库

