# 作业说明

## 材料

### 网格

- [square_sparse.obj](https://github.com/Ubpa/USTC_CG_Data/tree/master/Homeworks/06_MassSpring/Mesh/square_sparse.obj) 
- [square_sparse.obj](https://github.com/Ubpa/USTC_CG_Data/tree/master/Homeworks/06_MassSpring/Mesh/square_sparse.obj) 
- [cuboid.tet](https://github.com/Ubpa/USTC_CG_Data/tree/master/Homeworks/06_MassSpring/Mesh/cuboid.tet) 

## 实现例子

作业可以利用上述网格实现一些例子：前两个为正方形，一个三角网格较密，一个稀疏，可固定两个端点或一条边模拟在重力作用下的效果；第三个为一个长方体网格，固定一侧的面实现在重力作用下的效果。



## 一些参数的选取

弹簧的劲度系数可以先选取大一点(例如1e5)，所有弹簧的劲度系数设置相同；重力加速度设置为9.8(测试时可以先设置为1);质点质量统一设置为1;程序中有按钮设置网格x坐标最小的点为固定点，可以添加其他设置固定点的交互