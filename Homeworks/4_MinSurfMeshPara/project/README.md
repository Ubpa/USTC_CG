# 作业完成说明

## UEngine三角网格框架简介

- UEngine是一个使用半边结构的三角网格的数据结构框架，我们会不断更新该框架，提升用户使用体验
- UEngine的安装步骤参考 [setup.md](setup.md) 
- 本课程将在后续的作业中都使用UEngine，请务必掌握并妥善维护

【注】 [UGM](https://github.com/Ubpa/UGM) 是一个包含向量和矩阵相关运算的数学库，作业中可能会用到；其使用方式参考其中的示例（去下载 UGM 源码并编译看看测试例子）。

## 作业完成步骤

### 主要完成

- 极小化曲面类：[MinSurf.h](inlcude/Engine/MeshEdit/MinSurf.h) 和 [MinSurf.cpp](src/Engine/MeshEdit/MinSurf.cpp) 
- 参数化类：[Paramaterize.h](inlcude/Engine/MeshEdit/Paramaterize.h) 和 [Paramaterize.cpp](src/Engine/MeshEdit/Paramaterize.cpp) 

### 步骤

1. 学习和了解三角网格的半边结构的数据结构及相关接口 [UHEMesh](https://github.com/Ubpa/UHEMesh)，使用方式可参考 [IsotropicRemeshing.cpp](src/Engine/MeshEdit/IsotropicRemeshing.cpp) ，IsotropicRemeshing为一个重新进行网格生成的类。

2. 学习使用class TriMesh三角网格的类，数据有顶点positions、法向量normals、纹理坐标texcoords等；其中提供了基本的数据获取的接口。

2.1 如果需要修改三角网格的顶点坐标，可通过以下接口进行修改：
		bool Update(const std::vector<pointf3>& positions);
这时系统会自动更改网格的显示。
  
2.2 如果需要修改顶点的纹理坐标，可通过以下接口进行修改：
		bool Update(const std::vector<pointf2>& texcoords);
这时系统会自动更改网格贴上了纹理的显示（如果已加载了纹理图像）；如果此时还未加载纹理图像，则仍是网格显示方式。
【注：如果设置了纹理坐标，且想显示纹理贴图的效果，别忘了要加载纹理图像。】
  
3. class MinSurf为我们提供的极小曲面(minimal surface)的类，完成接口Minimize()实现极小曲面的算法；
【注：计算完毕后，需要按照2.1来更新顶点】

4. class Paramaterize为我们提供的参数化(parameterization)的类，留了4个接口，可模仿MinSurf来完成参数化的实现，参数化在Run()中实现；
【注：计算完毕后，需要按照2.2来更新纹理坐标。注意纹理坐标的范围缺省为[0,1]x[0,1]，因此映射网格边界的范围最好在[0,1]x[0,1]范围。】

5. 为了可视化参数化的结果（2D三角网格），用于书写作业报告，可以增加个UI界面，可仿照 [Attribute.cpp](src/UI/Attribute.cpp) 来扩充UI按钮；


## Bugs

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

