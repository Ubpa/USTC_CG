# 作业说明

## 1. 作业任务

- 实现 **Poisson Image Editing 算法** [^03Perez] 
- 实现[**扫描线算法**](ScanningLine.md)：对于任意多边形边界的封闭区域，要得到它的内部像素，需要使用扫描线算法。参考任一计算机图形学书。
- 实时拖动多边形区域得到结果：需要使用矩阵求解方法（分解方法），对特定的问题使用适应的矩阵分解方法会极大的提高求解效率，提升求解稳定性。
- 使用矩阵**预分解**技术，实现**实时**的 Poisson Image Editing。
- 对于线性方程组 $AX=b_i$，对于相同的 $A$，不同的 $b_i$，如果每次都重新整体求解方程组，耗时太严重。常用的做法是对 $A$ 进行预分解，这样对于不同的 $b_i$，只需要花费很少的时间，就可以得到解。
- Eigen 库中，稀疏求解器（例如 `SparseLU`）中的函数 `compute` 就是对矩阵进行预分解。
- MKL 库的实现较底层，有能力的同学可以尝试使用。**一般建议使用 Eigen 库**。
- 仔细思考：使用**一种**稀疏方程组求解方法（LU，QR，cholesky，……）求解线性方程组。有些方法不适用该问题，可能效率很低，也可能求解失败。请选择一种适合的方法。
- 学会使用 Qt 多窗口显示框架：项目 [project/](../project/)  写好了一个多窗口框架，展示了如何得到每个子窗口的指针即可。详见说明文档。
- 继续巩固 OOP 编程思想。

## 2. 1. 测试说明

### 1.2 测试图像

须用 [**测试图像**](../project/data/) 来进行测试

### 1.1 结果示例

![https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/03_PoissonImageEditing/demo.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/03_PoissonImageEditing/demo.jpg)

## 3. 资源介绍

- 进一步规范代码实现规范和习惯，继续阅读并提高 C++ 编程的技巧和经验
  - 提高 C++ 编程的 50 项技巧（[百度网盘](http://pan.baidu.com/s/1gdECTBd)）
  - 改进 C++ 程序的 6 个技巧（[百度网盘](http://pan.baidu.com/s/1ntNojhf)）
  - [Google 命名规则](http://zh-google-styleguide-fix.readthedocs.org/en/latest/google-cpp-styleguide/naming.html) 
- 如何查找文献（[百度网盘](http://pan.baidu.com/s/1o6z56T8)）
- [数学在计算机图形学中的应用](http://staff.ustc.edu.cn/~lgliu/Resources/CG/Math_for_CG_Turk_CN.htm) 
- Qt 原理了解：现在开始可以有意识地开始看一些有关 Qt 原理的参考书，在使用的过程中逐步体会。另外，可以逐渐追踪 Qt 的源代码，自己试着修改 Qt 的代码，实现自己的功能。比如，你也可以试着追踪 STL 的源码，看看其实现原理，且与你以前自己实现的 Dynamic Array 的原理比较一下。Qt 5 刚刚出现不久（2014/04/12），较之 Qt 4 有较大变动，而市面上大多数书都是 Qt 4。所以我们更建议你看 Qt 官方 example，以及在 Qt 官网上的资料。
- 学习使用图像库 OpenCV：`QImage` 类只是一个非常简单的图像类，本次作业须学习使用 OpenCV（Open Computer Vision）库。它是一个非常强大的图像处理的开发库，集成了很多算法。是从事图像处理和计算机视觉的研究工作者首选的图像库。我们提供了配置和使用 OpenCV 的示例项目 [opencv_example](opencv_example/)，请务必编译运行成功。

## 参考

[^03Perez]: Pérez P, Gangnet M, Blake A. [**Poisson image editing**](http://citeseerx.ist.psu.edu/viewdoc/download;jsessionid=3E90823236109B11FC90C49DCA2679D5?doi=10.1.1.133.6932&rep=rep1&type=pdf)[M]//ACM SIGGRAPH 2003 Papers. 2003: 313-318.

