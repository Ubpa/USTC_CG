# 作业说明文档

> 旧版资料：https://pan.baidu.com/s/151e-R8tEKmI3die5rBVBAA
>
> **只看其 demo 部分** 

通过以前的练习你学会了

通过 [练习 3](../../0_CppPratices) 你已经掌握了面向对象编程的基本思想（类的封装、继承、多态），你需要好好体会。现在的理念就是：

**程序＝对象＋对象＋对象＋…** 

对象＝数据结构＋算法

这是与面向过程编程不一样的。

用户界面可参考 demo 目录下的程序；需用[**格子图像**](https://raw.githubusercontent.com/Ubpa/PicGoImgBed/master/warp_test.png)来进行测试

![warp_test](https://raw.githubusercontent.com/Ubpa/PicGoImgBed/master/warp_test.png)

须看懂英文论文，只要看懂算法过程即可，不必追究太多背景知识及细节内容；实现基本算法即可，不必拘泥于太多细节。

作为图像编程的入门，可以使用 Qt 自带的 `QImage` 类来实现算法。

- [project](../project) 是一个基于 `QImage` 来操作图像的示范工程。你只要看懂 `ImageWidget::Invert` 函数，**模仿使用 `QImage` 类中的四个函数 ( `width()`, `height()`, `pixel()`, `setPixel()`)** 即可操作图像的处理。不必去看其他图像处理的书籍和知识后才来处理图像编程。建议大家通过该工程来实现一个非常简单的图像算法，比如线性方法的 `Color2Gray`；
- **注意 `QPoint` 的坐标是整数**，你不能拿整数来做运算，要用浮点运算才能保证计算精度。

结果图像中有时会出现白色空洞或条纹，你需要分析是什么原因造成的？空洞的填补可以利用周围的已知像素进行插值填充。这也是个插值问题（即利用空洞周围一定范围的已知像素来插值该像素的颜色）。你可以尝试如何用你实现的 `IDW warping` 类（或者对 `IDW warping` 类的简单改造。）来填充这些空洞像素的颜色？

求解线性方程组的功能，你可以自己实现，也可以从网上找其他程序或库来用。我们建议使用 Eigen 库，我们提供了使用示例：[eigen_example](eigen_example/) 

> [eigen_example](eigen_example/) 演示的添加依赖的方式重点掌握，另外为了保证项目的简洁性，不要将依赖部分加到 git 版本管理中，使用 [.gitignore](../../../.gitignore) 忽略掉 [eigen_example/include/_deps/](eigen_example/include/_deps/) 

若你需要用搜索最近点的任务，可以学习使用如下的库：

[ANN: A Library for Approximate Nearest Neighbor Searching](http://www.cs.umd.edu/~mount/ANN/) （[百度云盘](http://pan.baidu.com/s/1EMZqm)）

注意：目录不要用中文名，否则编译会出错。要习惯用英文来思考，包括代码注释等。养成使用英文的习惯！

务必自己独立完成该作业，做得不好没有关系，我们会指出你的问题，一步一步帮你理解该作业需要你所理解的东西，这点极其重要（只有不断从失败中改正才能有长进！我们会帮你逐步纠正错误）。

