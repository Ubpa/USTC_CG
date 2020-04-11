# 项目说明

## 依赖配置

- 用 CMake 安装 [glfw3](https://github.com/glfw/glfw) 
- 下载 [stb_image.h](https://raw.githubusercontent.com/nothings/stb/master/stb_image.h) 并置于 [include/_deps/](include/_deps/) 
- 通过 GLAD [在线服务](http://glad.dav1d.de/) 获取 GLAD 相关文件
  - API -> gl -> Version 3.3
  - Profile -> Core
  - options -> Generate a loader (defualt)
  - **Generate** -> 下载 glad.zip
  - 将  glad.zip 的 include 中的 glad/ 和 KHR/ 放到该项目的 [include/_deps/](include/_deps/) 中
  - 将 glad.zip 的 glad.c 放到该项目的 [src/_deps/glad/](src/_deps/glad/) 中

