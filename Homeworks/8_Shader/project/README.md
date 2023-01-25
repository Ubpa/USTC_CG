# 项目说明

## 依赖配置

- 用 CMake 安装 [glfw3](https://github.com/glfw/glfw) 
  - CMake-GUI configure 后
    - 勾选 `BUILD_SHARED_LIBS` 
    - 去掉勾选 `GLEW_BUILD_DOCS`，`GLEW_BUILD_EXAMPLES`，`GLEW_BUILD_TESTS` 
    - `CMAKE_INSTALL_PREFIX` 修改成自己想要的值（放在 C 盘的话需要用管理员权限打开 VS2019）
  - CMake-GUI Generate，Open Project，执行其中的 INSTALL 项目（右键生成）
  - 将 `<your-path-to-GLFW>/bin` 加到系统环境变量 `Path` 中
  - 删除 glfw3 的 build
- VS 2019
- VS2019 -> 菜单栏 -> 扩展 -> 管理扩展 -> 搜索 glsl -> 安装 **GLSL language integration** 
- [Python](https://www.python.org/) 3.7（也可 VS2019->工具->获取工具和功能->下载 Python）

环境准备好之后，用 CMake-GUI 进行 configure 时会下载一些依赖项目（UCMake，UTemplate，UDP，UGM，UGL 等），较慢，网络良好的情况下耗时 2 分钟左右。

在 CMake-GUI 中找到 `Ubpa_BuildTest` 并勾选，可以构建测试用例（大概 70 个，包含各个项目的测试用例，可以玩玩看），可能会因为路径过长而部分项目构建失败，解决办法是将 project 放到浅层路径中，如 `D:/`。

对于使用 N 卡的同学，可以下载 [Nsight](https://developer.nvidia.com/nsight-visual-studio-edition)（自行研究如何安装，着重参考[手册](https://docs.nvidia.com/nsight-visual-studio-edition/2019.4/Nsight_Visual_Studio_Edition_User_Guide.htm#Release_Notes_in_User_Guide.htm)）来进行 debug（查看渲染指令，中间结果，uniforms 等等），配置好之后可以在 vs2019 的菜单栏中选择 扩展->Nsight-> Start Graphics Debugging（需要提前编译好需要 debug 的项目），之后会运行项目，按 ctrl+Z 后再按空格可截取一帧

![Nsight0.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/Nsight0.jpg)

并在 vs2019 的 API Inspector 中可查看各 drawcall 的情况

![Nsight1.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/08_Shader/Nsight1.jpg)

## 框架说明

UGL 是 OpenGL 的 C++ wrapper，提供了 OpenGL 对象，函数，`enum` 等的简单封装，从而可以简化资源申请和释放，简化资源设置，避免错误使用 `GLenum` 等

使用方法如下

- 使用 UGL 提供的 `gl::VertexArray`，`gl::Shader`，`gl::Program` 等对象
- 使用带 c++ `namespace` 的 OpenGL 接口，如 `gl::Viewport`，同于裸接口 `glViewport` 
- 使用裸接口，如 `glViewport`（不推荐）

## 用户手册

框架提供了三个项目，用户可以使用 WSADQE 和鼠标来漫游场景。

另外项目 `HW8_Shader_app_1_denoise` 和 `HW8_Shader_app_2_shadow` 可使用空格键来开启/关闭相关效果（降噪、阴影），以便对比调试。

## 作业

### 1. 法线贴图和置换贴图

完成 [data/shaders/dn.vert](data/shaders/dn.vert) 和 [data/shaders/light_dn.frag](data/shaders/light_dn.frag) （以文本的方式打开，可使用 sublime，notepad++，vscode，vs2019 等，已自动加入到解决方案中，安装 **GLSL language integration** 后含有语法高亮支持）的 TODO 部分

可修改置换函数

### 2. 降噪

完成 [src/app/1_denoise/main.cpp](src/app/1_denoise/main.cpp) 中的函数 `genDisplacementmap()` ，所生成图片会保存到 [data/](data/) 中（名为 `1_denoise_displacement_map.png`）

完成 [data/shaders/p3t2n3_denoise.vert](data/shaders/p3t2n3_denoise.vert) 中的 TODO 部分

### 3. （可选）点光源阴影

完成 [src/app/2_shadow/main.cpp](src/app/2_shadow/main.cpp) 中的 TODO 部分

完成 [data/shaders/light_shadow.frag](data/shaders/light_shadow.frag) 中的 TODO 部分

