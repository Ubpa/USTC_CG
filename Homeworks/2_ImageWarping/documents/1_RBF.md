# Radial basis functions interpolation method [^RBF] 

给定 $n$ 对控制点 $(\mathbf{p} _ i,\mathbf{q _ i})$，$\mathbf{p} _ i,\mathbf{q} _ i\in\mathbb{R}^2$，$i=1,\dots,n$ 

插值函数

$$
\pmb{f}(\pmb{p})=\sum _ {i=1}^n \boldsymbol{\alpha} _ i R(\Vert\mathbf{p}-\mathbf{p} _ i\Vert)+A\mathbf{p}+\mathbf{b}
$$

其中权重系数 $\boldsymbol{\alpha} _ i\in\mathbb{R}^2$，$A\in\mathbb{R}^{2\times 2}$，$\mathbf{b}\in\mathbb{R}^2$，径向基函数 $R(d)=(d^2+r^2)^{\mu/2}$ 

要求满足插值条件

$$
\mathbf{f}(\mathbf{p} _ j)=\sum _ {i=1}^n\boldsymbol{\alpha} _ i R(\Vert\mathbf{p} _ j-\mathbf{p} _ i\Vert)+A\mathbf{p} _ j+\mathbf{b}=\mathbf{q} _ j,\quad j=1,\dots,n
$$

自由度每维有 $n+3$ 个

可选的补充约束为

$$
\left[\begin{array}{c}
\mathbf{p} _ 1 & \dots & \mathbf{p} _ n\newline
1            & \dots & 1
\end{array}\right] _ {3\times n}
\left[\begin{array}{c}
\boldsymbol{\alpha} _ 1 \newline
\vdots \newline
\boldsymbol{\alpha} _ n
\end{array}\right] _ {n\times2}
=\mathbf{0} _ {3\times 2}
$$

也可根据论文，通过额外的仿射集中的控制点 $(\mathbf{x} _ i,\mathbf{y} _ i)$ 来确定 $A$ 和 $\mathbf{b}$ 

- 没有点时，恒等变换（$A=I,\mathbf{b}=\mathbf{0}$）
- 一个点时，平移变换（$A=I$，$\mathbf{b}=\mathbf{y _ i}-\mathbf{x _ i}$）
- 两个点时，平移+缩放
- 三个点时，一般仿射变换
- 多个点时，用最小二乘法求仿射变换

## 参考文献

[^RBF]: Arad N, Reisfeld D. [**Image warping using few anchor points and radial functions**](https://pdfs.semanticscholar.org/5a9e/2604064d08f2a8b7dcef4cd4e9a2ce2a88c2.pdf )[C]//Computer graphics forum. Edinburgh, UK: Blackwell Science Ltd, 1995, 14(1): 35-46.

