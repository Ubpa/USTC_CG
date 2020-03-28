# 作业说明

[toc]

## 1. 基础知识

### MPM（物质点法）

物质点法（MPM）是一种粒子与网格混合使用的模拟方法，Stomakhin et al. 用MPM实现了雪的模拟。

Hu et al. 2018 提出了 MLS-MPM（移动最小二乘物质点法），对传统 MPM 算法进行了性能优化，并且使代码实现更简洁。

查看[睿客网](https://rec.ustc.edu.cn/share/9e4591a0-68ef-11ea-b1ad-a1939e6c81c9)上的 Homework > Homework7-Simulation-Taichi > Homework7-Simulation-Taichi.pptx 了解算法的详细内容

### Taichi

- Taichi 是 MIT 博士生 Yuanming Hu 开发的计算机图形学代码库，在此基础上可以实现很多物理模拟算法，参考 [Taichi](http://taichi.graphics/) 
- [project](../project/) 中包含的示例代码 [mls-mpm88](../project/src/example/00_mls_mpm88/mls-mpm88.cpp) 是一个 MLS-MPM 算法 2D 实现，只需包含头文件 [taichi.h](https://github.com/Ubpa/USTC_CG_Data/blob/master/Homeworks/07_SimulationTaichi/mls_mpm88/taichi.h) 即可编译运行

## 2. [mls-mpm88](../project/src/example/00_mls_mpm88/mls-mpm88.cpp) 说明

### 2.1 算法说明

MPM 的基本流程是：模拟对象的物理量存储在粒子中，在每一步执行 advance(dt) 函数将模拟向前推进 dt 时间，
其中在每一步中都执行以下步骤，用辅助背景网格更新粒子的运动状态：

1．  P2G：将粒子（质量和动量）通过B样条插值到网格（line18-36）

2．  Update Grid：更新格点的位置（line37-46）

3．  G2P：将网格速度插值到粒子（line48-59）

4．  Update F：粒子形变梯度更新（line61）

5．  Plasticity：更新弹性和塑性梯度（line63-67）

6．  Advection：粒子位置更新（line60）

与传统 MPM 相比，MLS-MPM 只需要改变两个步骤：（2）中格点上力的计算和（4）中粒子形变梯度F的更新。


### 2.2 参数说明

算法及代码中使用的部分参数如下：

- 仿真参数（网格密度 n，模拟步长 dt）
- 物理参数（杨氏模量 E，泊松比 nu，重力加速度）
- 粒子参数（粒子采样的形状，粒子的初始速度，粒子的质量/密度）

## 参考文献

[^Stomakhin13]: Stomakhin et al. "A Material Point Method for Snow Simulation." *Acm Transactions on Graphics (SIGGRAPH 2013)* 
[^Hu18]: Hu et al. "A Moving Least Squares Material Point Method with Displacement Discontinuity and Two-Way Rigid Body Coupling." *Acm Transactions on Graphics (SIGGRAPH 2018)*