# 作业说明

## 1. 基础知识

### 1.0 OpenGL

本次作业在 OpenGL 这一层完成，那么了解 OpenGL 的知识是很有必要的

推荐学习教程 [LearnOpenGL](https://learnopengl-cn.github.io/) 的入门这一章来学习 OpenGL 的基础知识

本次作业挑选了其中的部分章节，提供了框架，只需完成其中缺失部分即可

### 1.1 法向贴图

典型使用方式如下

![normal_map_usage](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/normal_map_usage.jpg)

其中法线贴图在渲染中用于改变原法向，从而影响着色效果。

法线贴图一般为蓝紫色，这是因为贴图中的法向是定义在切空间中的，上方向为 z 方向，对应于 RGB 的 B 通道。

> 示例
>
> ![normal_mapping_compare](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/normal_mapping_compare.jpg)

详细内容可参考：[LearnOpenGL - 法线贴图](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/04%20Normal%20Mapping/)。

### 1.2 置换贴图

典型使用方式如下

![displacement_map_usage](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/displacement_map_usage.jpg)

其中置换贴图用于改变顶点的位置，0 （黑色）表示不动，1（白色）表示沿着法向偏移。

将置换贴图的值转换成顶点偏移量的方式是自定义的，示例如下

- 0-0.5 为下沉，0.5-1.0 为凸起，变化量用一个系数决定
- 0-1.0 为凸起，变化量用一个系数决定
- 上边两种方式可范化成：displacement = lambda * (bias + scale * pixel value)
  - displacement = lambda * (-1 + 2 * pixel value)
  - displacement = lambda * (0 + 1 * pixel value)

由于要改变顶点坐标，在实时渲染中应在 vertex shader 中采样置换贴图来偏移顶点，因此简单网格应含有大量的内部顶点。

由于置换贴图只改变了顶点的位置，不改变顶点的法向，所以，如果不添加相应的法线贴图的话，渲染效果不太正确，如下

![displacement_map_error](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/displacement_map_error.jpg)



常见的用法有水面模拟（每帧切换置换贴图和法线贴图，从而实现水面动态效果）

### 1.3 点光源阴影

> 示例
>
> ![shadow_mapping_with_without](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/shadow_mapping_with_without.jpg)

详细内容可参考：[LearnOpenGL - 阴影映射](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/03%20Shadows/01%20Shadow%20Mapping/) 

## 2. 作业要求

本次作业包含三个小项目

### 2.1 法线贴图和置换贴图

法线贴图的详细内容可参考：[LearnOpenGL - 法线贴图](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/04%20Normal%20Mapping/) 

置换贴图原理简单，参照上边的说明即可，置换函数自定，项目默认为 displacement = lambda * pixel value

在线法线贴图和置换贴图生成的网页：[NormalMap-Online](https://cpetry.github.io/NormalMap-Online/) 

项目默认提供了一个示例的法线贴图和置换贴图，较为简单，同学可行生成或者收集一些资源来丰富测试效果

![cg_normalmap.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/textures/cg_normalmap.jpg)

![cg_displacementmap.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/textures/cg_displacementmap.jpg)

> 示例
>
> ![dn_demo.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/dn_demo.jpg)

### 2.2 用置换贴图进行简单去噪

项目提供的模型如下

![spot.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/spot.jpg)

加了随机噪声之后

![noise.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/noise.jpg)

虽然给顶点加了噪声，但法线还是用了原本的，所以含噪声模型在渲染的不同主要体现在纹理的扭曲和边缘的凹凸不平上。

我们只需将顶点进行合理的偏移就能达到不错的去噪效果。

步骤如下：

- 计算每个顶点的偏移量

$$
\delta_i=p_i-\frac{1}{|N(i)|}\sum_{j\in N(i)}p_j
$$

- 将偏移量投影到法向上

$$
\bar{\delta}_i=\langle\delta_i,\pmb{n}_i\rangle \pmb{n}_i
$$

- 对每一个顶点进行偏移

$$
\bar{p}_i=p_i-\lambda \bar{\delta}_i=p_i-\lambda\langle\delta_i,\pmb{n}_i\rangle \pmb{n}_i
$$

- 我们将 $\langle\delta_i,\pmb{n}_i\rangle$ 存到置换贴图中，注意设置好 bias 和 scale 将值变换到 0 和 1 之间

  > 简单来说，每个顶点有纹理坐标，将图像中该位置设为 bias 和 scale 后的 $\langle\delta_i,\pmb{n}_i\rangle$ 
  >
  > 但图像是离散的，只是按上述做法难免出现重合、缺漏等，因此要根据每个顶点的偏移量，合理插值出整个置换贴图（比如 K 近邻加权均值，最近邻等）

### 2.3 （可选）点光源阴影

详细内容可参考：[LearnOpenGL - 阴影映射](https://learnopengl-cn.github.io/05%20Advanced%20Lighting/03%20Shadows/01%20Shadow%20Mapping/) 

