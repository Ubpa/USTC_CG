# obj 三维网格文件

OBJ 三维网格文件不需要任何[文件头](http://baike.baidu.com/view/1606305.htm)(File Header)，可以使用几行文件信息的注释作为文件的开头。OBJ 文件由一行行文本组成，注释行以符号“#”为开头，空格和空行可以随意加到文件中以增加文件的可读性。多行可以逻辑地连接在一起表示一行，方法是在每一行最后添加一个连接符(\)。注意连接符(\)后面不能出现空格或Tab格，否则将导致文件出错。

更详细的OBJ 三维网格文件的格式说明可查看课程百度云目录"Homework/Homework4/三角网格数据/MeshFileFormat"。

在这仅列出了常使用的数据类型：

**数据(Vertex data)：**

- v 网格顶点(Geometric vertices)
- vn 顶点法向量(Vertex normals)
- vt [纹理贴图](http://baike.baidu.com/view/366383.htm)坐标(Texture coordinates)
- vp 参数空格顶点 (Parameter space vertices)
- f 网格的面，由若干顶点（序号）组成。在OBJ中，顶点序号是从1（而不是0）开始的。

**材质(Material data)：**

OBJ 文件不包含面的颜色定义信息，不过可以引用材质库，材质库信息储存在一个后缀是".mtl"的独立文件中。关键字"mtllib"即材质库的意思。

材质库中包含材质的漫射(diffuse)，环境(ambient)，光泽(specular)的 RGB (红绿蓝)的定义值，以及反射(specularity)，折射(refraction)，透明度(transparency)等其它特征。

**一个例子**

下面是一个多边形的例子：

```
v -2 0 0
v -1 1 0
v 1 1 0
v 2 0 0
v 1 -1 0
v -1 -1 0
v 0 0 0
f 1 6 2
f 6 7 2
f 6 5 7
f 2 7 3
f 7 5 3
f 3 5 4
```

