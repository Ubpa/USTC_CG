# Report

利用蒙特卡洛方法实现渲染方程

# 1. 摘要

本文主要介绍了利用蒙特卡洛方法求解渲染方程。将渲染方程分解成直接光与间接光；对直接光进行蒙特卡洛重要性采样，采用Alias算法加速采样时间；对间接光递归求解，并讨论了对每个像素不同采样次数（SPP）的结果。

# 2. 原理

## 2.1 渲染方程

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\render_eq.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 1:Rendering
    </div>
    <p> </p>
</center>

$$
L _ {o}(\pmb{p},\pmb{\omega} _ {o})=L _ {e}(\pmb{p},\pmb{\omega} _ {o})+\int _ {\mathcal{H}^2(\pmb{n}(\pmb{p}))} f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})L _ {i}(\pmb{p},\pmb{\omega} _ {i})\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}\mathbb{d}\pmb{\omega} _ {i}\tag{1}
$$

其中

> - $L _ {o}$ 是出射 radiance
> - $\pmb{p}$ 是渲染点
> - $\pmb{\omega} _ {i}$ 是入射光方向
> - $\pmb{\omega} _ {o}$ 是出射光方向
> - $L _ {e}$ 是自发光 radiance
> - $\pmb{n}(\pmb{p})$ 为 $\pmb{p}$ 处法向
> - ${\mathcal{H}^2(\pmb{n}(\pmb{p}))} $ 是法向 $\pmb{n}(\pmb{p})$ 所在半球
> - $f _ {r}$ 是双向散射分布函数（bidirectional scattering distribution function ，BRDF）
> - $L _ {i}$ 是入射 radiance
> - $\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}$ 是 $\pmb{\omega} _ {i}$ 与 $\pmb{n}(\pmb{p})$ 的夹角
>

记反射方程为

$$
L _ {r}(\pmb{p},\pmb{\omega} _ {o})=\int _ {\mathcal{H}^2(\pmb{n}(\pmb{p}))} f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})L_i(\pmb{p},\pmb{\omega} _ {i})\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}\mathbb{d}\pmb{\omega} _ {i}\tag{2}
$$

则可以将渲染方程改写为：

$$
L _ {o}(\pmb{p},\pmb{\omega} _ {o})=L _ {e}(\pmb{p},\pmb{\omega} _ {o})+L _ {r}(\pmb{p},\pmb{\omega} _ {o})
$$

由于 $L _ {e}(\pmb{p},\pmb{\omega} _ {o})$ 是物体自发光强度，可以作为已知量，我们假设除了光源外，其他物体不会自发光，所以只要重点关注反射光强 $L _ {r}(\pmb{p},\pmb{\omega} _ {o})$ 即可，即设
$$
L _ {o}(\pmb{p},\pmb{\omega} _ {o})=L _ {r}(\pmb{p},\pmb{\omega} _ {o})\tag{3}
$$
在（2）式中，反射光 $L _ {r}(\pmb{p},\pmb{\omega} _ {o})$ 实际上是来自两部分：

- 来自光源的直接入射，称为**直接光**，记作 $L _ {\text{dir}}(\pmb{p},\pmb{\omega} _ {o})$；
- 来自其他物体反射的间接光，称作**间接光**，记作 $L _ {\text{indir}}(\pmb{p},\pmb{\omega} _ {o})$ 

$$
L_o=L_r(\pmb{p},\pmb{\omega} _ {o})=L _ {\text{dir}}(\pmb{p},\pmb{\omega} _ {o})+L _ {\text{indir}}(\pmb{p},\pmb{\omega} _ {o})\tag{4}
$$

上式中 $-\pmb{\omega} _ {i}$ 是 $L_{\text{dir}}$ 和 $L_{\text{indir}}$ 的出射方向
## 2.2 直接光

### 2.2.1 光源光照方程


$$
L _ {\text{dir}}(\pmb{p},\pmb{\omega} _ {o})=\int _ {\mathcal{H}^2(\pmb{n}(\pmb{p}))}L _ {e}(\pmb{p},-\pmb{\omega} _ {i})f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})\cos\theta\mathbb{d}\pmb{\omega} _ {i} \tag5
$$

对于光源 $L_e$ 来说，出射光方向 $\pmb{\omega} _ {e,o}=-\pmb{\omega} _ {i}$

其中 $\pmb{p}$， $\pmb{\omega} _ {o}$ 和 $\pmb{\omega} _ {i}$ 可用三点确定，如下图所示

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\xyz.jpg" 
        width = "30%">
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\dwi_dA.jpg" 
        width = "33%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 2
    </div>
    <p> </p>
</center>

> 注：图中 $\pmb{x}$ 即为 $\pmb{p}$，$\pmb{y}$ 即为 $\pmb{p}^\prime$ 

如果我们只是在原光照点 $\pmb{p}$ 处积分，我们采样的是半球上立体角，这样会有大量的方向的光学被浪费。所以我们应该直接在光源所在区域上积分，即积分限：从半球变成了光源的表面积！
$$
L _ {\text{dir}}(\pmb{p},\pmb{\omega} _ {o})=L _ {\text{dir}}(\pmb{x}\to\pmb{z})=\int _ A f _ {r}(\pmb{y}\to \pmb{x}\to\pmb{z})L _ {e}(\pmb{y}\to\pmb{x})G(\pmb{x}\leftrightarrow\pmb{y})\mathbb{d}A(\pmb{y})\tag6
$$
其中积分域 $A$ 为场景中所有的面积，但只有光源处 $L _ {e}(\pmb{y}\to\pmb{x})\neq 0$ 

根据**Mento Carlo 积分法**，得到
$$
L _ {\text{dir}}(\pmb{x}\to\pmb{z})=\sum _ {A(i,j)}\frac{ f _ {r}(\pmb{y}\to \pmb{x}\to\pmb{z})L _ {e}(\pmb{y}\to\pmb{x})G(\pmb{x}\leftrightarrow\pmb{y})}{p(i,j)}\tag7
$$
其中 $p(i,j)$ 是对光源区域 $A$ 处的**均匀采样**。

由(5)、(6)式积分变换得：
$$
\mathbb{d}\pmb{\omega} _ {i}=\frac{|\cos\theta _ {\pmb{y},\pmb{x}}|}{\|\pmb{x}-\pmb{y}\|^2}\mathbb{d}A(\pmb{y})
$$

其中 $\theta _ {\pmb{y},\pmb{x}}$ 是方向 $\pmb{x}-\pmb{y}$ 与 $\pmb{n}(\pmb{y})$ 的夹角，引入几何传输项（两点间的“传输效率”）

$$
G(\pmb{x}\leftrightarrow\pmb{y})=V(\pmb{x}\leftrightarrow\pmb{y})\frac{|\cos\theta _ {\pmb{x},\pmb{y}}||\cos\theta _ {\pmb{y},\pmb{x}}|}{\|\pmb{x}-\pmb{y}\|^2}
$$

>其中 $V(\pmb{x}\leftrightarrow\pmb{y})$ 是可见性函数，当 $\pmb{x}$ 和 $\pmb{y}$ 之间无阻隔时为 $1$，否则为 $0$ 
>
>$G$ 是对称函数，即 $G(\pmb{x}\leftrightarrow\pmb{y})=G(\pmb{y}\leftrightarrow\pmb{x})$ 

如果记光源数 $N _ {e}$，场景中的光源集为 $\{L _ {e _ {i}}\} _ {i=1}^{N _ {e}}$ ，对应的区域集为 $\{A(L _ {e _ {i}})\} _ {i=1}^{N _ {e}}$，则可写为

$$
\begin{aligned}
L _ {\text{dir}}(\pmb{x}\to\pmb{z})&=\sum _ {k=1}^{N _ {e}}\int _ {A(L _ {e _ {k}})} f _ {r}(\pmb{y}\to\pmb{x}\to\pmb{z})L _ {e}(\pmb{y}\to\pmb{x})G(\pmb{x}\to\pmb{y})\mathbb{d}A(\pmb{y})\\
&=\sum _ {i=k}^{N _ {e}}\sum_ {A_k(i,j)} \frac{f _ {r}(\pmb{y}\to\pmb{x}\to\pmb{z})L _ {e}(\pmb{y}\to\pmb{x})G(\pmb{x}\to\pmb{y})}{p_k(i,j)}
\end{aligned}\tag8
$$

### 2.2.2  环境光贴图重要性采样

#### Monte Carlo 重要性采样

直接光源除了从我们给定的灯泡中发出，还可以来自周围的**环境光**。而当对环境光进行采样时，如果我们对环境光的所有区域进行平均采样，那么结果会所损失，应该进行改进的是对光强较大的区域重点采样，故应该采取**Monte Carlo重要采样**的方法，如下图所示

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\important_sample.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 3: MC Important Sampling
    </div>
    <p> </p>
</center>

所谓重要采样，即选择的抽样概率密度大致符合待抽样的分布，对于给定的环境光贴图，设采样为像素点 $\pmb{y}=(i,j)$,那么该点的概率为 $p _ {img}(i,j)$
$$
p_{img}(i,j)=\frac{L_e(i,j)}{\sum_{k,l}L_e(k,l)}\tag8
$$
相关概率关系如下
$$
\begin{aligned}
1=\int _ {I}p _ {\text{img}}(i,j)\mathbb{d}i\mathbb{d}j
&=\int _ {\Theta}p _ {\text{img}}(\theta,\phi)\left|\frac{\partial(i,j)}{\partial(\theta,\phi)}\right|\mathbb{d}\theta\mathbb{d}\phi\newline 
&=\int _ {A}p _ {\text{img}}(A)\left|\det J _ A\Theta\right|\left|\frac{\partial(i,j)}{\partial(\theta,\phi)}\right|\mathbb{d}A\newline 
&=\int _ {\mathcal{H}^2}p _ {\text{img}}(\pmb{\omega} _ {i})\left|\frac{\mathrm{d}A}{\mathrm{d}\pmb{\omega} _ {i}}\right|\left|\det J _ A{\Theta}\right|\left|\frac{\partial(i,j)}{\partial(\theta,\phi)}\right|\mathbb{d}\pmb{\omega} _ {i}\newline 
&=\int _ {\mathcal{H}^2}p(\pmb{\omega} _ {i})\mathbb{d}\pmb{\omega} _ {i}\newline 
\end{aligned}
$$


其中根据 *Figure 2* 以及环境贴图对应关系得
$$
\begin{align}
&\left|\frac{\mathrm{d}\pmb{\omega} _ {i}}{\mathrm{d}A}\right|=\frac{|\cos\theta _ {o}|}{\|\pmb{x}-\pmb{y}\|^2}=\frac{1}{R^2}\\
&\left|\det J _ A\Theta\right|=\frac{1}{R^2\sin\theta}\\
&\left|\frac{\partial(i,j)}{\partial(\theta,\phi)}\right|=\frac{wh}{2\pi^2}
\end{align}
$$
其中：

> - $i \in [0, w]$，$w$ 是图像宽度
> - $j\in[0,h]$，$h$ 是图像高度
> - $u,v\in[0,1]$，且 $u=i/w$，$v=j/h$ 
> - $\theta\in[0,\pi]$，$\theta=\pi(1-v)$ 
> - $\phi\in[0,2\pi]$，$\phi=2\pi u$ 
> - $\pmb{\omega} _ i=(\sin\theta\sin\phi,\cos\theta,\sin\theta\cos\phi)$ 

所以
$$
p(\pmb{\omega} _ {i})=\frac{wh}{2\pi^2\sin\theta}p _ {\text{img}}(i,j)\tag9
$$
此时 *equation 6*可改写为：
$$
\begin{aligned}
L _ {\text{dir}}(\pmb{p},\pmb{\omega} _ {o})&=\int _ A f _ {r}(\pmb{y}\to \pmb{x}\to\pmb{z})L _ {e}(\pmb{y}\to\pmb{x})G(\pmb{x}\leftrightarrow\pmb{y})\mathbb{d}A(\pmb{y})
\\&=\int _ {\mathcal{H}^2}\frac{ f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})L_i(\pmb{p},\pmb{\omega} _ {i})\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}}{p(\pmb{\omega} _ {i})}\times p(\pmb{\omega} _ {i})\mathbb{d}\pmb{\omega} _ {i}
\\&=\int _ {I}\frac{ f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})L_e(\pmb{p},\pmb{\omega} _ {i})\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}}{p(\pmb{\omega} _ {i})}\times p _ {\text{img}}(i,j)\mathbb{d}i\mathbb{d}j
\\&=\sum_{I_{i,j}}\frac{ f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})L_e(\pmb{p},\pmb{\omega} _ {i})\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}}{p _ {\text{img}}(i,j)}\times \frac{2\pi^2\sin\theta}{wh}
\end{aligned}\tag{10}
$$
其中求和是按照重要性采样得到的分布 $p _ {img}(i,j) $ 去离散采样。

#### Alias Method

如果环境光贴图的大小是 $m=width\times height=n\times n$ 个像素点，那么常规的离散采样法抽样得到一个采样点的平均时间复杂度为 $O(m)$ ,而Alias Method 可以使得离散采样的时间复杂度为 $O(1)$。具体操作如下

##### Initialize Table 

0. 假设初始概率分布表为 $U(k=i * n + j)=p(i,j) * m$，别名表 $K_k =k$ ，其中 $k\in[0,m)$,初始化两个队列A、B分别存储 $U_k$ 小于1的节点标号，和 $U_k$ 大于1的节点标号

1. 从A、B中各出队列一个值 $a=A.pop()、b=B.pop()$

2. 修改概率分布表， $U(b) = U(b) - (1 - U(a))$；修改别名表，$K_k=b$

3. 如果 $U(b)<1$，则向A队列加入b元素 $A.push(b)$

   如果 $U(b)>1$，则向B队列加入b元素 $B.pushback(b)$

4. 如果 A,B为空，则结束；否则返回 step 1

##### Sample Operation

1. 生成两个随机数 $\xi_1,\xi_2\in[0,1)$

2. 设 $k_1=\lfloor m*\xi_1 \rfloor\in \{ 0,1,2,3,...m-1\} $

3. 如果 $\xi_2 \le U(k_1)$ ,则采样点$(i，j)= (k_1\%n,k_1/n)$

   否则令 $k_2=K(k_1)$ 采样点 $(i,j)= (k_2\%n,k_2/n)$

该算法的采样时间复杂度为 $O(1)$

## 2.3 间接光

### 2.3.1 间接光照方程

除了要计算直接光照以外，还要计算**间接光照**
$$
\begin{aligned}
L _ {\text{indir}}(\pmb{p},\pmb{\omega} _ {o})
=\int _ {\mathcal{H}^2(\pmb{n}(\pmb{p}))}L _ {r}(\pmb{p},-\pmb{\omega} _ {i})f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})\cos\theta\mathbb{d}\pmb{\omega} _ {i}
\end{aligned}\tag{11}
$$


<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\indirect.png" 
        width = "75%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 4: Indirect Light
    </div>
    <p> </p>
</center>

如果回顾一下 *equation 4* 就会发现**间接光照要递归！**

设摄像机的位置为O点，光源为T点，我们想要计算P点射到O点的Radiance，我们Path Tracing在OPQ这段路径上

- 如果没有物体相遇，则递归结束，$L_o=L_r=L_{\text{dir}}$

- 如果有物体相遇，则直接把Q看做光源，那么对OPQ段**解第一次渲染方程**，方程中被积函数的 P点入射Radiance **Li(p,ωi)** 是未知的，而这个P点入射光 **Li** 又是Q点的出射光 **Lo** ，则可以在PQT上解**第二个渲染方程**。一直递归下去，直到遇到环境光或者光源。

### 2.3.2 指数爆炸问题

1. 随着弹射次数的增多，打出的光线数量会指数级上升，设N是对入射方向的采样次数, 那么随着我们递归深度的增加，递归栈将会指数增长

   **解决办法：**当且仅当N = 1时，递归不会出现指数爆炸的现象， 所以我们并不是选择N个方向的入射光，而是**随机**选1条入射光

2. 在自然界随着光照弹射实际上是无限多次的，假如我们限定弹射次数必然损失能量。不想无限递归又不想损失能量，该怎么办呢？

   **解决办法：** 俄罗斯轮盘赌 RussianRoulette(RR)

   - 我们的目标是求着色点的着色结果，即该点到相机方向的**Lo**
   - 假设我们手动设置一个概率**P**（0 < P < 1）
      以P概率继续发射一条光线，返回Lo除以P之后的着色结果：**Lo / P**
      以1-P的概率不发射光线，返回**0**
   - 最终计算离散型随机变量的期望 $E = P \times (Lo / P) + (1 - P) \times 0$

3. 由于我们只随机采样1条光线，会产生非常多的噪点。因为有些着色点计算时，随机采样的某个入射光方向wi并没有击中光源，甚至也没有击中其他物体。

   **解决办法：** 增大spp，即每个像素追踪更多的路径，然后取平均值。所以呢只要用足够多的path，就能够确保有更大的几率击中有效光源或物体，计算出接近正确的着色。

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\spp.png" 
        width = "75%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 4: Compare spp
    </div>
    <p> </p>
</center>

## 2.4 算法

至此，我们已经得到了所有关于求解 *equation 1*的算法，伪代码形式如下

```c
Shade(p,wo)
{
	// 1、Direct illumination from light sources and ambient light.
    //pdf_light = 1 / A
    Uniformly sampling light sources by randomly selecting a point on the surface of the light source.x';   
	shoot a ray form p to x';
	L_dir = 0.0;	
	if (the ray is not blocked in the middle)	// if light is blocked
        if (the ray from source light)
			L_dir += L_e * f_r * cosθ * cosθ' / |x' - p|^2 / pdf_light;//2.2.1
    	else if(the ray from environment backgroud)
            L_dir += L_e * f_r * cosθ / pdf_env * (2*pi*2pi*sinθ)/(w*h)
	//2、Indirect illumination from other objects.
	L_indir = 0.0;
	Test Russian Roulette with probability P_RR;    
    //pdf_hemi = 1 / 2π
	Uniformly sample the hemisphere toward wi;  
	Trace a ray r(p,wi);
	if (ray r hit a non-emitting object at q)
	    L_indir = shade(q, -wi) * f_r * cosθ / pdf_hemi / P_RR; 
    return L_dir + L_indir;
}
```

# 3 Result



## 3.1 低spp

我们在低spp(spp=4)情况下模拟

### 3.1.1 只有直接光

首先只考虑直接光照模型，即
$$
L_o=L_{dir}
$$
得到一下结果

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\rst_dir.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 5: Only direct light
    </div>
    <p> </p>
</center>
结果分析：仅加入直接光照的效果，对于高反射率的金属球效果不好，但已经基本能展示图像了


### 3.1.2 全局光照

现在我们加上间接光照，即
$$
L_o=L_{\text{dir}}+L_{\text{indir}}
$$

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\rst_full.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 6: Global Illumination
    </div>
    <p> </p>
</center>
结果分析：加入间接光照，在全局光照模型下金属球表现得更加真实，可以明显反射出其他物体的图案

### 3.1.3 Monte Carlo重要性采样

考虑对环境光的重要性采样

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\rst_full_impSamp.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 7: Global Illumination by important sampling
    </div>
    <p> </p>
</center>

结果分析：当对环境光照进行重要性采样时，图像的噪点有所下降，而图像的阴影展现的更好

## 3.2 高spp

最终我们增大spp个数(spp=1024)，显示结果

<center>
    <img style = "
        border-radius: 0.3125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);" 
        src = "..\images\img-path-tracing\rst_high_spp.png" 
        width = "50%">
    <br>
    <div style = "
        color: orange;
        border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">
        Figure 8: Global Illumination with high spp
    </div>
    <p> </p>
</center>

结果分析：当增多追踪的光学个数后，场景已经十分逼真了，成功的渲染出图像

## 4 Summary

- 本文首先分析了渲染方程的原理，对渲染方程进行合理的分解
- 然后，介绍了如何采用Mento Carlo 积分法进行采样，即对直接光照采样区域变化和重要性采样的方法；并借助Alias算法实现抽样效率的提高。通过实验结果可以看出重要性采样对提高图像质量具有很好的作用。
- 接着，阐述了间接光照递归求解的方法，并且为防止指数爆炸的问题提出一系列方法
- 最终，在提高每个像素点采样数后，图像已经能够成功渲染出结果了。
