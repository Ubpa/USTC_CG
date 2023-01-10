# 作业说明

> 旧版资料：https://pan.baidu.com/s/151e-R8tEKmI3die5rBVBAA
>
> **只须看其 demo 目录的可执行程序及操作演示** 

## 测试图片及报告范例

- 测试例子：须用以下 [**格子图像**](../project/data/test.png) 来进行测试，可以很清楚看到 warping 方法的特点

![https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/test.png](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/test.png)

> 图片无法加载则访问该链接：[USTC_CG_Data@master/Homeworks/02_ImageWarping/test.png](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/test.png) 

- 作业实验报告范例：[示例参考](http://pan.baidu.com/s/1i3mi2yT) 

## 其他测试图片

- 用户交互示例如下：

![https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/ui_demo.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/ui_demo.jpg)

> 图片无法加载则访问该链接：[USTC_CG_Data@master/Homeworks/02_ImageWarping/ui_demo.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/ui_demo.jpg) 

- 变形效果示例如下：

![https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/warp_demo.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/warp_demo.jpg)

> 图片无法加载则访问该链接：[USTC_CG_Data@master/Homeworks/02_ImageWarping/warp_demo.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/warp_demo.jpg) 


## 详细说明

### 面向对象编程思想

通过 [C++ 课前热身练习](../../0_CppPratices) 你已经掌握了面向对象编程的基本思想（类的封装、继承、多态），其理念就是：

- **程序＝对象＋对象＋对象＋…** 

- 对象＝数据结构＋算法

这与面向过程编程（程序＝数据结构＋算法）是不一样的。

### 图像库

作为图像编程的入门，可以使用 Qt 自带的 `QImage` 类来实现算法。

- [project](../project) 是一个基于 `QImage` 来操作图像的示范工程。你只要看懂 `ImageWidget::Invert` 函数，**模仿使用 `QImage` 类中的四个函数 ( `width()`, `height()`, `pixel()`, `setPixel()`)** 即可操作图像的处理。不必去看其他图像处理的书籍和知识后才来处理图像编程。建议大家通过该工程来实现一个非常简单的图像算法，比如线性方法的 `Color2Gray`；
- **注意 `QPoint` 的坐标是整数**，你不能拿整数来做运算，要用浮点运算才能保证计算精度。

### Eigen库

- 实现RBF方法需要求解线性方程组，你可以自己实现，也可以从网上找其他程序或库来用
- 强烈推荐使用 Eigen 库来求解线性方程组，Eigen 库是强大的数学算法库，是计算机图形学必须使用的算法库
- 我们提供了 Eigen 库的使用示例：[eigen_example](eigen_example/) 

> [eigen_example](eigen_example/) 演示的添加依赖的方式重点掌握，另外为了保证项目的简洁性，不要将依赖部分加到 git 版本管理中，使用 [.gitignore](../../../.gitignore) 忽略掉 [eigen_example/include/_deps/](eigen_example/include/_deps/) 


### 补洞（Optional）

结果图像中有时会出现白色空洞或条纹，你需要分析是什么原因造成的？空洞的填补可以利用周围的已知像素进行插值填充。这也是个插值问题（即利用空洞周围一定范围的已知像素来插值该像素的颜色）。你可以尝试如何用你实现的 `IDW warping` 类（或者对 `IDW warping` 类的简单改造。）来填充这些空洞像素的颜色？

![https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/white_stitch.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/white_stitch.jpg)

> 图片无法加载则访问该链接：[USTC_CG_Data@master/Homeworks/02_ImageWarping/white_stitch.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/02_ImageWarping/white_stitch.jpg) 

### ANN库（Optional）

若你需要用搜索最近点的任务（在补洞的任务中），建议学习使用如下的库：

[ANN: A Library for Approximate Nearest Neighbor Searching](http://www.cs.umd.edu/~mount/ANN/) 

我们提供了测试项目 [ann_example](ann_example/) 


### 注意事项

- 只须看懂英文论文中的主要思想及计算过程即可，不必追究太多背景知识及细节内容；实现基本算法即可，不必拘泥于太多细节。

- 目录不要用中文名，否则编译会出错。要习惯用英文来思考，包括代码注释等。养成使用英文的习惯！

- 务必自己独立完成该作业，做得不好没有关系，我们会指出你的问题，一步一步帮你理解该作业需要你所理解的东西，这点极其重要！只有不断从失败中改正才能有长进！我们会帮你逐步纠正错误。

