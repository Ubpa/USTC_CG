# Inverse distance-weighted interpolation methods[^IDW] 

> 王润泽

## 问题描述

给定 $n$ 对控制点 $(\mathbf{p} _ i,\mathbf{q _ i})$，$\mathbf{p} _ i,\mathbf{q} _ i\in\mathbb{R}^2$，$i=1,\dots,n$ 

得到一个函数 $\mathbf{f}:\mathbb{R}^2\to\mathbb{R}^2$，满足插值条件，即 $\mathbf{f}(\mathbf{p} _ i)=\mathbf{q} _ i,i=1,\dots,n$ 

## 算法原理

局部插值函数 $\mathbf{f} _ i(\mathbf{p}):\mathbb{R}^2\to\mathbb{R}^2$ 满足 $f _ i(\mathbf{p} _ i)=\mathbf{q} _ i$，具体为

$$
\mathbf{f} _ i(\mathbf{p})=\mathbf{q} _ i+\mathbf{D} _ i(\mathbf{p}-\mathbf{p} _ i)
$$
其中 $\mathbf{D} _ i:\mathbb{R}^2\to\mathbb{R}^2$，满足 $\mathbf{D} _ i(\mathbf{0})=\mathbf{0}$ 

可选 $\mathbf{D} _ i$ 为线性变换

对平面中任意点 $\bold x$ ，其插值函数为

$$
\mathbf{f}(\mathbf{x})=\sum _ {i=1}^n w _ i(\mathbf{x})\mathbf{f} _ i(\mathbf{x})
$$

#### 权函数

其中权函数 $w _ i:\mathbb{R}^2\to\mathbb{R}$，为
$$
w _ i(\mathbf{x})=\frac{\sigma _ i(\mathbf{x})}{\sum _ {j=1}^n \sigma _ j(\mathbf{x})}
$$

$$
\sigma _ i(\mathbf{x})=\frac{1}{\Vert\mathbf{x}-\mathbf{x} _ i\Vert^\mu}
$$

其中 $\mu>1$ ，常取2；显然 $0\le w _ i(\pmb{x})\le 1$，且 $\sum _ {i=1}^n w _ i(\mathbf{x})=1$ 



#### 基函数

1.可直接取 $\mathbf{D} _ i=\mathbf{0}$，基函数 $\bold f_i(\bold p)=q_i$，那么插值函数为
$$
\mathbf{f}(\mathbf{x})=\sum _ {i=1}^n w _ i(\mathbf{x})\mathbf{q} _ i
$$
2.定义基函数 $\bold f_i(\bold p)$ 为：$\mathbf{f} _ i(\mathbf{p})=\mathbf{q} _ i+\mathbf{D} _ i(\mathbf{p}-\mathbf{p} _ i)$

为了求得变换矩阵 $\bold D_i$ 定义误差函数

$$
\begin{aligned}
E_i(f)
=&\sum _ {j=1,j\neq i}^n \sigma_i(\bold p_j)\left\Vert\bold f(p_j)-\bold q_j)\right\Vert^2\\
E _ i(f)
=&\sum _ {j=1,j\neq i}^n \sigma_i(\bold p_j)\left\Vert\mathbf{q} _ i+\left(\begin{array}{c}d _ {i,11} & d _ {i,12}\newline d _ {i,21} & d _ {i,22}\end{array}\right)(\mathbf{p} _ j-\mathbf{p} _ i)-\mathbf{q} _ j\right\Vert^2\newline
=&\sum _ {j=1,j\neq i}^n \sigma_i(\bold p_j)\left\Vert\left(\begin{array}{c}d _ {i,11} & d _ {i,12}\newline d _ {i,21} & d _ {i,22}\end{array}\right)\Delta\mathbf{p}-\Delta\mathbf{q}\right\Vert^2\newline
=&\sum _ {j=1,j\neq i}^n \sigma_i(\bold p_j)\{
[d _ {i,11}(p _ {j,1}-p _ {i,1})+d _ {i,12}(p _ {j,2}-p _ {i,2})+q _ {i,1}-q _ {j,1}]^2+\newline
&\qquad\qquad\qquad[d _ {i,21}(p _ {j,1}-p _ {i,1})+d _ {i,22}(p _ {j,2}-p _ {i,2})+q _ {i,2}-q _ {j,2}]^2\}
\end{aligned}
$$

最小化该误差函数，可求得变换矩阵 $\mathbf{D} _ i=\left(\begin{array}{c}d _ {i,11} & d _ {i,12}\newline d _ {i,21} & d _ {i,22}\end{array}\right)$ 
$$
\bold D_i^*=\arg\min_{\bold D_i}{E_i(\bold D_i)}
$$
对 $D$ 的每个元素求偏导，得到
$$
\part E/\part d_{11}=\sum _ {j=1,j\neq i}^n2 \sigma_i(\bold p_j)*(\Delta p_1)*
[\bold D_{1,\cdot}\Delta \bold p-\Delta q_1]=0\\
\part E/\part d_{12}=\sum _ {j=1,j\neq i}^n2 \sigma_i(\bold p_j)*(\Delta p_2)*
[\bold D_{1,\cdot}\Delta \bold p-\Delta q_1]=0\\
\part E/\part d_{21}=\sum _ {j=1,j\neq i}^n2 \sigma_i(\bold p_j)*(\Delta p_1)*
[\bold D_{2,\cdot}\Delta \bold p-\Delta q_2]=0\\
\part E/\part d_{22}=\sum _ {j=1,j\neq i}^n2 \sigma_i(\bold p_j)*(\Delta p_2)*
[\bold D_{2,\cdot}\Delta \bold p-\Delta q_2]=0
$$
整理一下上式，可以得到
$$
\sum _ {j=1,j\neq i}^n \sigma_i(\bold p_j)(\bold D\Delta \bold p-\Delta \bold q)\Delta \bold p^T=0\\
\bold D^* =\left(\sum _ {j=1,j\neq i}^n \sigma_i(\bold p_j)\Delta \bold q\Delta \bold p^T\right)\left(\sum _ {j=1,j\neq i}^n \sigma_i(\bold p_j)\Delta \bold p\Delta \bold p^T\right)^{-1}
$$
那么插值函数为
$$
\mathbf{f}(\mathbf{x})=\sum _ {i=1}^n w _ i(\mathbf{x})[\mathbf{q} _ i+\mathbf{D}^* _i(\mathbf{x}-\mathbf{q} _ i)]
$$


## 参考文献

[^IDW]: Ruprecht D, Muller H. [**Image warping with scattered data interpolation**](https://www.semanticscholar.org/paper/Image-Warping-with-Scattered-Data-Interpolation-Ruprecht-M%C3%BCller/5a9e2604064d08f2a8b7dcef4cd4e9a2ce2a88c2?p2df)[J]. IEEE Computer Graphics and Applications, 1995, 15(2): 37-43.

