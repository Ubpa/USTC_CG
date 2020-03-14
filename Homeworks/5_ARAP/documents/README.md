# 作业说明

实现 SGP 2008 的 ARAP (As-rigid-as-possible) 参数化算法[^08Liu] 

![cow.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/assets/cow.jpg)

## 0. 材料

> 新增

### 非封闭网格（一条边界）

- [Beetle_ABF.obj](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/mesh/Beetle_ABF.obj) 
- [Cow_dABF.obj](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/mesh/Cow_dABF.obj) 
- [Gargoyle_ABF.obj](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/mesh/Gargoyle_ABF.obj) 
- [Isis_dABF.obj](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/mesh/Isis_dABF.obj) 

### 纹理图片

![green_checkerboard.png](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/green_checkerboard.png)

> 下载链接：[green_checkerboard.png](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/green_checkerboard.png) 

## 1. 基础知识

### 1.1 线性化参数方法

作业 4 中的参数化方法是将边界进行固定，能保持 valid (flip-free) 的参数化结果，但是三角形的形变较大

> 思考：三角形的形变量度量？

![head.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/assets/head.jpg)

### 1.2 非线性参数化方法

如果不固定边界，边界的点也有自由度进行移动，能减小三角形的形变量，从而得到更好的参数化结果

![monster.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/assets/monster.jpg)

### 1.3 ARAP 参数化方法

![ARAP.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/05_ARAP/assets/ARAP.jpg)
$$
E(u,A)=\sum_{t=1}^T\Delta_i\|J(L_t(u))-A_t\|^2_F
$$
其中 $\Delta_t$ 是 3D 三角形的面积，$J(L_t(u))$ 是 $L_t$ 的 Jacobian 矩阵

## 2. 实现步骤

- 首先实现 **ASAP 算法**，进一步熟悉网格 Laplacian 矩阵的构建及求解稀疏方程组
- 其次实现 **ARAP 算法** 
  - 局部步骤：相对简单
  - 全局步骤：矩阵结构不变，元素需要更新
- 如果有时间，可实现下文中的 ASAP 方法与 ARAP 方法之间的 **Hybrid 方法**（可选）
- [0. 材料](#0. 材料) 给了一些文中所使用的带有边界的网格数据及测试纹理，使用这些数据来做测试即可
- 在课程百度云中的目录 Homework/Homework5/bin 中有可执行程序，能生成 ASAP、ARAP、Hybrid 方法的结果，可利用其进行参考对比

## 参考文献

[^08Liu]: Ligang Liu, et al. "[**A local/global approach to mesh parameterization.**](http://cs.harvard.edu/~sjg/papers/arap.pdf)" *Computer Graphics Forum* (Proc. SGP). , 2008.


