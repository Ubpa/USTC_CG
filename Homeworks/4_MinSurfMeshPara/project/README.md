# 作业完成说明

## UEngine 三角网格框架简介

- UEngine 是一个使用半边结构的三角网格的数据结构框架，我们会不断更新该框架，提升用户使用体验
- UEngine 的安装步骤参考 [setup.md](setup.md) 
- 本课程将在后续的作业中都使用 UEngine，请务必掌握并妥善维护

>  [UGM](https://github.com/Ubpa/UGM) 是一个包含向量和矩阵相关运算的数学库，作业中可能会用到；其使用方式参考其中的示例（去下载 UGM 源码并编译看看测试例子）

## 作业完成步骤

### 框架相关说明

- 框架用 `class TriMesh` 进行渲染，内部存储了简单的顶点位置 `positions`，法向量 `normals` 和纹理坐标 `texcoords` 等；其中提供了基本的数据获取的接口。
  - 如果需要修改三角网格的顶点坐标，可通过以下接口进行修改： `bool TriMesh::Update(const std::vector<pointf3>& positions);`，这时系统会自动更改网格的显示。
  - 如果需要修改顶点的纹理坐标，可通过以下接口进行修改：`bool TriMesh::Update(const std::vector<pointf2>& texcoords);`。这时系统会自动更改网格贴上了纹理的显示（如果已加载了纹理图像）；如果此时还未加载纹理图像，则仍是网格显示方式。可在 UI 上加载（Hierarchy 中选取网格对象，然后再 Attribute 中找到 Material，并在其中的 Albedo Texture 中通过 Load 按钮加载图片（png 格式）。如果设置了纹理坐标，且想显示纹理贴图的效果，别忘了要加载纹理图像。
- 在做网格计算（如本次作业的极小曲面，参数化）时，就使用半边结构 `class HEMesh`，包含了点线面的连接关系。
- 网格计算流程：`class TriMesh` -> `class HEMesh` -> 在 `HEMesh` 上做网格计算 -> `class TriMesh` 

### 目标

- 极小化曲面类：[MinSurf.h](include/Engine/MeshEdit/MinSurf.h) 和 [MinSurf.cpp](src/Engine/MeshEdit/MinSurf.cpp) 
- 参数化类：[Paramaterize.h](include/Engine/MeshEdit/Paramaterize.h) 和 [Paramaterize.cpp](src/Engine/MeshEdit/Paramaterize.cpp) 

### 步骤

- 学习和了解三角网格的半边结构的数据结构及相关接口 [UHEMesh](https://github.com/Ubpa/UHEMesh)，接口如下
  - [HEMesh](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/HEMesh.h) 
  - [TEdge](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/TEdge.h) 
  - [THalfEdge](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/THalfEdge.h) 
  - [TPolygon](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/TPolygon.h) 
  - [TVertex](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/TVertex.h) 
- [UHEMesh](https://github.com/Ubpa/UHEMesh) 具体使用可参考 [IsotropicRemeshing.cpp](src/Engine/MeshEdit/IsotropicRemeshing.cpp) ，`IsotropicRemeshing` 为一个重新进行网格生成的类，无需理解算法，只需看看 HEMesh 的使用方式。
- `class MinSurf` 为我们提供的极小曲面 (minimal surface) 的类，完成接口 `Minimize()` 实现极小曲面的算法；
- `class Paramaterize` 为我们提供的参数化 (parameterization) 的类，模仿 `class MinSurf` 留了 4 个接口，可模仿 `MinSurf` 来完成参数化的实现，也可自行修改接口。计算完毕后，需要按照将结果传给 `class TriMesh` 来更新纹理坐标。注意纹理坐标的范围缺省为 [0,1] x [0,1]，因此映射网格边界的范围最好在 [0,1] x [0,1] 范围。
- 为了可视化参数化的结果（2D 三角网格），用于书写作业报告，可以增加个 UI 界面，可仿照 [Attribute.cpp](src/UI/Attribute.cpp) 来扩充 UI 按钮


## Bugs 记录（助教会逐步 fix 这些 bugs，同学们忽略）

- [ ] 方向光
- [ ] 相机：按钮 To Roamer Camera
- [ ] 法线贴图
- [x] 拖动 area light 时报错

## 依赖库

- [UTemplate](https://github.com/Ubpa/UTemplate) 
- [UHEMesh](https://github.com/Ubpa/UHEMesh) 
- [UDP](https://github.com/Ubpa/UDP) 
- [UGM](https://github.com/Ubpa/UGM) 
- [stb](https://github.com/nothings/stb) 
- [assimp](https://github.com/assimp/assimp) 
- [tinyxml2](https://github.com/leethomason/tinyxml2) 

