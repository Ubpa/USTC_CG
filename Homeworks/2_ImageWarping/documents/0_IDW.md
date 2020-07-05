# Inverse distance-weighted interpolation methods[^IDW] 

## 问题描述

给定 $n$ 对控制点 $(\mathbf{p} _ i,\mathbf{q _ i})$，$\mathbf{p} _ i,\mathbf{q} _ i\in\mathbb{R}^2$，$i=1,\dots,n$ 

得到一个函数 $\mathbf{f}:\mathbb{R}^2\to\mathbb{R}^2$，满足插值条件，即 $\mathbf{f}(\mathbf{p} _ i)=\mathbf{q} _ i,i=1,\dots,n$ 

## 算法原理

局部插值函数 $\mathbf{f} _ i(\mathbf{p}):\mathbb{R}^2\to\mathbb{R}^2$ 满足 $f _ i(\mathbf{p} _ i)=\mathbf{q} _ i$，具体为

$$
\mathbf{f} _ i(\mathbf{p})=\mathbf{q} _ i+\mathbf{D} _ i(\mathbf{p}-\mathbf{q} _ i)
$$
其中 $\mathbf{D} _ i:\mathbb{R}^2\to\mathbb{R}^2$，满足 $\mathbf{D} _ i(\mathbf{0})=\mathbf{0}$ 

可选 $\mathbf{D} _ i$ 为线性变换

插值函数为

$$
\mathbf{f}(\mathbf{x})=\sum _ {i=1}^n w _ i(\mathbf{x})\mathbf{f} _ i(\mathbf{x})
$$

其中 $w _ i:\mathbb{R}^2\to\mathbb{R}$，为

$$
w _ i(\mathbf{x})=\frac{\sigma _ i(\mathbf{x})}{\sum _ {j=1}^n \sigma _ j(\mathbf{p})}
$$

$$
\sigma _ i(\mathbf{x})=\frac{1}{\Vert\mathbf{x}-\mathbf{x} _ i\Vert^\mu}
$$

其中 $\mu>1$ 

显然 $0\le w _ i(\pmb{x})\le 1$，且 $\sum _ {i=1}^n w _ i(\mathbf{x})=1$ 

简单地，可直接取 $\mathbf{D} _ i=\mathbf{0}$，此时 $\mathbf{f}(\mathbf{x})=\sum _ {i=1}^n w _ i(\mathbf{x})\mathbf{q} _ i$ 

定义能量

$$
\begin{aligned}
E _ i(\mathbf{D} _ i)
=&\sum _ {j=1,j\neq i}^n w _ {ij}\left\Vert\mathbf{q} _ i+\left(\begin{array}{c}d _ {i,11} & d _ {i,12}\newline d _ {i,21} & d _ {i,22}\end{array}\right)(\mathbf{p} _ j-\mathbf{p} _ i)-\mathbf{q} _ j\right\Vert^2\newline
=&\sum _ {j=1,j\neq i}^n w _ {ij}(
(d _ {i,11}(p _ {j,1}-p _ {i,1})+d _ {i,12}(p _ {j,2}-p _ {i,2})+q _ {i,1}-q _ {j,1})^2+\newline
&(d _ {i,21}(p _ {j,1}-p _ {i,1})+d _ {i,22}(p _ {j,2}-p _ {i,2})+q _ {i,2}-q _ {j,2})^2)
\end{aligned}
$$

最小化该能量可求得 $\mathbf{D} _ i=\left(\begin{array}{c}d _ {i,11} & d _ {i,12}\newline d _ {i,21} & d _ {i,22}\end{array}\right)$ 

## 参考文献

[^IDW]: Ruprecht D, Muller H. [**Image warping with scattered data interpolation**](https://pdfs.semanticscholar.org/5a9e/2604064d08f2a8b7dcef4cd4e9a2ce2a88c2.pdf)[J]. IEEE Computer Graphics and Applications, 1995, 15(2): 37-43.

