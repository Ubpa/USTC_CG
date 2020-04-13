# 项目说明

## 依赖配置

- 用 CMake 安装 [glfw3](https://github.com/glfw/glfw) （将 `<your-path-to-GLFW>/bin` 加到系统环境变量 `Path` 中，注意删除 glfw3 的 build）
- VS 2019
- [Python](https://www.python.org/) 3.7（也可 VS2019->工具->获取工具和功能->下载 Python）

环境准备好之后，用 CMake-GUI 进行 configure 时会下载一些依赖项目（UCMake，UTemplate，UDP，UGM，UGL 等），较慢，网络良好的情况下耗时 2 分钟左右

## 框架说明

UGL 是 OpenGL 的 C++ wrapper，提供了 OpenGL 函数的简单封装，从而可以简化资源申请和释放，资源设置，避免错误使用 `GLenum` 等

使用方法如下

- 使用 UGL 提供的 `gl::VertexArray`，`gl::Shader`，`gl::Program` 等对象
- 使用带 c++ `namespace` 的 OpenGL 接口，如 `gl::Viewport`，同于裸接口 `glViewport` 
- 使用裸接口，如 `glViewport`（不推荐）

## 作业

### 1. 法线贴图和置换贴图

完成 [data/shaders/dn.vs](data/shaders/dn.vs) 和 [data/shaders/light_dn.fs](data/shaders/light_dn.fs) （以文本的方式打开，可使用 sublime，notepad++，vscode，vs2019 等）的 TODO 部分

可修改置换函数

### 2. 降噪

完成 [src/app/1_denoise/main.cpp](src/app/1_denoise/main.cpp) 中的函数 `genDisplacementmap()`  

完成 [data/shaders/p3t2n3_denoise.vs](data/shaders/p3t2n3_denoise.vs) 中的 TODO 部分

### 3. （可选）点光源阴影

完成 [src/app/2_shadow/main.cpp](src/app/2_shadow/main.cpp) 中的 TODO 部分

完成 [data/shaders/light_shadow.fs](data/shaders/light_shadow.fs) 中的 TODO 部分

