# obj 文件

OBJ 文件不需要任何种[文件头](http://baike.baidu.com/view/1606305.htm)(File Header)，可以使用几行文件信息的注释作为文件的开头。OBJ 文件由一行行文本组成，注释行以符号“#”为开头，空格和空行可以随意加到文件中以增加文件的可读性。多行可以逻辑地连接在一起表示一行，方法是在每一行最后添加一个连接符(\)。注意连接符(\)后面不能出现空格或Tab格，否则将导致文件出错。

在这仅列出了常使用的数据类型：

**顶点数据(Vertex data)：**

- v 几何体顶点(Geometric vertices)
- vt [贴图](http://baike.baidu.com/view/366383.htm)坐标点(Texture vertices)
- vn 顶点法线(Vertex normals)
- vp 参数空格顶点 (Parameter space vertices)

OBJ 文件不包含面的颜色定义信息，不过可以引用材质库，材质库信息储存在一个后缀是".mtl"的独立文件中。关键字"mtllib"即材质库的意思。

材质库中包含材质的漫射(diffuse)，环境(ambient)，光泽(specular)的 RGB (红绿蓝)的定义值，以及反射(specularity)，折射(refraction)，透明度(transparency)等其它特征。

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

