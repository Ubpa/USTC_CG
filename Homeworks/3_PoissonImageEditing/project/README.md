# 作业项目

## 文件说明

- [include/](inculde/)：头文件
  - [_deps/](_deps/)：依赖库（目前为空）
- [src/](src/)：源代码
- [data/](data/)：资源文件夹（目前为空）

## 使用说明

### 测试图片

本次作业涉及的测试图片下载方式参考 [data/README.md](data/README.md) 

### OpenCV 配置

本次实验必须用到 OpenCV，**配置方式**和**使用示例**务必参考项目 [documents/opencv_example](../documents/opencv_example) 

### Eigen 配置

作业 2 ImageWarping 的项目 [documents/eigen_example](../../2_ImageWarping/documents/eigen_example) 所述的方法是将 Eigen 源码放在了项目内部。

本次项目可采用更合理的方式

> 当然你**可以**沿用之前的方法（将 Eigen/ 放在 [include/_deps/](include/_deps/)），本项目提供了支持
>
> 但不推荐，这样又拷贝了一份 Eigen

- 下载 eigen-3.3.7：[官方网站](http://eigen.tuxfamily.org/)，[下载链接](https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.zip) 
- 用 CMake-GUI 对其进行 Configure（不要点 Generate, Open Project）（较久）
- 在中间的配置框中
  - 找到 `CMAKE_INSTALL_PREFIX`，将其设置成任意位置（不含中文），假设为 `<install-path>`，之后 Eigen 将安装于此
  - 找到 `BUILD_TESTING`，去掉勾选，这样后边快一点（勾选上的话可以看官方的 Eigen 使用示例）
- 配置好后点击 Generate 和 Open Project，然后在解决方案资源管理器中选择 INSTALL，右键选择”生成“，这样 Eigen 就会安装到 `<install-path>` 
  - `<install-path>/include/eigen3/` 包含了 Eigen 的源码
  - `<install-path>/share/eigen3/cmake/` 包含了 CMake 所需的配置文件
- **删除**刚才 CMake 生成的 `<build>` 文件夹（不删除会导致 CMake 会优先从此处找 Eigen，导致错误）
- 创建系统环境变量
  - 变量名：`Eigen3_DIR` 
  - 变量值：`<install-path>/share/eigen3/cmake/`（注意 `<install-path>` 要替换成实际路径）

这样 Eigen 就安装并配置完毕了

> 在对本项目进行 CMake 的 Configure 时，如果配置正确，会在 log 中提示
>
> ```c++
> EIGEN3_FOUND: 1
> EIGEN3_USE_FILE: <install-path>/share/eigen3/cmake/UseEigen3.cmake
> EIGEN3_DEFINITIONS: 
> EIGEN3_INCLUDE_DIR: <install-path>/include/eigen3
> EIGEN3_INCLUDE_DIRS: <install-path>/include/eigen3
> EIGEN3_ROOT_DIR: <install-path>
> EIGEN3_VERSION_STRING: 3.3.7
> ```
>
> ---
>
> 在 CMake 中使用 Eigen 有两种方式
>
> **方式一**（全局）
>
> ```cmake
> find_package(Eigen3 3.3 NO_MODULE REQUIRED)
> # way 1
> include(${EIGEN3_USE_FILE})
> # way 2
> include_directories(${EIGEN3_INCLUDE_DIR}) # maybe legacy in future
> ```
>
> **方式二**（局部）
>
> ```cmake
> find_package(Eigen3 3.3 NO_MODULE REQUIRED)
> add_executable(example example.cpp)
> target_link_libraries(example Eigen3::Eigen)
> ```

### 配置总结

从 OpenCV 和 Eigen 的 CMake 配置可以看出，思路很简单，就是通过 `find_package` 来导入它们的配置。

为了 `find_package` 成功，需要提供 `Find<package-name>.cmake` 或者在环境变量中添加 `<package-name>_DIR`（该目录下含有 `<package-name>Config.cmake`）

这种方法是共通的，以后在使用开源库的时候就知道该如何将开源库配置成 CMake 可使用的方式

## 其他说明

- Qt：[简易说明](../../../Softwares/Qt.md) 
- CMake-GUI 在 configure 的时候，Optional platform for generator 要选 **x64** 
- CMake 一开始会拉取 [UCMake](https://github.com/Ubpa/UCMake)，所以会稍微久一点，拉取的 UCMake 位于 `build/_deps/`，内含一些自定义函数 `Ubpa_xxx` 

