# 作业完成说明

## UEngine 三角网格处理框架简介

### UEngine简介
- UEngine 是一个三角网格曲面的处理与渲染的框架，其安装步骤可参考 [setup.md](setup.md) 
- 本课程将在后续的作业中都使用 UEngine，请务必掌握并妥善维护
- 该框架仍在不断更新，会不断提升用户使用体验；如在使用过程中遇到任何问题或 bugs，可及时联系助教
- 建议大家在作业之前，多操作下 UEngine 操作界面，感受它所能得到的呈现结果。比如，在左边的 `Hierachy` 的 `mesh` 处点击右键可添加一个3D网格数据；在右边的 `Attribute` 处可以改变各种属性，然后看看渲染窗口中的变换

### UEngine中的三角网格数据结构

三角网格本质上是个图的数据结构，有存储空间与计算效率的矛盾与折衷。UEngine 框架中同时使用了 2 个三角网格的数据结构：
 - `class TriMesh` 为面向渲染的数据结构，存储量少（仅仅存储了 `positions`, `normals` 等数组），作为 vertex buffer 传给 OpenGL 渲染管线进行渲染
 - `class HEMesh` 为面向计算的数据结构，使用了半边结构 (Half-Edge data structure) 的数据结构，存储量多（存储了点、线、面的连接关系），便于高效网格的计算与处理
 - 两个三角网格对象中都提供了相互数据结构转换的接口，比如 `Init()` 等
 - 一般网格处理与计算的流程：`class TriMesh`  -> `class HEMesh` -> 在 `HEMesh` 上做网格的处理与计算操作 -> `class TriMesh` ，即使用 `TriMesh` 处理 I/O 读取网格数据，然后转为 `HEMesh` 在其上做处理与计算，然后再转为 `TriMesh` 进行渲染。


### TriMesh 三角网格数据结构

- `class TriMesh` 是用于渲染，内部仅存储了顶点 `positions`，法向量 `normals` 和纹理坐标 `texcoords` 等列表，这些数据作为 vertex buffer 直接传给OpenGL渲染管线进行渲染
  - 如果需要修改三角网格的顶点坐标，可通过以下接口进行修改： `bool TriMesh::Update(const std::vector<pointf3>& positions);`，系统会自动更改网格的显示
  - 如果需要修改顶点的纹理坐标，可通过以下接口进行修改：`bool TriMesh::Update(const std::vector<pointf2>& texcoords);`，系统会自动纹理映射的结果（如果已加载了纹理图像）；如果此时还未加载纹理图像，则仍是网格显示方式

### UHEMesh 三角网格数据结构

- `class HEMesh` [UHEMesh](https://github.com/Ubpa/UHEMesh) 使用半边结构进行三角网格的存储，存储了点、线、面之间的连接关系，主要的几何对象如下：
  - [HEMesh](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/HEMesh.h) 
  - [TEdge](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/TEdge.h) 
  - [THalfEdge](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/THalfEdge.h) 
  - [TPolygon](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/TPolygon.h) 
  - [TVertex](https://github.com/Ubpa/UHEMesh/blob/master/include/UHEMesh/TVertex.h) 
- 可通过浏览参考代码 [IsotropicRemeshing.cpp](src/Engine/MeshEdit/IsotropicRemeshing.cpp) 来了解和学习半边结构数据结构的使用规则
  - `IsotropicRemeshing` 为一个重新进行网格生成的类，无需理解算法（算法较为复杂），同学们只需看看 HEMesh 中各种数据的使用方法并模仿其使用方式，无须花很多时间去了解其中的算法

### Graphics Math 库

- Ubpa Graphics Math 库 [UGM](https://github.com/Ubpa/UGM) 是一个简单的包含向量和矩阵相关运算的数学库，会经常用到；可参考其中的示例（去下载 UGM 源码并编译看看测试例子）快速学习使用


## 作业完成步骤

### 目标

- 极小化曲面类：[MinSurf.h](include/Engine/MeshEdit/MinSurf.h) 和 [MinSurf.cpp](src/Engine/MeshEdit/MinSurf.cpp) ，在其中完成极小曲面生成算法
  - 求解好极小曲面后，需要重新更新顶点法向方向，渲染才是光滑的极小曲面；否则渲染效果看起来是具有原始网格的几何细节的
- 参数化类：[Paramaterize.h](include/Engine/MeshEdit/Paramaterize.h) 和 [Paramaterize.cpp](src/Engine/MeshEdit/Paramaterize.cpp) ，在其中完成网格参数化算法
  - 显示纹理映射结果（可选）

### 步骤

- `class MinSurf` 为极小曲面 (minimal surface) 的类，在接口 `Minimize()` 中实现极小曲面的算法
- `class Paramaterize` 为参数化 (parameterization) 的类，模仿 `class MinSurf` 留了 4 个接口，可模仿 `MinSurf` 来完成参数化的实现，也可自行添加其他接口
  - 可以将参数化的结果直接修改网格的顶点坐标（比如z分量为0），即可显示参数化结果

- 显示纹理映射（可选）
  - 增加按钮：为了可视化参数化的结果，可以在 UI 界面中增加个按钮用于显示纹理映射结果，可仿照 [Attribute.cpp](src/UI/Attribute.cpp) 来增添 UI 按钮
  - 添加网格：建议保留上面的参数化结果用于显示。如需显示纹理映射结果，可再添加一个网格（在左边的`Hierachy`的 `mesh`处点击右键再添加一个3D网格数据）
  - 加载纹理：在右边的 `Attribute` 处的 `Material` 处的  `Albedo Texture` 中通过 Load 按钮加载图片（png 格式）
  - 纹理坐标：在`class Paramaterize` 中计算好的参数化的值（范围最好为 [0,1] x [0,1]）来更新网格的纹理坐标，即可看到贴上纹理的三维网格曲面
  
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

