# 作业完成说明

## 项目说明

本次项目读取obj格式面网格或tet体网格，生成类MassSpring用于传输用于模拟的网格数据，且MassSpring中存储了模拟方法类Simulate；程序ui窗口最上方设置两个按钮分别为开始（或继续）模拟时间计时和停止时间计时；attribute中新添了Simulation组件用于设置模拟的一些参数





## 作业完成步骤

本次作业主要完成类Simulation中Init和SimulateOnce函数，前者主要初始化一些模拟用到的矩阵和辅助变量，后者则是更新下一帧的位置position和速度velocity；模拟只需要顶点的位置和边的索引信息，均已在Simulation中建立完成。

