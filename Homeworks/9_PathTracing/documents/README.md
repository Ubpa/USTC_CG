# 作业说明

## 1. 基础知识

### 1.1 渲染方程

$$
L_o(\pmb{p},\pmb{\omega}_o)=L_e(\pmb{p},\pmb{\pmb{\omega}_o})+\int_{\mathcal{H}^2(\pmb{n}(\pmb{p}))} f_r(\pmb{p},\pmb{\omega}_i,\pmb{\omega}_o)L_i(\pmb{p},\pmb{\omega}_i)\cos\theta_{\pmb{\omega}_i,\pmb{n}(\pmb{p})}\mathbb{d}\pmb{\omega}_i
$$

> 其中
>
> - $L_o$ 是出射 radiance
> - $\pmb{p}$ 是渲染点
> - $\pmb{\omega}_i$ 是入射光方向
> - $\pmb{\omega}_o$ 是出射光方向
> - $L_e$ 是发光 radiance
> - $\pmb{n}(\pmb{p})$ 
> - ${\mathcal{H}^2(\pmb{n}(\pmb{p}))} $ 是法向 $\pmb{n}(\pmb{p})$ 所在半球
> - $f_r$ 是双向散射分布函数（bidirectional scattering distribution function ，BRDF）
> - $L_i$ 是入射 radiance
> - $\theta_{\pmb{\omega}_i,\pmb{n}(\pmb{p})}$ 是 $\pmb{\omega}_i$ 与 $\pmb{n}(\pmb{p})$ 的夹角
>

记

$$
\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L=\int_{\mathcal{H}^2(\pmb{n}(\pmb{p}))} f_r(\pmb{p},\pmb{\omega}_i,\pmb{\omega}_o)L\cos\theta_{\pmb{\omega}_i,\pmb{n}(\pmb{p})}\mathbb{d}\pmb{\omega}_i
$$

则

$$
L_o(\pmb{p},\pmb{\omega}_o)=L_e(\pmb{p},\pmb{\pmb{\omega}_o})+\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_i(\pmb{p},\pmb{\omega}_i)
$$

反射方程为

$$
L_r(\pmb{p},\pmb{\omega}_o)=\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_i(\pmb{p},\pmb{\omega}_i)
$$

对于 $L_i$ 有关系式

$$
L_i(\pmb{p},\pmb{\omega}_i)=L_o(\mathop{raytrace}(\pmb{p},\pmb{\omega_i}),-\pmb{\omega_i})
$$

> 其中 $\mathop{raytrace}$ 是射线与场景的相交函数

记 $\mathop{raytrace}(\pmb{p},\pmb{\omega}_i)$ 为 $\pmb{p}^\prime$，则有

$$
L_i(\pmb{p},\pmb{\omega}_i)=L_o(\pmb{p}^\prime,-\pmb{\omega_i})
$$

如此形成递归

$$
L_o(\pmb{p},\pmb{\omega}_o)=L_e(\pmb{p},\pmb{\pmb{\omega}_o})+\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_o(\pmb{p}^\prime,-\pmb{\omega_i})
$$

更有效地是对 $L_r$ 进行递归

将 $L_r$ 展开一次

$$
\begin{aligned}
L_r(\pmb{p},\pmb{\omega}_o)
&=\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}\left(L_e(\pmb{p}^\prime,-\pmb{\omega}_i)+L_r(\pmb{p}^\prime,-\pmb{\omega}_i)\right)\\
&=\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_e(\pmb{p}^\prime,-\pmb{\omega}_i)
+\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_r(\pmb{p}^\prime,-\pmb{\omega}_i)
\end{aligned}
$$

将

- $\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_e(\pmb{p}^\prime,-\pmb{\omega}_i)$ 称为**直接光**，记作 $L_{\text{dir}}(\pmb{p},\pmb{\omega}_o)$ 
- $\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_r(\pmb{p}^\prime,-\pmb{\omega}_i)$ 称为**间接光**，记作 $L_{\text{indir}}(\pmb{p},\pmb{\omega}_o)$ 

### 1.2 直接光

$$
L_{\text{dir}}(\pmb{p},\pmb{\omega}_o)=\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_e(\pmb{p}^\prime,-\pmb{\omega}_i)
$$

积分中，对于大部分方向 $\pmb{\omega}_i$，$L_e(\pmb{p}^\prime,-\pmb{\omega}_i)=0$（非光源），所以我们直接在光源所在方向上积分

其中 $\pmb{p}$， $\pmb{\omega}_o$ 和 $\pmb{\omega}_i$ 可用三点确定，如下图所示

![xyz.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/09_PathTracing/xyz.jpg)

> 图中 $\pmb{x}$ 即为 $\pmb{p}$，$\pmb{y}$ 即为 $\pmb{p}^\prime$ 

由几何关系可知

$$
\mathbb{d}\pmb{\omega}_i=\frac{|\cos\theta_{\pmb{y},\pmb{x}}|}{\|\pmb{x}-\pmb{y}\|^2}\mathbb{d}A(\pmb{y})
$$

其中 $\theta_{\pmb{y},\pmb{x}}$ 是方向 $\pmb{x}-\pmb{y}$ 与 $\pmb{n}(\pmb{y})$ 的夹角

引入几何传输项（两点间的“传输效率”）

$$
G(\pmb{x}\leftrightarrow\pmb{y})=V(\pmb{x}\leftrightarrow\pmb{y})\frac{|\cos\theta_{\pmb{x},\pmb{y}}||\cos\theta_{\pmb{y},\pmb{x}}|}{\|\pmb{x}-\pmb{y}\|^2}
$$

>其中 $V(\pmb{x}\leftrightarrow\pmb{y})$ 是可见性函数，当 $\pmb{x}$ 和 $\pmb{y}$ 之间无阻隔时为 $1$，否则为 $0$ 
>
>$G$ 是对称函数，即 $G(\pmb{x}\leftrightarrow\pmb{y})=G(\pmb{y}\leftrightarrow\pmb{x})$ 

故有

$$
L_{\text{dir}}(\pmb{x}\to\pmb{z})=\int_A f_r(\pmb{y}\to \pmb{x}\to\pmb{z})L_e(\pmb{y}\to\pmb{x})G(\pmb{x}\leftrightarrow\pmb{y})\mathbb{d}A(\pmb{y})
$$

其中积分域 $A$ 为场景中所有的面积，但只有光源处 $L_e(\pmb{y}\to\pmb{x})\neq 0$ 

记光源数 $N_e$，场景中的光源集为 $\{L_{e_i}\}_{i=1}^{N_e}$ ，对应的区域集为 $\{A(L_{e_i})\}_{i=1}^{N_e}$，则可写为

$$
L_{\text{dir}}(\pmb{x}\to\pmb{z})=\sum_{i=1}^{N_e}\int_{A(L_{e_i})} f_r(\pmb{y}\to\pmb{x}\to\pmb{z})L_e(\pmb{y}\to\pmb{x})G(\pmb{x}\to\pmb{y})\mathbb{d}A(\pmb{y})
$$

### 1.3 间接光

递归即可

$$
L_r(\pmb{p},\pmb{\omega}_o)=\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_e(\pmb{p}^\prime,-\pmb{\omega}_i)
+\int_{\pmb{p},\pmb{\omega}_o,\pmb{\omega}_i}L_r(\pmb{p}^\prime,-\pmb{\omega}_i)
$$

### 1.4 蒙特卡洛积分与重要性采样

![mc.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/09_PathTracing/mc.jpg)

![is.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/09_PathTracing/is.jpg)

### 1.5 渲染方程的计算

我们要计算的是如下积分

$$
L_r(\pmb{p},\pmb{\omega}_o)=L_{\text{dir}}+L_{\text{indir}}
$$

右侧积分式需要递归

利用蒙特卡洛积分可将积分转成采样
$$
\begin{aligned}
L_{\text{dir}}(\pmb{x}\to\pmb{z})
&\approx\sum_{i=1}^{N_e}\sum_{j=1}^{N_i}\frac{f_r(\pmb{y}_i^{(j)}\to\pmb{x}\to\pmb{z})L_e(\pmb{y}_i^{(j)}\to\pmb{x})G(\pmb{x}\to\pmb{y}_i^{(j)})}{p(\pmb{y}_i^{(j)})}\\
L_{\text{indir}}(\pmb{p},\pmb{\omega}_o)
&\approx\sum_{k=1}^{N}\frac{f_r(\pmb{p},\pmb{\omega}_i^{(k)},\pmb{\omega}_o)L_r(\pmb{p}^{\prime(k)},-\pmb{\omega})\cos\theta_{\pmb{\omega}_i,\pmb{n}(\pmb{p})}}{p(\pmb{\omega}_i^{(k)})}
\end{aligned}
$$
$L_{\text{dir}}$ 在各光源区域采样

对于 $L_{\text{indir}}$ 则半球采样

采样个数皆为 1（$N_i=1\quad(i=1,\dots,N_e)$，$N=1$） 

> 也可采用其他采样策略

## 2. 作业要求

### 2.1 路径追踪算法

框架提供了实现路径追踪算法的绝大部分算法（包括场景加速结构，相交，采样等），同学只需实现路径追踪算法核心部分（递归步骤），并渲染场景。

在实际编写路径追踪的代码时，有的材质是绝对光滑的，如镜面。这时直接光的计算退化（光线只沿一个方向出射，即 BRDF 函数中含 delta 函数），只需要知道出射方向处是否与光源相交即可，这正是间接光所要做的。故此时将该计算任务交给计算间接光的步骤里（通过变量 `bool last_bounce_specular` 来标识）。

代码中的函数 `PathTracer::Shade` 即为上文中的 $L_r$ 

### 2.2 搭建场景并渲染

TODO

### （可选）2.3 环境光贴图重要性采样

![image-20200425170048209](assets/README/image-20200425170048209.png)

参考：[wiki | Alias_method](https://en.wikipedia.org/wiki/Alias_method) 

优点是采样时间复杂度为 $O(1)$ 

生成别名表后，可采用离散的像素，相关概率关系如下
$$
\begin{aligned}
\int_{I}p_{\text{img}}(i,j)\mathbb{d}i\mathbb{d}j
&=\int_{\Theta\Phi}p_{\text{texture}}(\theta,\phi)\left|\frac{\part(i,j)}{\part(\theta,\phi)}\right|\mathbb{d}\theta\mathbb{d}\phi\\
&=\int_{\Omega}p(A)\left|J_{\theta\phi}A\right|\left|\frac{\part(i,j)}{\part(\theta,\phi)}\right|\mathbb{d}A\\
&=\int_{\Omega}p(\omega_i)\left|\frac{\mathrm{d}\omega_i}{\mathrm{d}A}\right|\left|J_{\theta\phi}A\right|\left|\frac{\part(i,j)}{\part(\theta,\phi)}\right|\mathbb{d}\omega_i\\
\end{aligned}
$$
其中

![xyz.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/09_PathTracing/xyz.jpg)
$$
\left|\frac{\mathrm{d}\omega_i}{\mathrm{d}A}\right|=\frac{\cos\theta_o}{\|\pmb{x}-\pmb{y}\|^2}=1\\
\left|J_{\theta\phi}A\right|=\sin\theta\\
\left|\frac{\part(i,j)}{\part(\theta,\phi)}\right|=\frac{wh}{2\pi^2}
$$
则
$$
p(\omega_i)=\frac{2\pi^2}{wh\sin\theta}p_{\text{img}}(i,j)
$$
