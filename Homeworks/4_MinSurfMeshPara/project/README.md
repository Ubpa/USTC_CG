# UEngine

## 简介

该框架用于多个作业，请妥善维护，助教会不断更新该框架以完善体验

安装步骤参考 [setup.md](setup.md) 

## 作业 4

主要完成

- 极小化曲面：[MinSurf.h](inlcude/Engine/MeshEdit/MinSurf.h) 和 [MinSurf.cpp](src/Engine/MeshEdit/MinSurf.cpp) 
- 参数化：[Paramaterize.h](inlcude/Engine/MeshEdit/Paramaterize.h) 和 [Paramaterize.cpp](src/Engine/MeshEdit/Paramaterize.cpp) 

如果需要改动 UI（如可视化参数化的中间步骤，方便写报告），可仿照 [Attribute.cpp](src/UI/Attribute.cpp) 进行扩充

你需要用到半边结构 [UHEMesh](https://github.com/Ubpa/UHEMesh)，使用方式参考 [IsotropicRemeshing.cpp](src/Engine/MeshEdit/IsotropicRemeshing.cpp) 

另外还要用到 [UGM](https://github.com/Ubpa/UGM)，使用方式参考其中的示例（去下载 UGM 源码并编译看看测试例子）

## Bugs

- 方向光
- 相机：按钮 To Roamer Camera
- 法线贴图

## 依赖库

- [UTemplate](https://github.com/Ubpa/UTemplate) 
- [UHEMesh](https://github.com/Ubpa/UHEMesh) 
- [UDP](https://github.com/Ubpa/UDP) 
- [UGM](https://github.com/Ubpa/UGM) 
- [stb](https://github.com/nothings/stb) 
- [assimp](https://github.com/assimp/assimp) 
- [tinyxml2](https://github.com/leethomason/tinyxml2) 

