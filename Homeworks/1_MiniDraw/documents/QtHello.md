# Qt Hello

本文教会大家如何利用 Qt + CMake 创建一个 [Hello World 级别的项目](hello/) 

> [hello 项目](hello/) 代码已经填充完整了，只要大概试着操作就行，不然就重复代码了

## 创建空项目

- 打开 Qt Creator
  - 菜单栏文件 -> 新建项目或文件->Qt Widgets Application -> Choose
    - 名称：hello
    - 路径：[hello/src/](hello/src/) 
    - 下一步直到完成，从而在 [hello/src/](hello/src/)hello/ 生成了一些文件
  - 关闭 Qt Creator
- 删除 [hello/src/](hello/src/)hello/ 下的 \*.pro 和 \*.user（Qt Creator 所用的工程文件，我们用 CMake，所以不需要），只留下 \*.cpp，\*.h，\*.qrc，\*.ui
- 添加文件 CMakeLists.txt，输入如下内容

```cmake
Ubpa_AddTarget(
  TEST
  QT
  MODE EXE
  LIB
    Qt5::Widgets
)
```

- CMake 三连（configure + generate + open project）

## 代码解释

### main.cpp

```c++
int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // 创建 Qt 应用
    MainWindow w; // 创建主窗口
    w.show(); // 显示主窗口

    return a.exec(); // 将应用交给 Qt 托管，进入事件循环
}
```

### mainwindow.h

```c++
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui; // 由 *.ui 文件定义，Qt 会自动生成相关 ui_*.h 文件
};
```

### mainwindow.cpp

```c++
#include "mainwindow.h"
#include "ui_mainwindow.h" // Qt 自动由 *.ui 生成的文件

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); // UI 初始化，就在 ui_mainwindow.h 里定义了，必须最先调用
}

MainWindow::~MainWindow()
{
    delete ui;
}
```

## 添加菜单，工具栏和动作

添加

- `QAction`：菜单栏，工具栏中的动作（按钮，选项）
- `QMenu`：菜单
- `QToolBar`：工具栏

一般我们将 `QAction` **同时**加载到 `QMenu`，`QToolBar` 中

### mainwindow.h

修改 `MainWindow` 

```c++
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    QAction* hello_world_action_; // 声明动作
    QMenu* main_menu_; // 声明菜单
    QToolBar* main_toolbar_; // 声明工具栏
    void CreateButtons(); // 声明函数
};
```

### mainwindow.cpp

添加函数定义

```c++
#include <QToolBar> // include QToolBar head file

void MainWindow::CreateButtons()
{
    // 创建动作，标签为Hello world
    //& 的作用：设置此动作的快捷键为标签字符串的首字母，此例为 'H'
    hello_world_action_ = new QAction(tr("&Hello world"), this);
    
    // 此句先不用理会
    // connect(hello_world_action_, &QAction::triggered, this, &MainWindow::HelloWorld);
    
    // 创建菜单，标签为Main
    main_menu_ = menuBar()->addMenu(tr("&Main"));
    
    // 将动作 Hello world 加载到 Main 菜单中
    main_menu_->addAction(hello_world_action_);
    
    // 创建工具栏，标签为 Main
    main_toolbar_ = addToolBar(tr("&Main"));
    
    // 将动作 Hello world 加载到 Main 工具栏中
    main_toolbar_->addAction(hello_world_action_);
}
```

调用该函数

```c++
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CreateButtons();
}
```

运行即可

## 信号 & 槽

### mainwindow.cpp

定义槽函数 `MainWindow::HelloWorld()`（需在类定义处添加函数声明 `private slots: void HelloWorld();`，并且在 mainwindow.cpp 内添加 `#include <qmessagebox.h>`）

```c++
void MainWindow::HelloWorld() {
    QMessageBox::about(this, tr("Hello world"),
        tr("Well done! Go ahead."));
}
```

连接动作和槽函数

```c++
void MainWindow::CreateButtons()
{
    hello_world_action_ = new QAction(tr("&Hello world"), this);
    
    // 使用 connect 函数将信号与槽连接起来
    // connect(信号发射方，信号，信号接收方，槽函数);
    connect(hello_world_action_, &QAction::triggered, this, &MainWindow::HelloWorld);
    
    main_menu_ = menuBar()->addMenu(tr("&Main"));
    main_menu_->addAction(hello_world_action_);
    main_toolbar_ = addToolBar(tr("&Main"));
    main_toolbar_->addAction(hello_world_action_);
}
```

## 回顾

通过 `QAction`, `QMenu`, `QToolBar` 构建应用程序框架 

通过 `connect` 函数实现 `signal slot` 机制

通常我们将用单独的函数去分别创建 `QAction`, `QMenu`, `QToolBar`（`CreateActions()`函数）

