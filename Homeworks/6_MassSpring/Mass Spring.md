# Mass Spring

弹簧质点系统

## 模拟方法

由前 n 帧信息，求得第 n+1 帧信息（位移 x，速度 v）（设时间步长为 h）

##### 隐式欧拉法

$$
\boldsymbol x_{n+1}=\boldsymbol x_n+h\boldsymbol v_{n+1},\\
\boldsymbol v_{n+1}=\boldsymbol v_n+h\boldsymbol M^{-1}(\boldsymbol f_{int}(x_{n+1}) +\boldsymbol f_{ext})
$$

上式方程可以转换成
$$
\boldsymbol x_{n+1}=\boldsymbol x_n+h\boldsymbol v_n+h^2M^{-1}[\boldsymbol f_{int}(\boldsymbol x_{n+1})+\boldsymbol f_{ext}]\\
\boldsymbol v_{n+1}=(\boldsymbol x_{n+1}-\boldsymbol x_{n})/h\tag1
$$
为了解出迭代公式中非线性方程的解，有下面两类方法

#### 1 牛顿迭代法

记
$$
\boldsymbol y =\boldsymbol x_n + h\boldsymbol v_n + h^2\boldsymbol M^{-1}\boldsymbol f_{ext}
$$
其中 $\boldsymbol x_n,\boldsymbol v_n, \boldsymbol f_{ext}$为上一步迭代的常量

​	则（1）式转换成求解关于 $\boldsymbol x$的方程
$$
\boldsymbol g(\boldsymbol x) = \boldsymbol M(\boldsymbol x-\boldsymbol y) -h^2\boldsymbol f_{int}(\boldsymbol x) = 0\tag2
$$
方程的根即为 $\boldsymbol x_{n+1}$，那么牛顿法求非线性方程根的格式为：
$$
\boldsymbol x^{(k+1)}=\boldsymbol x^{(k)}-(\nabla \boldsymbol g(\boldsymbol x^{(k)}))^{-1}\boldsymbol g(\boldsymbol x^{(k)})\tag3
$$
其中迭代的初值为 $\boldsymbol x^{(0)}=\boldsymbol y$

> 要注意误差范数的度量方式和步长的选择，如果选择不恰当，可能造成无法收敛

##### 对弹簧内力求导

​	上式中涉及到对内力 $\boldsymbol f_{int}(\boldsymbol x)$的求导，对某单个弹簧，设其端点为$\boldsymbol  x_1$，$\boldsymbol  x_2$，劲度系数为$k$，原长为$l$，有：
$$
\boldsymbol x_1所受弹力：     \boldsymbol f_1(\boldsymbol x_1,\boldsymbol x_2)=k(||\boldsymbol x_1-\boldsymbol x_2||-l)\frac{-(\boldsymbol x_1-\boldsymbol x_2)}{||\boldsymbol x_1-\boldsymbol x_2||},\\
\boldsymbol x_2所受弹力：     \boldsymbol f_2(\boldsymbol x_1,\boldsymbol x_2)=-\boldsymbol f_1(\boldsymbol x_1,\boldsymbol x_2),
$$
​	令 $\boldsymbol r=\boldsymbol x_1-\boldsymbol x_2$，可得：
$$
\boldsymbol f_1(\boldsymbol x_1,\boldsymbol x_2)=\boldsymbol f_1(\boldsymbol r)=k(||\boldsymbol r||-l)\frac{-\boldsymbol r}{||\boldsymbol r||}\\

\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1}=\frac{\partial  \boldsymbol f_1}{\partial \bold r}\frac{\part \bold r}{\part \boldsymbol x_1}=k(\frac{l}{||\boldsymbol r||}-1)\boldsymbol I-kl||\boldsymbol r||^{-3}\boldsymbol r \boldsymbol r^T\\

\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_2}=-\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1},\\

\frac{\partial  \boldsymbol f_2}{\partial \boldsymbol x_1}=-\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1},\\

\frac{\partial  \boldsymbol f_2}{\partial \boldsymbol x_2}=\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1},
$$
对所有弹簧求导并组装即可求得力的导数（组装为稀疏矩阵，矩阵为对称阵）

##### 梯度

​	所以对一个质点  $\boldsymbol x_i$，$\boldsymbol g(\boldsymbol x_i)$的梯度为：
$$
\nabla \boldsymbol g(\boldsymbol x_i)=M\bold I-h^2\sum_j\left[ k(\frac{l}{||\boldsymbol r_{ij}||}-1)\boldsymbol I-kl||\boldsymbol r_{ij}||^{-3}\boldsymbol r_{ij} \boldsymbol r_{ij}^T\right]
$$


#### 2 加速方法

​	解非线性方程式（2）式的根 $\bold x_{n+1}$ 等价为一个最小化问题
$$
\boldsymbol x_{n+1}=\min\limits_{x}\frac{1}{2}(\boldsymbol x-\boldsymbol y)^T\boldsymbol M(\boldsymbol x-\boldsymbol y)+h^2E(\boldsymbol x)
$$
其中 $E(\boldsymbol x)$为弹簧内力 $\boldsymbol f_{int}(x)$的势能项，满足：$\boldsymbol f_{int}(x)=-\nabla E(\boldsymbol x)$

​	弹簧势能可写为：
$$
E(\bold x_1,\bold x_2)=\frac{1}{2}k(||\boldsymbol x_1-\boldsymbol x_2||-l)^2=\frac{1}{2}k \min\limits_{||\boldsymbol d||=l}||\boldsymbol x_1-\boldsymbol x_2-\boldsymbol d||^2\\
当且仅当\ \bold d=l\frac{\bold x_1-\bold x_2}{||\bold x_1-\bold x_2||}时取极值
$$
​	从而把问题化简成：
$$
\boldsymbol x_{n+1}=\min\limits_{x,\boldsymbol d\in\boldsymbol U}\frac{1}{2}\boldsymbol x^T(\boldsymbol M+h^2\boldsymbol L)\boldsymbol x-h^2\boldsymbol x^T\boldsymbol J \boldsymbol d-\boldsymbol x^T \boldsymbol M \boldsymbol y
$$
​	其中，m为质点个数，s 为弹簧个数,有
$$
\boldsymbol x,\boldsymbol y\in \R^{3m}, \quad
\boldsymbol d\in \R^{3s};\\
\bold L=\left(\sum_{i=1}^sk_i\bold A_i \bold A_i^T\right)\otimes \bold I_3 \in\R^{3m\times3m}\\
\bold J=\left(\sum_{i=1}^sk_i\bold A_i \bold S_i^T\right)\otimes \bold I_3 \in\R^{3m\times3s}
$$
​	$\bold A_i\in \R^m$表示第 $i$ 个弹簧中两个相连质点位置向量，比如第 $i$ 个弹簧由 $i_1,i_2$两质点相连，则 $\bold A_i(i_1)=1,\bold A_i(i_2)=-1$，其余位置为 0。

​	$S_i\in\R^s$表示  第 $i$ 个弹簧的位置向量，$S_i(j)=\delta_{ij}$；如果抛开直积的部分，$\sum_{i=1}^sk_i\bold A_i \bold S_i^T$意味着一个 ${m\times s}$ 的矩阵，即 $(k_1 \bold A_1,k_2 \bold A_2,...,k_s\bold A_s)\in \R^{m\times s}$。

​	从而可以对 $\boldsymbol x$，$\boldsymbol d$ 迭代优化求得该优化问题的解：
$$
\boldsymbol x 优化：求解方程(\boldsymbol M+h^2\boldsymbol L)\boldsymbol x=h^2\boldsymbol J \boldsymbol d+ \boldsymbol M \boldsymbol y（这里可以预分解矩阵），\\
$$

$$
\boldsymbol d 优化：\boldsymbol d_i=l_i\frac{\boldsymbol x^{(k)}_{i_1}-\boldsymbol x^{(k)}_{i_2}}{||\boldsymbol x^{(k)}_{i_1}-\boldsymbol x^{(k)}_{i_2}||}（这里l_i为第i个弹簧原长，\boldsymbol x^{(k)}_{i_1}，\boldsymbol x^{(k)}_{i_2}为其两端点），
$$

重复迭代过程直到收敛。

#### 3 边界条件和约束

通常模拟过程中物体会有各种约束或额外条件，例如物体被固定了几个点，对某些点施W加外力（如重力、浮力、风力等）。

### 外力条件

- 物体受到的外力可以直接加在模拟的外力项中，其导数为 0
- 对于重力，可以将其加在外力中，另一方面，重力为保守力，也可以将重力势能加在能量项中与弹性势能进行合并

### 位移约束

这里主要考虑固定部分质点的情形，有两种方法处理：

- 第一种方法是在每一帧中求出该点的内力，再施加与该内力大小相同，方向相反的外力，但与上一种情形不同的是，若该内力对位移导数不为 0，则该外力对位移导数也不为 0，需要将其导数考虑进去；

- 第二种方法为仅考虑真正的自由坐标，降低问题的维数，具体如下：

将所有n个质点的坐标列为列向量 $x\in R^{3n}$，将所有 m 个自由质点坐标（无约束坐标）列为列向量 $x_f\in R^{3m}$,则两者关系：
$$
\boldsymbol x_f=\boldsymbol K\boldsymbol x,\\  \boldsymbol x=\boldsymbol K^T\boldsymbol x_f+\boldsymbol b,
$$
其中 $K\in R^{3m\times 3n}$ 为单位阵删去约束坐标序号对应行所得的稀疏矩阵，$b$ 为与约束位移有关的向量，计算为 $b=x-K^TKx$, 若约束为固定质点则 $b$ 为常量。由此我们将原本的关于 $x$ 的优化问题转化为对 $x_f$ 的优化问题：欧拉隐式方法中求解方程为：
$$
\boldsymbol g_1(\boldsymbol x_f) = K(\boldsymbol M(\boldsymbol x-\boldsymbol y) -h^2\boldsymbol f_{int}(\boldsymbol x)) = 0,\\
梯度：\nabla_{x_f} \boldsymbol g_1(\boldsymbol x_f) = K\nabla_{x} \boldsymbol g(\boldsymbol x)K^T,\\
$$
加速方法中优化问题中 $x$ 迭代步骤转化为求解关于 $x_f$ 的方程：
$$
K(\boldsymbol M+h^2\boldsymbol L)K^T\boldsymbol x_f=K(h^2\boldsymbol J \boldsymbol d+ \boldsymbol M \boldsymbol y-(\boldsymbol M+h^2\boldsymbol L)\boldsymbol b)
$$