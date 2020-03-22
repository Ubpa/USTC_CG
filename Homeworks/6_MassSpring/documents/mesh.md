# 数据及参数说明

## 网格数据

### 三角网格数据

- [square_sparse.obj](https://github.com/Ubpa/USTC_CG_Data/tree/master/Homeworks/06_MassSpring/Mesh/square_sparse.obj) 
- [square_dense.obj](https://github.com/Ubpa/USTC_CG_Data/tree/master/Homeworks/06_MassSpring/Mesh/square_dense.obj) 

### 四面体网格数据

- [cuboid.tet](https://github.com/Ubpa/USTC_CG_Data/tree/master/Homeworks/06_MassSpring/Mesh/cuboid.tet) 

## 试验例子

- 前两个为正方形网格数据，一个三角网格较密，一个较稀疏，可固定顶部两个端点或顶部边，将另一头的顶点拎起后释放，模拟其在重力作用下的仿真效果
- 第三个为一个长方体网格数据，可固定一侧的面实现其在重力作用下的弯曲仿真效果

## 参数选取

- 弹簧的劲度系数可以先选取大一点（例如1e5），所有弹簧的劲度系数设置相同
- 重力加速度设置为 9.8（测试时可以先设置为1）
- 质点质量统一设置为 1
- 程序中有按钮设置网格 x 坐标最小的点为固定点，也可以添加其他设置固定点的交互
- 也可以自己测试其他的参数，看仿真结果有何区别
