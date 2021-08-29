# 作业项目

## 安装说明

cmake configure 后，执行以下操作

- 注释掉 [build/_deps/utemplate-src/include/UTemplate/Num.h](build/_deps/utemplate-src/include/UTemplate/Num.h) 的 29-46 行
- 下载 [CMakeLists.txt](https://github.com/Ubpa/UGM/blob/0.5.8/include/UGM/Interfaces/IArray/_deps/CMakeLists.txt) 到 [build/_deps/ugm-src/include/UGM/Interfaces/IArray](build/_deps/ugm-src/include/UGM/Interfaces/IArray)/_deps/CMakeLists.txt
- 下载 [CMakeLists.txt](https://github.com/Ubpa/UECS/blob/0.12.0/include/UECS/detail/_deps/CMakeLists.txt) 到 [build/_deps/uecs-src/include/UECS/detail](build/_deps/uecs-src/include/UECS/detail)/_deps/CMakeLists.txt
- 打开目录 [build/_deps/ugl-src/config/enums/tools](build/_deps/ugl-src/config/enums/tools) 双击运行 `src2enum.py` 

然后重新 cmake configure generate

## Change Log

- v0.0.7
  - 修复 `SampleBRDF()` 在 `pd==0` 时引发的 bug
  - `SampleLight()` 给面光源添加了**多重重要性采样**，~~从而修复了金色噪点问题~~ 
  - `CMakeLists.txt` 大幅优化
- v0.0.6
  - `Inspector` 添加 `range` 
  - `stdBRDF` 在入/出射方向在表面下方时 `PDF == 0` 
- v0.0.5
  - 修复 `EnvLight::PDF(<vecf3>)` 的 bug，现使用 `EnvLight::PDF(<vecf3>,<normalf>)` 
  - 添加 ArcBall 功能（[ArcBall.h](src/UEditor/Cmpt/ArcBall.h)/[cpp](src/UEditor/Cmpt/ArcBall.cpp)）
  - 丰富 `Inspector` 面板功能 （[Inspector.h](src/UEditor/Cmpt/Inspector.h)/[cpp](src/UEditor/Cmpt/Inspector.cpp)）
  - 底层依赖 [UECS](https://github.com/Ubpa/UECS) 升级至 v0.6.0
  - 程序运行结束前会将场景的序列化结果输出到标准输出（默认控制台窗口）
- v0.0.4
  - 将 `IntersectorVisibility` 和 `IntersectorClosest` 改为单例类，使用 `IntersectorClosest::Instance()` 和 `IntersectorClosest::Instance()` 来获取类实例，`PathTracer` 的接口相应变化
  - 提供 obj 载入功能（在 [GenScene.cpp](src/UEditor/GenScene.cpp) 中查看相应变化）
  - 提供场景 json 文件读取功能（在 [GenScene.h](src/UEditor/GenScene.h)/[cpp](src/UEditor/GenScene.cpp) 中查看相应变化）
  - 默认载入 json 文件（在 [main.cpp](src/UEditor/main.cpp) 中查看相应变化）
  - `GenScene0` 新增载入 [cube.obj](data/models/cube.obj) 
  - 修复 `EnvLight::Sample(normalf)` 偶发的 bug（`t` 与 `n` 共线）

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
  - （可选）找到项目 `INSTALL`，右键执行，安装 `Ubpa` 系列库（debug 和 release 都要安装一遍）（编译时间较久），然后重新 cmake
  - 找到项目 `HW9/HW9_UEditor`，右键设为启动项，并启动（编译时间较久）

## 框架说明

本框架涉及的依赖库如下

- [UCMake](https://github.com/Ubpa/UCMake) v0.5.0：CMake 工具库
- [UTemplate](https://github.com/Ubpa/UTemplate) v0.3.4：C++ Template 库
- [UDP](https://github.com/Ubpa/UDP) v0.4.2：设计模式
- [UBL](https://github.com/Ubpa/UBL) v0.1.4：基本库
- [UGM](https://github.com/Ubpa/UGM) v0.5.4：图形数学库
- [UECS](https://github.com/Ubpa/UECS) v0.6.1：Entity-Component-System
- [UScene](https://github.com/Ubpa/UScene) v0.5.5：场景库
- [UGL](https://github.com/Ubpa/UGL) v0.2.4：C++ OpenGL Wrapper
- [URTR](https://github.com/Ubpa/URTR) v0.0.10：实时渲染器
- [UEngine](https://github.com/Ubpa/UEngine) v0.0.7：引擎（实时渲染器 + ECS + imgui）

> 第三方库：[glad](https://github.com/Dav1dde/glad)，[glfw](https://github.com/glfw/glfw)，[imgui](https://github.com/ocornut/imgui)，[stb](https://github.com/nothings/stb)，[xsimd](https://github.com/xtensor-stack/xsimd)，[cpp-taskflow](https://github.com/cpp-taskflow/cpp-taskflow)，[RapidJSON](https://github.com/Tencent/rapidjson)，[tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) 

本框架在这些依赖库上构建了两个项目

- PathTracer：用于完成路径追踪算法
- UEditor：用于编辑场景（功能逐步完善中）

## 用户手册

启动项目 `HW9_UEditor` 

- WASDQE 移动相机，按住鼠标右键并移动鼠标可旋转镜头
- 按 `P` 可启动路径追踪，控制台调试窗口可看到进度，达到 `1` 后会将渲染结果保存为 [data/](data/)`rst.png` （此部分逻辑位于 [src/UEditor/Cmpt/PathTracerAgency.h](src/UEditor/Cmpt/PathTracerAgency.h) 和 [src/UEditor/Cmpt/PathTracerAgency.cpp](src/UEditor/Cmpt/PathTracerAgency.cpp)）
- 在 `Hierarchy` 中找到 `scene` 下的 `camera_obj`，点击后在 `Inspector` 的 `PathTracerAgency` 中可调节离线渲染的 `spp` 和 `width` 
- `HW9_UEditor.exe` 可接受一个命令行参数——场景 json 文件路径，如未提供，则默认为 [../data/models/uscene.json](data/models/uscene.json)。在 VS2019 中，可右键项目 -> 属性 -> 调试 -> 命令参数中填写命令行参数，如 `../data/models/uscene.json`，也可在编译后在 `bin/` 下用命令行执行 `HW9_UEditor.exe` 
- 在 `Hierarchy` 中选择一个 `SObj`（**S**cene **Obj**ect） 后，按住 `alt` 键可以启动 ArcBall 功能，鼠标点击左键开始旋转物体，释放鼠标左键结束旋转，可在 `Editor Scene` 中找到 `SObj : ui` 下的 `ArcBall` 组件配置方向反转和旋转速度。
- 在 `Hierarchy` 中选择一个 `SObj`（**S**cene **Obj**ect） 后，`Inspector` 可查看其组件，部分组件的部分域可修改（如 `Cmpt::Position::value`）
- 程序运行结束后，控制台调试窗口会输出场景的序列化结果

## 开发手册

- 完成 [src/PathTracer/PathTracer.cpp](src/PathTracer/PathTracer.cpp) 中 `PathTracer::Shade` 的 TODO 部分
- 在 `PathTracer::PathTracer` 中构建环境贴图的别名表，然后在 `PathTracer::SampleLight` 中实现 `EnvLight::Sample()` 和 `EnvLight::PDF()` 的等价函数，用上别名法
- 在 [src/UEditor/GenScene.cpp](src/UEditor/GenScene.cpp) 中模仿 `GenScene0` 来增加场景（添加新的函数 `GenScene1`，并修改函数 `GenScene`，之后在 [src/UEditor/main.cpp](src/UEditor/main.cpp) 中修改 `GenScene(0)`（可改为控制台输入数字）
- 模仿 `uscene.json` 来创建场景

