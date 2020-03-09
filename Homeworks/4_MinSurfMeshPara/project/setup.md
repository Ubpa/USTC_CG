# 安装步骤

## 依赖准备

- 下载 stb 头文件，详情请看 [include/_deps/README.md](include/_deps/README.md) 
- 安装 Eigen
- 下载 Qt UI style 资源 [Resources.zip](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/04_MinSurfMeshPara/Resources.zip) 并置于 [src/App/UEngine/](src/App/UEngine/) 下
- 可选（安装方式与之前的 Eigen 共享安装方式类似：下载源码，cmake config 时选择好 install path，得到 vs 工程后执行里边的 INSTALL，然后将相应 bin/ 路径加到系统环境变量 Path 中）
  - [assimp](https://github.com/assimp/assimp)（读取 obj）（没有也可以运行，但作业需要读取 obj）
  - [tinyxml2](https://github.com/leethomason/tinyxml2)（保存场景）

## CMake

准备完依赖后，就可以跑一遍 CMake 了，注意该项目需要在你的 USTC_CG 库上跑，因为 CMake 中用到了 git submodule，而 [.gitmodules](../../../.gitmodules) 在项目根目录。

