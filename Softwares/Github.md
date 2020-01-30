# Github

> [官网](https://desktop.github.com/)，[下载链接](https://central.github.com/deployments/desktop/desktop/latest/win32)，[官方文档](https://help.github.com/en/desktop) 

git 是很好的代码版本管理工具，github 是 git 一个托管平台（把本地的代码历史上传到云端）

因此需要学习两个内容：git 和 github

## Git

教程

- [廖雪峰 Git 教程](https://www.liaoxuefeng.com/wiki/896043488029600) 
- [Git教程 | 菜鸟教程](https://www.runoob.com/git/git-tutorial.html) 

重点学会一下命令

- `git log`：查看历史版本信息
- `git reset --soft <hash-code>`：跳转到任意版本，保留代码不变
- `git reset --hard <hash-code>`：跳转到任意版本，代码改变，谨慎使用
- `git push origin master -f`：当将本地代码推到 github 后还想撤销时，先本地回退到相应版本，然后用此命令将本地版本强推到 github 上

## Github

官方有一个教程：[https://lab.github.com/](https://lab.github.com/) 

软件 Github Desktop 就可以方便使用 GitHub，操作很简单，是 git 功能的简单封装，一般操作可以在上边完成，复杂操作就使用 Git 的命令行，可查阅文档简单学习一下 [https://help.github.com/en/desktop](https://help.github.com/en/desktop) 

