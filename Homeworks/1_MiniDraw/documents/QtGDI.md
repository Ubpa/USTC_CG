# QtGDI

> 本次作业的 [project](../project/) 即为下边所述的代码
>
> 可**自行重复一遍**，本文主要描述那些文件是如何创建和编写的
>
> 需要参考 [project](../project/) 来理解本文

[TOC]

## 1. 鼠标画线

Qt GUI 绘图可以在 `QWidget` 控件中实现，我们将自定义继承于 `QWidget` 的子类 `ViewWidget` 类去实现绘图功能

运用 [QtHello](QtHello.md) 的方法创建窗体 `MiniDraw`，包含

- 菜单：Figure Tool
- 工具栏：About，Line，Rectangle
- About，Line，Rectangle 相应动作（槽函数为空即可，后边添加）

### 1.1 创建绘图区域

#### 1.1.1 创建 `ViewWidget` 类

- 打开 Qt Creator -> 新建文件或项目 -> Qt Widgets Application -> Choose...
- 弹出窗口，名称随意，位置自选（待会需要访问该位置），下一步，基类选 `QWidget`，类名为 `ViewWidget`，下一步，完成
- 找到刚才的位置，将其中的 `viewwidget.cpp`，`viewwidget.h`，`viewwidget.ui` 拷贝到[项目源码文件处（同于 MiniDraw）](../project/src/App/) 
- CMake 三连（configure，generate，open project）

#### 1.1.2 将 `ViewWidget` 添加到 `MiniDraw` 中

**minidraw.h** 

```c++
#include "viewwidget.h"

class MiniDraw : public QMainWindow
{
	//...
private:
	ViewWidget*		view_widget_; // 声明 ViewWidget 指针
};
```

**minidraw.cpp** 

```c++
// 实例代码中将绘图控件的实例化放在 Init 函数中
// Init 函数将在 MiniDraw 的构造函数中被调用
void MiniDraw::Init()
{
	//...
	view_widget_ = new ViewWidget(); // 实例化 ViewWidget 控件窗口
	//...
	setCentralWidget(view_widget_); // 将 ViewWidget 控件设置为主窗口的中心位置
	//...
}
```

### 1.2 绘制功能实现

我们从 QWidget 继承后，通过添加事件响应来实现绘制功能

给 `ViewWidget` 添加响应事件和一些辅助变量

```c++
class ViewWidget : public QWidget
{
	// ...
public:
	void mousePressEvent(QMouseEvent *event); // 鼠标击发响应函数（左右键，单双击）
	void mouseMoveEvent(QMouseEvent *event); // 鼠标移动响应函数（其一个重要性质在文档最后有详述）
	void mouseReleaseEvent(QMouseEvent *event); // 鼠标释放响应函数（左右键，单双击）
    void paintEvent(QPaintEvent *); // Qt 所有的绘制都只能在此函数中完成
private:
	bool draw_status_; // 当前绘制状态，true 为绘制当前鼠标拖动的图元，false 为不绘制
	QPoint start_point_; // 当前图元的起始点
	QPoint end_point_; // 当前图元的终止点
};
```

实现如下

```c++
ViewWidget::ViewWidget(QWidget *parent)
	: QWidget(parent)
{
    // ...
	draw_status_ = false; // 设置初始绘制状态为 – 不绘制
}

void ViewWidget::mousePressEvent(QMouseEvent *event)
{
	if (Qt::LeftButton == event->button()) // 判断是否是鼠标左击
	{
		draw_status_ = true; // 设置绘制状态为 – 绘制
		start_point_ = end_point_ = event->pos(); // 将图元初始点设置为当前鼠标击发点
	}
}

void ViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (draw_status_) // 判断当前绘制状态
	{
		end_point_ = event->pos(); // 若为真，则设置图元终止点位鼠标当前位置
	}
}

void ViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	draw_status_ = false; // 设置绘制状态为 – 不绘制
}

void ViewWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this); // 定义painter在this指向的控件（此例为ViewWidget）中绘图
	painter.drawLine(start_point_, end_point_); // 绘制线段
	painter.end(); // 结束绘图
	update(); // 更新窗口
}
```

至此 MiniDraw 有了绘制功能，但只能画一条线，因为**没有存储数据** 

## 2. 存储数据

我们将要绘制的多条线存储起来，然后在绘制事件里重画所有的线即可

> 这个思路跟游戏渲染是一致的，每帧画面都全部重新画，而不会去考虑下一画面如何重复利用上一帧画面

我们设计一个线段类

```c++
class Line
{
public:
	Line(void);
	Line(int start_point_x, int start_point_y, int end_point_x, int end_point_y)
	{
		start_point_x_ = start_point_x;
		start_point_y_ = start_point_y;
		end_point_x_ = end_point_x;
		end_point_y_ = end_point_y;
	}
	~Line(void);
	
public:
	void Draw(QPainter &paint)
	{
		paint.drawLine(start_point_x_, start_point_y_, end_point_x_, end_point_y_);
	}

private:
	int	start_point_x_, start_point_y_, end_point_x_, end_point_y_;
};

```

然后在 ViewWidget 中用 `std::vector` 保存它

**viewwidget.h** 

```c++
#include<vector>
using namespace std;

class ViewWidget : public QWidget
{
    // ...
private:
	vector<Line*> line_array_;
}
```

**viewwidget.cpp** 

```c++
void ViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	Line* current_line_ = NULL;
	current_line_ = new Line(start_point_.rx(), start_point_.ry(), end_point_.rx(), end_point_.ry());
		line_array_.push_back(current_line_);
	draw_status_ = false;
}

void ViewWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
    // 重画所有线段
	for (size_t i = 0; i < line_array_.size(); i++)
	{
		line_array_[i]->Draw(painter);
	}
	painter.end();
	painter.drawLine(start_point_, end_point_);
}
```

这样就可以绘制多条直线了

## 3. 内存泄漏

在 main 函数加入 

```c++
_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
```

启动**内存检测** 

会在输出窗口中看到

```
Detected memory leaks!
Dumping objects ->
{229791} normal block at 0x000002B54495E900, 400 bytes long.
 Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD 
Object dump complete.
```

因为 `new` 了没有 `delete`，修改析构函数

```c++
ViewWidget::~ViewWidget()
{
	for (size_t i = 0; i < line_array_.size(); i++)
	{
		if (line_array_[i])
		{
			delete line_array_[i];
			line_array_[i] = NULL;
		}
	}
}
```

Qt 的资源：QAction* QMenu* 等将由 Qt 自己回收，不需要用户自己释放，即使是用户自己为这些资源申请空间也不需要手动释放空间

非 Qt 资源： 需由用户自己申请空间，自己释放空间

## 4. 更多图形元素

不同类型图形元素如何存储？

如何组织它们之间的关系？

椭圆类如下

```c++
class Ellipse
{
public:
	Ellipse (void);
	Ellipse (int start_point_x, int start_point_y, int end_point_x, int end_point_y)
	{
		start_point_x_ = start_point_x;
		start_point_y_ = start_point_y;
		end_point_x_ = end_point_x;
		end_point_y_ = end_point_y;
	}
	~Ellipse (void);
	
public:
	void Draw(QPainter &paint)
	{
		paint.drawEllipse (start_point_x_, start_point_y_, end_point_x_ - start_point_x_, end_point_y_ - start_point_y_);
	}

private:
	int	start_point_x_, start_point_y_, end_point_x_, end_point_y_;
};
```

然后 ViewWidget 里添加

```c++
vector<Line* >		line_array_;
vector<Ellipse* >		ellipse_array_;
```

然而这是好做法吗？

## 5. 继承与多态

C++ 继承与多态概念自行学习，本文只介绍应用

考虑图元的性质，虽然形状不同，但是都可以被画

此例中可建立父类 `Figure`，虚方法 `Draw` 

图元 `Line`，`Rectangle`， `Ellipse` 继承于 `Figure`，实现 `Draw` 方法

使用父类指针指向子类实例，调用 `Draw` 函数，实现多态

### 5.1 继承

```c++
class Figure
{
public:
	virtual void Draw(QPainter &paint);
    // 父类的析构函数必须是 virtual 的
    // 否则当用父类指针指向子类的实例并删除该实例时，将只会调用父类的析构函数
    // 而不调用子类的析构函数。会造成内存泄漏
	virtual ~Figure();
}

class Line : public Figure { /*...*/ };

class Ellipse : public Figure { /*...*/ };
```

`ViewWidget` 用父类指针数组存储数据

```c++
class ViewWidget : public QWidget
{
	vector<Figure* >  figure_array_;
	FigureType  figure_type_;
}

enum FigureType // 推荐用枚举类代表图元类型
{
	kDefault = 0,
	kLine = 1,
	kRectangle = 2,
	kEllipse = 3,
	kPolygon = 4,
};
```

```c++
// 修改 mouseReleaseEvent 函数，使用父类指针指向子类实例
void ViewWidget::mouseReleaseEvent(QMouseEvent *event) 
{
	Figure* current_figure_ = NULL; // 声明父类指针

    current_figure_ = start_pointnew Line(start_point_.rx(), _.ry(), end_point_.rx(), end_point_.ry()); // 实例化子类

    Figure_array_.push_back(current_figure_);
    draw_status_ = false;
}

void ViewWidget::set_figure_type_to_line()
{
	figure_type_ = kLine;
}

void ViewWidget::set_figure_type_to_ellipse()
{
	figure_type_ = kEllipse;
}

void ViewWidget::mouseReleaseEvent(QMouseEvent *event)
{	
	Figure* current_figure_ = NULL;

	switch (figure_type_)
	{
	case kDefault:
		break;
	case kLine:
		current_figure_ = new Line(start_point_.rx(), start_point_.ry(), end_point_.rx(), end_point_.ry());
		figure_array_.push_back(current_figure_);
		draw_status_ = false;
		break;
	case kEllipse:
		current_figure_ = new Ellipse(start_point_.rx(), start_point_.ry(), end_point_.rx(), end_point_.ry());
		figure_array_.push_back(current_figure_);
		draw_status_ = false;
		break;
	default:
		break;
	}

	current_figure_ = NULL;
}
```

### 5.2 多态

```c++
void ViewWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	for (size_t i = 0; i < figure_array_.size(); i++)
	{
		figure_array_[i]->Draw(painter);
	}

	if (draw_status_)
	{
		switch (figure_type_)
		{
		case kDefault:
			break;
		case kLine:
			painter.drawLine(start_point_, end_point_);
			break;
		case kEllipse:
			painter.drawEllipse(start_point_.rx(), start_point_.ry(), end_point_.rx() - start_point_.rx(), end_point_.ry() - start_point_.ry());
			break;
		}
	}

	update();
}
```

## 6. 其他注意事项

`setMouseTracking(true);` 将设置鼠标移动响应函数当鼠标在窗口内部移动时就触发，而不必须鼠标键出于击发状态

Qt 的内存泄漏检测必须在 `main` 函数里面加入相应的语句

类的声明和实现最好分离，例如 `Figure.h/cpp` 

