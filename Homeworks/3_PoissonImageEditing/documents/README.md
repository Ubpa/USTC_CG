# 说明

## 任务

- 实现 Siggraph 2003 论文 “Poisson Image Editing” 的算法 [下载](https://www.cs.jhu.edu/~misha/Fall07/Papers/Perez03.pdf)
  - 至少须实现文中的Seamless cloning的应用
- 实现[**多边形扫描转换算法**](ScanningLine.md)，可参考任一计算机图形学教材。
- 学习使用 Qt 多窗口显示框架：项目 [project/](../project/)  写好了一个多窗口框架，展示了如何得到每个子窗口的指针即可。详见说明文档。
- 学习使用图像库 OpenCV：`QImage` 类只是一个非常简单的图像类，本次作业须学习使用 OpenCV（Open Computer Vision）库。它是一个非常强大的图像处理的开发库，集成了很多算法。是从事图像处理和计算机视觉的研究工作者首选的图像库。我们提供了配置和使用 OpenCV 的示例项目 [opencv_example](opencv_example/)，请务必编译运行成功。
- 继续巩固 OOP 编程思想。
- 实时拖动多边形区域得到结果 (Optional)
  - 使用矩阵的**预分解**技术，实现**实时**的 Poisson Image Editing；对特定的问题使用适应的矩阵分解方法会极大的提高求解效率，提升求解稳定性。
  - 对于线性方程组 $AX=b_i$，对于相同的 $A$，不同的 $b_i$，如果每次都重新整体求解方程组，耗时太严重。常用的做法是对 $A$ 进行预分解，这样对于不同的 $b_i$，只需要花费很少的时间，就可以得到解。
  - Eigen 库中，稀疏求解器（例如 `SparseLU`）中的函数 `compute` 就是对矩阵进行预分解。
  - 仔细思考：使用**一种**稀疏方程组求解方法（LU，QR，cholesky，……）求解线性方程组。有些方法不适用该问题，可能效率很低，也可能求解失败。请选择一种适合的方法。
  



## 参考文献

[^03Perez]: Pérez P, Gangnet M, Blake A. [**Poisson image editing**](https://www.cs.jhu.edu/~misha/Fall07/Papers/Perez03.pdf), ACM Transactions on Graphics (Proc. SIGGRAPH), 22(3): 313-318, 2003. 

