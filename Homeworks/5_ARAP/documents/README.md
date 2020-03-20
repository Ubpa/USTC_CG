# 作业说明

## 1. 基础知识

### 3D 四面体网格

对实体的模拟通常将实体剖分为四面体单元（也有六面体单元）：![1](README.assets/1.PNG)

### 网格剖分

3d模型数据通常只会给出表面网格（如obj，stl文件），进行模拟前就需要对进行三角剖分从而得到四面体网格：

![2](README.assets/2.PNG)

### 弹簧质点模型

对于四面体网格可以将其每个顶点看为一个质点，将其每条边看为一根弹簧来进行模拟（对于三角网格也可以这样进行模拟，可以以此来模拟布料）：

![4](README.assets/4.PNG)

## 2. 模拟方法

问题：由前n帧信息，求得第n+1帧信息（位移$x$，速度$v$）(设时间步长为$h$)？

### 欧拉隐式方法

$$
\boldsymbol x_{n+1}=\boldsymbol x_n+h\boldsymbol v_{n+1},\\
\boldsymbol v_{n+1}=\boldsymbol v_n+h\boldsymbol M^{-1}(\boldsymbol f_{int}(t_{n+1}) +\boldsymbol f_{ext}),
$$

记
$$
\boldsymbol y =\boldsymbol x_n + h\boldsymbol v_n + h^2\boldsymbol M^{-1}\boldsymbol f_{ext}, \tag{*}
$$
则原问题转化为求解关于x的方程：
$$
\boldsymbol g(\boldsymbol x) = \boldsymbol M(\boldsymbol x-\boldsymbol y) -h^2\boldsymbol f_{int}(\boldsymbol x) = 0,
$$
利用牛顿法求解该方程，主要迭代步骤：
$$
\boldsymbol x^{(k+1)}=\boldsymbol x^{(k)}-(\nabla \boldsymbol g(\boldsymbol x^{(k)}))^{-1}\boldsymbol g(\boldsymbol x^{(k)}).
$$

迭代初值可选为$\boldsymbol x^{(0)}=y$ .

迭代得到位移$x$后更新速度$v_{n+1}=(x_{n+1}-x_{n})/h$






上式中涉及关于弹力的求导，对于单个弹簧（端点为$x_1$，$x_2$），劲度系数为$k$，原长为$l$，有：
$$
\boldsymbol x_1所受弹力：     \boldsymbol f_1(\boldsymbol x_1,\boldsymbol x_2)=k(||\boldsymbol x_1-\boldsymbol x_2||-l)\frac{\boldsymbol x_2-\boldsymbol x_1}{||\boldsymbol x_1-\boldsymbol x_2||},\\
\boldsymbol x_2所受弹力：     \boldsymbol f_2(\boldsymbol x_1,\boldsymbol x_2)=-\boldsymbol f_1(\boldsymbol x_1,\boldsymbol x_2),
$$
对
$$
\boldsymbol h(\boldsymbol x)=k(||\boldsymbol x||-l)\frac{-\boldsymbol x}{||\boldsymbol x||},
$$
求导有
$$
\frac{ d  \boldsymbol h}{d \boldsymbol x} = k(\frac{l}{||\boldsymbol x||}-1)\boldsymbol I-kl||\boldsymbol x||^{-3}\boldsymbol x \boldsymbol x^T.
$$
带入弹力公式得：
$$
\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1} =\frac{\partial  \boldsymbol h(\boldsymbol x_1-\boldsymbol x_2)}{\partial \boldsymbol x_1}=k(\frac{l}{||\boldsymbol r||}-1)\boldsymbol I-kl||\boldsymbol r||^{-3}\boldsymbol r \boldsymbol r^T,其中\boldsymbol r=\boldsymbol x_1-\boldsymbol x_2, \boldsymbol I为单位阵\\
\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_2}=-\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1},\,\,\,\,\,\,\,\,\,\,
\frac{\partial  \boldsymbol f_2}{\partial \boldsymbol x_1}=-\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1},\,\,\,\,\,\,\,\,\,\,
\frac{\partial  \boldsymbol f_2}{\partial \boldsymbol x_2}=\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1},
$$
对所有弹簧求导并组装即可求得力的导数（组装为稀疏矩阵，矩阵为对称阵）。



### 加速方法（projective dynamic）

（参考论文“Fast Simulation of Mass-Spring Systems”）

在上述欧拉方法中，对于内力(为保守力)有：
$$
\boldsymbol f_{int}(x)=-\nabla E(\boldsymbol x)
$$
故对方程$(*)$的求解可以转为为一个最小化问题：
$$
\boldsymbol x_{n+1}=\mathop{min}\limits_{x}\frac{1}{2}(\boldsymbol x-\boldsymbol y)^T\boldsymbol M(\boldsymbol x-\boldsymbol y)+h^2E(\boldsymbol x)
$$
同时对于弹簧的弹性势能可以描述为一个最小化问题：
$$
\frac{1}{2}k(||\boldsymbol p_1-\boldsymbol p_2||-r)^2=\frac{1}{2}k \mathop{min}\limits_{||\boldsymbol d||=r}||\boldsymbol p_1-\boldsymbol p_2-\boldsymbol d||^2,
$$
从而原问题转化为：
$$
\boldsymbol x_{n+1}=\mathop{min}\limits_{x,\boldsymbol d\in\boldsymbol U}\frac{1}{2}\boldsymbol x^T(\boldsymbol M+h^2\boldsymbol L)\boldsymbol x-h^2\boldsymbol x^T\boldsymbol J \boldsymbol d-\boldsymbol x^T \boldsymbol M \boldsymbol y
$$
其中
$$
\boldsymbol U=\{\boldsymbol d=(\boldsymbol d_1,\boldsymbol d_2,...,\boldsymbol d_s),\boldsymbol d_s\in R^3,||\boldsymbol d_i||=l_i \}\,\,\,\,(l_i为第i个弹簧原长),
$$
<img src="README.assets/5.PNG" alt="5" style="zoom: 67%;" />

从而可以对x，d迭代优化求得该优化问题的解：
$$
\boldsymbol x 优化：\,\,求解方程(\boldsymbol M+h^2\boldsymbol L)\boldsymbol x=h^2\boldsymbol J \boldsymbol d+ \boldsymbol M \boldsymbol y（这里可以预分解矩阵），\\
\boldsymbol d 优化：\boldsymbol d_i=l_i\frac{\boldsymbol p_{i_1}-\boldsymbol p_{i_2}}{||\boldsymbol p_{i_1}-\boldsymbol p_{i_2}||}（这里l_i为第i个弹簧原长，\boldsymbol p_{i_1}，\boldsymbol p_{i_2}为其两端点），
$$
重复迭代过程直到收敛。



## 3.边界条件和约束

通常模拟过程中物体会有各种约束或额外条件，例如物体被固定了几个点。

### 外力条件

物体受到的外力可以直接加在模拟的外力项中，其导数为0；对于重力，可以将其加在外力中，另一方面，重力为保守力，也可以将重力势能加在能量项中与弹性势能何并。

### 位移约束

这里主要考虑固定部分质点的情形，一种考虑是在每一帧中求出该点的内力，再施加与该内力大小相同，方向相反的外力，但与上一种情形不同的是，若该内力对位移导数不为0，则该外力对位移导数也不为0，需要将其导数考虑进去；另一种处理方法为仅考虑真正的自由坐标，降低问题的维数，具体如下：

将所有n个质点的坐标列为列向量$x\in R^{3n}$，将所有m个自由质点坐标（无约束坐标）列为列向量$x_f\in R^{3m}$,则两者关系：
$$
\boldsymbol x_f=\boldsymbol K\boldsymbol x,\\\boldsymbol x=\boldsymbol K^T\boldsymbol x_f+\boldsymbol b,
$$
其中$K\in R^{3m\times 3n}$为单位阵删去约束坐标序号对应行所得的稀疏矩阵，$b$为与约束位移有关的向量，计算为$b=x-K^TKx$, 若约束为固定质点则$b$为常量。由此我们将原本的关于$x$的优化问题转化为对$x_f$的优化问题：欧拉隐式方法中求解方程为：
$$
\boldsymbol g_1(\boldsymbol x_f) = K(\boldsymbol M(\boldsymbol x-\boldsymbol y) -h^2\boldsymbol f_{int}(\boldsymbol x)) = 0,\\
梯度：\nabla_{x_f} \boldsymbol g_1(\boldsymbol x_f) = K\nabla_{x} \boldsymbol g(\boldsymbol x)K^T,\\
$$
加速方法中优化问题中$x$迭代步骤转化为求解关于$x_f$的方程：
$$
K(\boldsymbol M+h^2\boldsymbol L)K^T\boldsymbol x_f=K(h^2\boldsymbol J \boldsymbol d+ \boldsymbol M \boldsymbol y-(\boldsymbol M+h^2\boldsymbol L)\boldsymbol b)
$$

## 4.作业要求

- 了解四面体网格数据及其处理方式，了解使用tetgen库完成对3d网格的四面体剖分
- 实现弹簧质点模型的欧拉隐式及加速方法



## 参考文献

[^Liu13]: Liu, Tiantian , et al. "Fast simulation of mass-spring systems." *Acm Transactions on Graphics* 32.6(2013):1-7.

