# 作业项目

## 构建说明

### 环境

- vs2019

- python 3.7

- CMake 3.16.3 及以上

- glfw3（作业 8 已安装）

  > 用 CMake 安装 [glfw3](https://github.com/glfw/glfw) 
  >
  > - CMake-GUI configure 后
  >   - 勾选 `BUILD_SHARED_LIBS` 
  >   - 去掉勾选 `GLEW_BUILD_DOCS`，`GLEW_BUILD_EXAMPLES`，`GLEW_BUILD_TESTS` 
  >   - `CMAKE_INSTALL_PREFIX` 修改成自己想要的值（放在 C 盘的话需要用管理员权限打开 VS2019）
  > - CMake-GUI Generate，Open Project，执行其中的 INSTALL 项目（右键生成）
  > - 将 `<your-path-to-GLFW>/bin` 加到系统环境变量 `Path` 中
  > - 删除 glfw3 的 build

### 步骤

- CMake-GUI
  - configue（期间会下载大量文件，可能由于网络问题导致失败，重新点 configure，直到无错成功
  - **确保 configure 无错后**，找到 CMAKE_INSTALL_PREFIX，设置其他位置，以文件夹 `Ubpa` 结尾，如 `D:/Program_Files/Ubpa` 
  - Generate
  - Open Project，打开 vs2019
- VS 2019
  - （可选）找到项目 `INSTALL`，右键执行，安装 `Ubpa` 系列库（编译时间较久）
  - 找到项目 `HW9/HW9_UEditor`，右键设为启动项，并启动（编译时间较久）

## 框架说明

本框架涉及的依赖库如下

- [UCMake](https://github.com/Ubpa/UCMake) v0.4.1
- [UTemplate](https://github.com/Ubpa/UTemplate) v0.3.1
- [UDP](https://github.com/Ubpa/UDP) v0.3.1
- [UBL](https://github.com/Ubpa/UBL) v0.1.0
- [UGM](https://github.com/Ubpa/UGM) v0.5.1
- [UECS](https://github.com/Ubpa/UECS) v0.5.5
- [UScene](https://github.com/Ubpa/UScene) v0.5.0
- [UGL](https://github.com/Ubpa/UGL) v0.2.0
- [URTR](https://github.com/Ubpa/URTR) v0.0.5
- [UEngine](https://github.com/Ubpa/UEngine) v0.0.2

> 第三方库：[glad](https://github.com/Dav1dde/glad)，[glfw](https://github.com/glfw/glfw)，[imgui](https://github.com/ocornut/imgui)，[stb](https://github.com/nothings/stb)，[xsimd](https://github.com/xtensor-stack/xsimd)，[cpp-taskflow](https://github.com/cpp-taskflow/cpp-taskflow)，[RapidJSON](https://github.com/Tencent/rapidjson) 

本框架在这些依赖库上构建了两个项目

- PathTracer：用于完成路径追踪算法
- UEditor：用于编辑场景（目前功能不完善）

## 用户手册

启动项目 `HW9_UEditor` 

- WASDQE 移动相机，按住鼠标右键并移动鼠标可旋转镜头

- 按 `P` 可启动路径追踪，控制台调试窗口可看到进度，达到 `1` 后会将渲染结果保存为 [data/](data/)`rst.png` 

## 开发手册

- 完成 [src/PathTracer/PathTracer.cpp](src/PathTracer/PathTracer.cpp) 中 `PathTracer::Shade` 的 TODO 部分
- 在 [src/UEditor/GenScene.cpp](src/UEditor/GenScene.cpp) 中模仿 `GenScene0` 来增加场景（添加新的函数 `GenScene1`，并修改函数 `GenScene`，之后在 [src/UEditor/main.cpp](src/UEditor/main.cpp) 中修改 `GenScene(0)`（可改为控制台输入数字）

