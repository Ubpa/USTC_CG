# 数据及参数说明

## 网格数据

### 三角网格数据

- [square_sparse.obj](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/06_MassSpring/Mesh/square_sparse.obj)  一个10x10格子的网格数据
- [square_dense.obj](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/06_MassSpring/Mesh/square_dense.obj) 一个20x20格子的网格数据

### 四面体网格数据

- [cuboid.tet](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/06_MassSpring/Mesh/cuboid.tet) 

### STL 网格数据

- Tetgen 以 STL 网格数据作为输入：STL数据也是一种常见的 3D 网格的数据格式，常用于 3D 打印的领域
  - 有关 STL 的数据格式可以在网上找到，比如[->](https://all3dp.com/what-is-stl-file-format-extension-3d-printing) 
  - 不少网站提供 STL 数据的下载，比如[->](https://www.thingiverse.com) 
- STL 网格数据也可以使用 Windows 10 自带的“3D查看器”进行浏览查看
- STL 网格数据有文本和二进制两种存储格式，文本的方式可读性比较好，与 OBJ 网格数据比较相似
  - 这是一个边长为20的立方体的 STL 网格数据[cube.stl](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/06_MassSpring/Mesh/cube.stl) 
  - 也可以自己生成其他的 STL 数据用于测试
- 使用 Tetgen 生成四面体网格数据时须注意调整剖分参数

## 试验例子

- 前两个为正方形网格数据，一个三角网格较密，一个较稀疏，可固定顶部两个端点或顶部边，将另一头的顶点拎起后释放，模拟其在重力作用下的仿真效果
- 第三个为一个长方体网格数据，可固定一侧的面实现其在重力作用下的弯曲仿真效果

## 参数选取

- 弹簧的劲度系数可以先选取大一点（例如1e5），所有弹簧的劲度系数设置相同
- 重力加速度设置为 9.8（测试时可以先设置为1）
- 质点质量统一设置为 1
- 程序中有按钮设置网格 x 坐标最小的点为固定点，也可以添加其他设置固定点的交互
- 也可以自己测试其他的参数，看仿真结果有何区别
