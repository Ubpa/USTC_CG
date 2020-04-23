# 作业项目

## 构建说明

### 环境

- vs2019
- python 3.7
- CMake 3.16.3 及以上

### 步骤

- CMake-GUI
  - configue（期间会下载大量文件，可能由于网络问题导致失败，重新点 configure，直到无错成功
  - **确保 configure 无错后**，找到 CMAKE_INSTALL_PREFIX，设置其他位置，以文件夹 `Ubpa` 结尾，如 `D:/Program_Files/Ubpa` 
  - Generate
  - Open Project，打开 vs2019
- VS 2019
  - （可选）找到项目 `INSTALL`，右键执行，安装 `Ubpa` 系列库，编译时间较久
  - 找到项目 `HW9/HW9_UEditor`，右键设为启动项，并启动，编译时间较久

## 用户手册

按 `P` 可启动路径追踪，控制台调试窗口可看到进度，达到 `1` 后会将渲染结果保存为 [data/](data/)`rst.png` 

## 开发手册

完成 [src/PathTracer/PathTracer.cpp](src/PathTracer/PathTracer.cpp) 中 `PathTracer::Shade` 的 TODO 部分

