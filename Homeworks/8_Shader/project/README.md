# 项目说明

## 依赖配置

- 用 CMake 安装 [glfw3](https://github.com/glfw/glfw) （将 `<your-path-to-GLFW>/bin` 加到系统环境变量 `Path` 中）
- VS 2019
- [Python](https://www.python.org/) 3.7（也可 VS2019->工具->获取工具和功能->下载 Python）

## 作业

### 1. 法线贴图和置换贴图

完成 [data/shaders/dn.vs](data/shaders/dn.vs) 和 [data/shaders/light_dn.fs](data/shaders/light_dn.fs) （以文本的方式打开，可使用 sublime，notepad++，vscode，vs2019 等）的 TODO 部分

### 2. 降噪

完成 [src/app/1_denoise/main.cpp](src/app/1_denoise/main.cpp) 中的函数 `genDisplacementmap()`  

完成 [data/shaders/p3t2n3_denoise.vs](data/shaders/p3t2n3_denoise.vs) 中的 TODO 部分

### 3. （可选）点光源阴影

完成 [src/app/2_shadow/main.cpp](src/app/2_shadow/main.cpp) 中的 TODO 部分

完成 [data/shaders/light_shadow.fs](data/shaders/light_shadow.fs) 中的 TODO 部分

