# OpenCV 示例

## 使用说明

**配置 OpenCV** 

- 下载 OpenCV 4.2：[官网](https://opencv.org)，[下载链接](https://sourceforge.net/projects/opencvlibrary/files/4.2.0/opencv-4.2.0-vc14_vc15.exe/download) 
- 安装任意位置（路径不含中文）
- 设置系统变量
  - 将 `your-path-to-opencv/build/x64/vc15/bin` 加到系统变量 Path 中
  - 新建系统变量（用于 CMake 命令 `find_package(OpenCV REQUIRED)`）
    - 变量名：`OpenCV_DIR` 
    - 变量值：`your-path-to-opencv/build/x64/vc15/lib` 

**CMake** 

- 配置好环境变量后启动 CMake-GUI（若已启动，则重启 CMake-GUI）
- Configure-Generate-Open Project（注意选 `x64`）

> 若 OpenCV 配置正确，可在 CMake-GUI 的 log 里找到信息
>
> ```
> OpenCV ARCH: x64
> OpenCV RUNTIME: vc15
> OpenCV STATIC: OFF
> Found OpenCV 4.2.0 in your-path-to-opencv/build/x64/vc15/lib
> ```

