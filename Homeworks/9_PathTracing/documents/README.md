# 作业说明

## 1. 基础知识

### 1.1 渲染方程

$$
L _ {o}(\pmb{p},\pmb{\omega} _ {o})=L _ {e}(\pmb{p},\pmb{\omega} _ {o})+\int _ {\mathcal{H}^2(\pmb{n}(\pmb{p}))} f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})L _ {i}(\pmb{p},\pmb{\omega} _ {i})\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}\mathbb{d}\pmb{\omega} _ {i}
$$

> 其中
>
> - $L _ {o}$ 是出射 radiance
> - $\pmb{p}$ 是渲染点
> - $\pmb{\omega} _ {i}$ 是入射光方向
> - $\pmb{\omega} _ {o}$ 是出射光方向
> - $L _ {e}$ 是发光 radiance
> - $\pmb{n}(\pmb{p})$ 为 $\pmb{p}$ 处法向
> - ${\mathcal{H}^2(\pmb{n}(\pmb{p}))} $ 是法向 $\pmb{n}(\pmb{p})$ 所在半球
> - $f _ {r}$ 是双向散射分布函数（bidirectional scattering distribution function ，BRDF）
> - $L _ {i}$ 是入射 radiance
> - $\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}$ 是 $\pmb{\omega} _ {i}$ 与 $\pmb{n}(\pmb{p})$ 的夹角
>

记

$$
\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L=\int _ {\mathcal{H}^2(\pmb{n}(\pmb{p}))} f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})L\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}\mathbb{d}\pmb{\omega} _ {i}
$$

则

$$
L _ {o}(\pmb{p},\pmb{\omega} _ {o})=L _ {e}(\pmb{p},\pmb{\omega} _ {o})+\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {i}(\pmb{p},\pmb{\omega} _ {i})
$$

反射方程为

$$
L _ {r}(\pmb{p},\pmb{\omega} _ {o})=\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {i}(\pmb{p},\pmb{\omega} _ {i})
$$

对于 $L _ {i}$ 有关系式

$$
L _ {i}(\pmb{p},\pmb{\omega} _ {i})=L _ {o}(\mathop{raytrace}(\pmb{p},\pmb{\omega _ {i}}),-\pmb{\omega _ {i}})
$$

> 其中 $\mathop{raytrace}$ 是射线与场景的相交函数

记 $\mathop{raytrace}(\pmb{p},\pmb{\omega} _ {i})$ 为 $\pmb{p}^\prime$，则有

$$
L _ {i}(\pmb{p},\pmb{\omega} _ {i})=L _ {o}(\pmb{p}^\prime,-\pmb{\omega _ {i}})
$$

如此形成递归

$$
L _ {o}(\pmb{p},\pmb{\omega} _ {o})=L _ {e}(\pmb{p},\pmb{\omega} _ {o})+\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {o}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
$$

更有效地是对 $L _ {r}$ 进行递归

将 $L _ {r}$ 展开一次

$$
\begin{aligned}
L _ {r}(\pmb{p},\pmb{\omega} _ {o})
&=\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}\left(L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i})+L _ {r}(\pmb{p}^\prime,-\pmb{\omega} _ {i})\right)\newline 
&=\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
+\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {r}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
\end{aligned}
$$

将

- $\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i})$ 称为**直接光**，记作 $L _ {\text{dir}}(\pmb{p},\pmb{\omega} _ {o})$ 
- $\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {r}(\pmb{p}^\prime,-\pmb{\omega} _ {i})$ 称为**间接光**，记作 $L _ {\text{indir}}(\pmb{p},\pmb{\omega} _ {o})$ 

### 1.2 直接光

$$
L _ {\text{dir}}(\pmb{p},\pmb{\omega} _ {o})=\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
$$

积分中，对于大部分方向 $\pmb{\omega} _ {i}$，$L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i})=0$（非光源），所以我们直接在光源所在方向上积分

其中 $\pmb{p}$， $\pmb{\omega} _ {o}$ 和 $\pmb{\omega} _ {i}$ 可用三点确定，如下图所示

![xyz.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/09_PathTracing/xyz.jpg)

> 图中 $\pmb{x}$ 即为 $\pmb{p}$，$\pmb{y}$ 即为 $\pmb{p}^\prime$ 

由几何关系可知

$$
\mathbb{d}\pmb{\omega} _ {i}=\frac{|\cos\theta _ {\pmb{y},\pmb{x}}|}{\|\pmb{x}-\pmb{y}\|^2}\mathbb{d}A(\pmb{y})
$$

其中 $\theta _ {\pmb{y},\pmb{x}}$ 是方向 $\pmb{x}-\pmb{y}$ 与 $\pmb{n}(\pmb{y})$ 的夹角

引入几何传输项（两点间的“传输效率”）

$$
G(\pmb{x}\leftrightarrow\pmb{y})=V(\pmb{x}\leftrightarrow\pmb{y})\frac{|\cos\theta _ {\pmb{x},\pmb{y}}||\cos\theta _ {\pmb{y},\pmb{x}}|}{\|\pmb{x}-\pmb{y}\|^2}
$$

>其中 $V(\pmb{x}\leftrightarrow\pmb{y})$ 是可见性函数，当 $\pmb{x}$ 和 $\pmb{y}$ 之间无阻隔时为 $1$，否则为 $0$ 
>
>$G$ 是对称函数，即 $G(\pmb{x}\leftrightarrow\pmb{y})=G(\pmb{y}\leftrightarrow\pmb{x})$ 

故有

$$
L _ {\text{dir}}(\pmb{x}\to\pmb{z})=\int _ A f _ {r}(\pmb{y}\to \pmb{x}\to\pmb{z})L _ {e}(\pmb{y}\to\pmb{x})G(\pmb{x}\leftrightarrow\pmb{y})\mathbb{d}A(\pmb{y})
$$

其中积分域 $A$ 为场景中所有的面积，但只有光源处 $L _ {e}(\pmb{y}\to\pmb{x})\neq 0$ 

记光源数 $N _ {e}$，场景中的光源集为 $\{L _ {e _ {i}}\} _ {i=1}^{N _ {e}}$ ，对应的区域集为 $\{A(L _ {e _ {i}})\} _ {i=1}^{N _ {e}}$，则可写为

$$
L _ {\text{dir}}(\pmb{x}\to\pmb{z})=\sum _ {i=1}^{N _ {e}}\int _ {A(L _ {e _ {i}})} f _ {r}(\pmb{y}\to\pmb{x}\to\pmb{z})L _ {e}(\pmb{y}\to\pmb{x})G(\pmb{x}\to\pmb{y})\mathbb{d}A(\pmb{y})
$$

### 1.3 间接光

递归即可

$$
L _ {r}(\pmb{p},\pmb{\omega} _ {o})=\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
+\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {r}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
$$

### 1.4 蒙特卡洛积分与重要性采样

![mc.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/09_PathTracing/mc.jpg)

![is.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/09_PathTracing/is.jpg)

> **补充** 
>
> 多重重要性采样
>
> 一个重要性采样的策略就会有一个采样概率分布 $p _ i(x)$， 假设可供选择的策略为 $N$ 个 $\{p _ i(x)\} _ {i=1}^N$，我们可以混合他们得到一个采样新策略
>
> 概率分布为 $p(x)=\sum _ {i=1}^N w _ i(x)p _ i(x)$，其中 $\sum _ 1^N w _ i(x)=1,w _ i(x)\ge0$ 
>
> > $w _ i(x)$ 的选择是任意的，如均匀
>
> 采样的伪代码如下
>
> ```c++
> float sample(float rnd){
>     if(rnd < w1)
>         return sample1();
>     else if(rnd < w1 + w2)
>         return sample2();
>     else if(rnd < w1 + w2 + w3)
>         return sample3();
>     ...
>     else
>         return sampleN();
> }
> ```

### 1.5 渲染方程的计算

我们要计算的是如下积分

$$
L _ {r}(\pmb{p},\pmb{\omega} _ {o})=L _ {\text{dir}}+L _ {\text{indir}}
$$

右侧积分式需要递归

利用蒙特卡洛积分可将积分转成采样

$$
\begin{aligned}
L _ {\text{dir}}(\pmb{x}\to\pmb{z})
&\approx\sum _ {i=1}^{N _ {e}}\sum _ {j=1}^{N _ {i}}\frac{f _ {r}(\pmb{y} _ {i}^{(j)}\to\pmb{x}\to\pmb{z})L _ {e}(\pmb{y} _ {i}^{(j)}\to\pmb{x})G(\pmb{x}\to\pmb{y} _ {i}^{(j)})}{p(\pmb{y} _ {i}^{(j)})}\newline 
L _ {\text{indir}}(\pmb{p},\pmb{\omega} _ {o})
&\approx\sum _ {k=1}^{N}\frac{f _ {r}(\pmb{p},\pmb{\omega} _ {i}^{(k)},\pmb{\omega} _ {o})L _ {r}(\pmb{p}^{\prime(k)},-\pmb{\omega})\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}}{p(\pmb{\omega} _ {i}^{(k)})}
\end{aligned}
$$

$L _ {\text{dir}}$ 在各光源区域采样

对于 $L _ {\text{indir}}$ 则半球采样

采样个数皆为 1（$N _ {i}=1\quad(i=1,\dots,N _ {e})$，$N=1$） 

> 也可采用其他采样策略

## 2. 作业要求

### 2.1 路径追踪算法

框架提供了实现路径追踪算法的绝大部分算法（包括场景加速结构，相交，采样等），同学只需实现路径追踪算法核心部分（递归步骤），并渲染场景。

在实际编写路径追踪的代码时，有的材质是绝对光滑的，如镜面。这时直接光的计算退化（光线只沿一个方向出射，即 BRDF 函数中含 delta 函数），只需要知道出射方向处是否与光源相交即可，这正是间接光所要做的。故此时将该计算任务交给计算间接光的步骤里（通过变量 `bool last _ bounce _ specular` 来标识）。

代码中的函数 `PathTracer::Shade` 即为上文中的 $L _ {r}$ 

### 2.2 环境光贴图重要性采样

![is _ em.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/09_PathTracing/is_em.jpg)

参考：[wiki | Alias _ method](https://en.wikipedia.org/wiki/Alias_method) 

优点是采样时间复杂度为 $O(1)$ 

生成概率表和别名表后，可采样离散的像素，相关概率关系如下

$$
\begin{aligned}
1=\int _ {I}p _ {\text{img}}(i,j)\mathbb{d}i\mathbb{d}j
&=\int _ {\Theta}p _ {\text{img}}(\theta,\phi)\left|\frac{\partial(i,j)}{\partial(\theta,\phi)}\right|\mathbb{d}\theta\mathbb{d}\phi\newline 
&=\int _ {A}p _ {\text{img}}(A)\left|\det J _ A\Theta\right|\left|\frac{\partial(i,j)}{\partial(\theta,\phi)}\right|\mathbb{d}A\newline 
&=\int _ {\Omega}p _ {\text{img}}(\pmb{\omega} _ {i})\left|\frac{\mathrm{d}A}{\mathrm{d}\pmb{\omega} _ {i}}\right|\left|\det J _ A{\Theta}\right|\left|\frac{\partial(i,j)}{\partial(\theta,\phi)}\right|\mathbb{d}\pmb{\omega} _ {i}\newline 
&=\int _ {\Omega}p(\pmb{\omega} _ {i})\mathbb{d}\pmb{\omega} _ {i}\newline 
\end{aligned}
$$

其中

![dwi _ dA.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/09_PathTracing/dwi_dA.jpg)

$$
\begin{align}
\left|\frac{\mathrm{d}\pmb{\omega} _ {i}}{\mathrm{d}A}\right|&=\frac{|\cos\theta _ {o}|}{\|\pmb{x}-\pmb{y}\|^2}=\frac{1}{R^2}\newline 
\left|\det J _ A\Theta\right|&=\frac{1}{R^2\sin\theta}\newline 
\left|\frac{\partial(i,j)}{\partial(\theta,\phi)}\right|&=\frac{wh}{2\pi^2}
\end{align}
$$

则

$$
p(\pmb{\omega} _ {i})=\frac{wh}{2\pi^2\sin\theta}p _ {\text{img}}(i,j)
$$

> **补充** 
>
> - $i \in [0, w]$，$w$ 是图像宽度
> - $j\in[0,h]$，$h$ 是图像高度
> - $u,v\in[0,1]$，且 $u=i/w$，$v=j/h$ 
> - $\theta\in[0,\pi]$，$\theta=\pi(1-v)$ 
> - $\phi\in[0,2\pi]$，$\phi=2\pi u$ 
> - $\pmb{\omega} _ i=(\sin\theta\sin\phi,\cos\theta,\sin\theta\cos\phi)$ 

### 2.3 搭建场景并渲染

框架目前提供了两种方式（未来可能新增第三种方式——UI 编辑）创建场景

- 代码
- json

#### 2.3.1 代码方式

对于代码的方式，同学们可以查看框架中 `class Ubpa::SceneGenerator` 的实现来了解具体使用方式

框架的场景层为 [UScene](https://github.com/Ubpa/UScene)，底层框架为**实体-组件-系统**（Entity-Component-System，ECS），相应库为 [UECS](https://github.com/Ubpa/UECS) 

实体由组件构成，组件是数据，系统则视为数据的变换，三者构成一个**世界**（上下文）

世界对应的类为 `class Ubpa::Scene`，统一管理了实体，组件，系统的资源。

实体对应的类为 `class Ubpa::SObj`（**S**cene **Obj**ect），成树状结构

系统为组件的 `OnStart()`，`OnUpdate()`，`OnStop()` 等函数

组件（[UScene/core/Cmpt/](https://github.com/Ubpa/UScene/tree/master/include/UScene/core/Cmpt)）包括

- [Cmpt::Camera](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Camera.h)：（针孔）相机，用于离线渲染和实时渲染
- [Cmpt::Geometry](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Geometry.h)：几何体，内含成员 `Cmpt::Geometry::primitive`（[UScene/core/Primitive/](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Primitive/)），目前可为 [Sphere](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Primitive/Sphere.h)，[Square](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Primitive/Square.h)，[Triangle Mesh](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Primitive/TriMesh.h)（[Triangle](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Primitive/Triangle.h) 仅做算法使用，不可将其设置给 `Cmpt::Geometry::primitive`），通过 `Cmpt::Geometry::SetPrimitive` 设置，资源所有权归 `Cmpt::Geometry`。
- [Cmpt::Light](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Light.h)：光源，内含成员 `Cmpt::Geometry::light`（[UScene/core/Light/](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Light/)），目前可为 [Area Light](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Light/AreaLight.h)，[Environment Light](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Light/EnvLight.h)，[Point Light](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Light/PointLight.h)，通过 `Cmpt::Light::SetLight` 设置，资源所有权归 `Cmpt::Light`。
- [Cmpt::Material](https://github.com/Ubpa/UScene/tree/master/include/UScene/core/Material)：材质，内含成员 `Cmpt::Material::material`（[UScene/core/Material/](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Material/)），目前仅有一种材质 [standard BRDF](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Material/stdBRDF.h)，采用金属工作流，GGX 和 Fresnel-Schlick。通过 `Cmpt::Material::SetMaterial` 设置，资源所有权归 `Cmpt::Material`。
- [Cmpt::Position](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Position.h)：局部位置
- [Cmpt::Scale](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Scale.h)：局部缩放
- [Cmpt::Rotation](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Rotation.h)：局部旋转，内含单位四元数
- [Cmpt::Transform](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Transform.h)：变换，由 `Cmpt::Position`，`Cmpt::Scale`，`Cmpt::Rotation` 决定
- [Cmpt::L2W](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/L2W.h)：local to world 变换，`SObj` 成树状结构，从一个 `SObj` 出发往根走，将所有变换累成则可得到 local to world 变换。
- [Cmpt::Root](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/L2W.h)：根节点，场景仅有一个根节点 `Scene::root` 
- [Cmpt::SObjPtr](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/SObjPtr.h)：储存了 `SObj` 指针

其中 [Cmpt::Position](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Position.h)，[Cmpt::Scale](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Scale.h)，[Cmpt::Rotation](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Rotation.h)，[Cmpt::Transform](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/Transform.h)，[Cmpt::L2W](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/L2W.h)，[Cmpt::SObjPtr](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/SObjPtr.h) 是必要节点，框架会自动添加，[Cmpt::Root](https://github.com/Ubpa/UScene/blob/master/include/UScene/core/Cmpt/L2W.h) 是特殊节点，仅有 `Scene::root` 拥有

代码通过组织以上元素实现场景的构建与运作

这次作业要求同学构建静态场景，不涉及系统。

#### 2.3.2 json

场景可将其所有内容保存起来，形成一个场景文件，为 [json 格式](http://www.json.org/json-zh.html)。该场景文件后续可再载入到程序中，恢复场景。

本框架提供了一个示例场景 [uscene.json](https://github.com/Ubpa/USTC _ CG/tree/master/Homeworks/9 _ PathTracing/project/data/models/uscene.json)，运行 UEditor 后会默认载入该文件

json 将对象视为一个**键值对**的集合，本框架也通过**反射机制**（代码元信息获取）实现该模型，从而实现序列化（C++ 对象 => json）和反序列化（json => C++ 对象）。键即为变量名。

同学们可通过编辑 json 文件来实现创建、修改场景，从而加快迭代速度（直接从代码编辑工作量大）

模仿为主，关键在于

- `type` 
- 键名（通过查头文件获知）
- 默认值（不必写出一个类的所有键值对，缺失的键值即视为默认值，可在头文件查到）
- 路径（路径采用绝对路径或相对路径，目前相对路径的工作目录为 `bin/`，因此一般为 `../data/...`）