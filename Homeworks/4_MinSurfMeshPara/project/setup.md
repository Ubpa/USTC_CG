# 安装步骤

## 项目配置方式更新

> 2020/07/06

找到文件 `Num.h`，注释掉 29 - 46 行

> 2020/03/18

1. 删除 [4_MinSurfMeshPara/project/external](../4_MinSurfMeshPara/project/external) 文件夹
2. 拉取更新（更新了较多文件，如有冲突，谨慎解决）
3. 下载 Ubpa 库 [Ubpa_20200318.zip](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/Ubpa/Ubpa_20200318.zip)（包含 UCMake，UTemplate，UDP，UGM，UHEMesh），将其中的文件夹 `Ubpa/` 放到合适的位置，并将 `<your-path-to-Ubpa>/bin`（`<your-path-to-Ubpa>`要替换成具体路径）加入到系统环境变量 `Path` 中
4. cmake 三连即可，包含多次 `find_package`，如上述配置成功则能找到具体的库，否则会自动 git clone 到 build/_deps 中，并加入到 UEngine 中一并编译

## 依赖准备

- 下载 stb 头文件，详情请看 [include/_deps/README.md](include/_deps/README.md) 

- 安装 Eigen

- 下载 Qt UI style 资源 [Resources.zip](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/04_MinSurfMeshPara/Resources.zip) 并置于 [src/App/UEngine/](src/App/UEngine/) 下

- 可选
  
  > 安装方式大致为：下载源码，cmake config 时选择好 `CMAKE_INSTALL_PREFIX`（默认安装 C 盘会需要管理员权限），得到 vs 工程后执行里边的 INSTALL（Debug 和 Release 都来一遍），然后将相应 bin/ 路径加到系统环境变量 Path 中。之后需要将 build/ 删除
  
  - [assimp](https://github.com/assimp/assimp)（读取 obj）（没有也可以运行框架，但作业需要读取 obj）（INSTALL 时会报错说 `assimp-vc142-mtd.pdb` 找不到，你可以手动在 build 下搜到它并放到其安装路径的 bin/ 中）
  - [tinyxml2](https://github.com/leethomason/tinyxml2)（保存场景）

## CMake

准备完依赖后，就可以跑一遍 CMake 了，注意该项目需要在你的 USTC_CG 库上跑，因为 CMake 中用到了 git submodule，而 [.gitmodules](../../../.gitmodules) 在项目根目录。

