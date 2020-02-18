# QT 简略使用说明

## 安装

- 打开[官网](https://www.qt.io/) 
- 点击右上角的 "Download. Try. Buy"
- 点击 "Downloads for open sources users" 的 "Go open source"（此时需要登录）得到下载器
- 打开下载器，选择 Qt 5.12.2 的 MSVC 2017 64-bit（可以用于 VS2019）
- 将 `YOUR_PATH_TO/Qt5/msvc 2017 64/bin` 加入到环境变量 `Path` 中（[配置方法](https://jingyan.baidu.com/article/7908e85ce8ddd3af491ad27c.html)）
- VS 2019 菜单栏->扩展->管理扩展，搜索 Qt，找到 Qt Visual Studio Tools 并安装（需退出 VS），该工具以便在 VS 中打开 *.qrc 和 *.ui 文件时可以自动打开 Qt Resource Editor 和 Qt Designer
- 安装 Qt Visual Studio Tools 成功后，重启 VS，菜单栏->扩展->Qt VS Tool->Qt Options，在弹出的窗口中
  - Qt Versions 下选择 Add，添加 path 为 `YOUR_PATH_TO/Qt5/msvc 2017 64` 
  - Qt Default Settings 下 Ask before checkout files 选择 **False** 

## 入门

Qt 的使用资料众多。本课程需要 Qt 相关技能不多，主要靠模仿和查阅官方文档的方式解决具体问题

建议可以花 1-2 天的时间跟一下教程，如 [跟小豆君学Qt](https://zhuanlan.zhihu.com/c_119081535) 

作业 1 包含了[入门教程](../Homeworks/1_MiniDraw/documents/) 

