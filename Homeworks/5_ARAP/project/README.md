# 作业完成说明

## 项目配置方式更新

> 2020/03/18

1. 删除 [4_MinSurfMeshPara/project/external](../4_MinSurfMeshPara/project/external) 文件夹
2. 拉取更新（更新了较多文件，如有冲突，谨慎解决）
3. 下载 Ubpa 库 [Ubpa_20200318.zip](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/Ubpa/Ubpa_20200318.zip)（包含 UCMake，UTemplate，UDP，UGM，UHEMesh），将其中的文件夹 `Ubpa/` 放到合适的位置，并将 `<your-path-to-Ubpa>/bin`（`<your-path-to-Ubpa>`要替换成具体路径）加入到系统环境变量 `Path` 中
5. cmake 三连即可，包含多次 `find_package`，如上述配置成功则能找到具体的库，否则会自动 git clone 到 build/_deps 中，并加入到 UEngine 中一并编译

## 作业完成步骤

本次作业在 [作业 4 的框架](../../4_MinSurfMeshPara/project) 上完成所要求的参数化方法（ASAP, ARAP方法），模仿作业4使用半边数据结构来实现算法。

参考步骤如下：

- 模仿 [Paramaterize.h](../../4_MinSurfMeshPara/project/include/Engine/MeshEdit/Paramaterize.h) 和 [Paramaterize.cpp](../../4_MinSurfMeshPara/project/src/Engine/MeshEdit/Paramaterize.cpp) 新建文件 ASAP.h，ASAP.cpp，ARAP.h，ARAP.cpp 等
- 在 [Attribute.cpp](../../4_MinSurfMeshPara/project/src/UI/Attribute.cpp) 中模仿已有示例给 ASAP 方法和 ARAP 方法各添加一个按钮，或者添加适当的 UI 进行参数化的方法选择，从而简化交互
- 参数化后也须显示纹理映射结果

