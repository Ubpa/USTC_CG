# OpenCV 应用示例

## 配置说明

**配置 OpenCV** 

- 下载 OpenCV 4.2：[官网](https://opencv.org)，[下载链接](https://sourceforge.net/projects/opencvlibrary/files/4.2.0/opencv-4.2.0-vc14_vc15.exe/download) 
- 安装任意位置（路径不含中文）
- 设置系统变量
  - 将 `your-path-to-opencv/build/x64/vc15/bin` 加到系统变量 Path 中
  - 新建系统变量（用于 CMake 命令 `find_package(OpenCV REQUIRED)`）
    - 变量名：`OpenCV_DIR` 
    - 变量值：`your-path-to-opencv/build` 

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

## OpenCV 使用说明

### `IpImage` VS `Mat` 

如果你在网上搜索 OpenCV 的图像处理，会出现基于 `IpImage` 和 `Mat` 两种不用数据结构的处理方法。那么它们有什么区别呢？

`IplImage` 是 OpenCV 最早的处理图像的数据结构，但这种结构的使用更像 C 语言习惯，`Mat` 是 OpenCV 2.0 以后版本出现的更符合面向对象编程习惯的数据结构，而这正是 OpenCV 更新的方向。

OpenCV 为这两种数据结构提供了大量的图像 IO，图像处理接口，你可以随便选用。推荐你用 `Mat` 数据结构，因为这符合我们一直追求的面向对象编程习惯。我们的示例程序也是基于 `Mat` 数据结构的。

### namespace

在 VS 工程中，如果使用 `using namespace cv`，会遇到错误

```c++
error C2872: “ACCESS_MASK”: 不明确的符号
```

因此需要在每个使用 OpenCV 的地方都手动加上 `cv::`，以避免命名冲突

