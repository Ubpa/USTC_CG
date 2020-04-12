# 作业说明


## 1. 基础知识

### 1.1 法向贴图

典型使用方式如下

![image-20200412213207467](assets/README/image-20200412213207467.png)

其中法线贴图在渲染中用于改变原法向，从而影响着色效果。

法线贴图一般为蓝紫色，这是因为贴图中的法向是定义在切空间中的，上方向为 z 方向，对应于 RGB 的 B 通道。

> 示例
>
> ![img](https://learnopengl-cn.github.io/img/05/04/normal_mapping_compare.png)

详细内容可参考：[LearnOpenGL - 法线贴图]([https://learnopengl-cn.github.io/05%20Advanced%20Lighting/04%20Normal%20Mapping/](https://learnopengl-cn.github.io/05 Advanced Lighting/04 Normal Mapping/))。

### 1.2 置换贴图

典型使用方式如下

![image-20200412214519054](assets/README/image-20200412214519054.png)

其中置换贴图用于改变顶点的位置，0 （黑色）表示不动，1（白色）表示沿着法向偏移。

由于要改变顶点坐标，在实时渲染中应在 vertex shader 中采样置换贴图来偏移顶点，因此简单网格应含有大量的内部顶点。

由于置换贴图只改变了顶点的位置，不改变顶点的法向，所以，如果不添加相应的法线贴图的话，渲染效果不太正确，如下

![image-20200412214928623](assets/README/image-20200412214928623.png)

### 1.3 点光源阴影

> 示例
>
> ![img](https://learnopengl-cn.github.io/img/05/03/01/shadow_mapping_with_without.png)

详细内容可参考：[LearnOpenGL - 阴影映射](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/03%20Shadows/01%20Shadow%20Mapping/) 

## 2. 作业要求

本次作业包含三个小项目

- 法线贴图和置换贴图的简单使用
- 用置换贴图进行简单去噪
- 点光源阴影

