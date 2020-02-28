# 多边形的扫描转换算法

多边形的扫描转换算法是多边形区域光栅化（求解一个平面多边形区域的内部像素）的经典算法，可在任何一本计算机图形学的课本上都能找到，网上也有不少详细介绍资料。

算法的基本思想是：通过维持一个特别的数据结构（结构中保存扫描线与多边形的交点）进行填充。

![https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/03_PoissonImageEditing/scan_line.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/03_PoissonImageEditing/scan_line.jpg)

> 图片无法加载则访问该链接：[USTC_CG_Data@master/Homeworks/03_PoissonImageEditing/scan_line.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/03_PoissonImageEditing/scan_line.jpg) 

