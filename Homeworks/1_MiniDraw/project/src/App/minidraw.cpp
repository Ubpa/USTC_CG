#include "minidraw.h"

MiniDraw::MiniDraw(QWidget* parent)//初始化函数，继承QMainWindow类
	: QMainWindow(parent) {
	ui.setupUi(this);//启动
	view_widget_ = new ViewWidget();//创建一个ViewWidget类的对象
	Creat_Action();
	Creat_ToolBar();
	Creat_Menu();

	setCentralWidget(view_widget_);
}

/*
QAction类作为动作。顾名思义，这个类就是代表了窗口的一个“动作”，
这个动作可能显示在菜单，作为一个菜单项，当用户点击该菜单项，对用户的点击做出响应；
也可能在工具栏，作为一个工具栏按钮，用户点击这个按钮就可以执行相应的操作。
无论是出现在菜单栏还是工具栏，用户选择之后，所执行的动作应该都是一样的。
Qt 并没有专门的菜单项类，只是使用一个QAction类，抽象出公共的动作。
当我们把QAction对象添加到菜单，就显示成一个菜单项，添加到工具栏，就显示成一个工具按钮。
用户可以通过点击菜单项、点击工具栏按钮、点击快捷键来激活这个动作。
*/
void MiniDraw::Creat_Action() {
	//输入参数包括：text ;parent QObject
	//text是这个动作的文本描述，用来显示文本信息，比如在菜单中的文本；
	//parent是指明这个QAction的父组件，当这个父组件被销毁时，比如delete或者由系统自动销毁，与其相关联的这个QAction也会自动被销毁。
	Action_About = new QAction(tr("&About"), this);
	
	//Qt中著名的槽与信号机制
	//connect(sender, signal, receiver, slot);
	/*connect()一般会使用前面四个参数，
	第一个是发出信号的对象，
	第二个是发送对象发出的信号，
	第三个是接收信号的对象，
	第四个是接收对象在接收到信号之后所需要调用的函数。
	也就是说，当 sender 发出了 signal 信号之后，会自动调用 receiver的 slot 函数。
	*/
	connect(Action_About, &QAction::triggered, this, &MiniDraw::AboutBox);

	Action_Line = new QAction(tr("&Line"), this);
	connect(Action_Line, SIGNAL(triggered()), view_widget_, SLOT(setLine()));

	Action_Rect = new QAction(tr("&Rect"), this);
	connect(Action_Rect, &QAction::triggered, view_widget_, &ViewWidget::setRect);

	Action_Ellip = new QAction(tr("&Ellipse"), this);
	connect(Action_Ellip, &QAction::triggered, view_widget_, &ViewWidget::setEllip);

	Action_Poly = new QAction(tr("&Polygeon"), this);
	connect(Action_Poly, &QAction::triggered, view_widget_, &ViewWidget::setPoly);
	
	Action_Free = new QAction(tr("&Pen"), this);
	connect(Action_Free, &QAction::triggered, view_widget_, &ViewWidget::setFree);

	Action_Undo = new QAction(tr("&Undo"), this);
	connect(Action_Undo, &QAction::triggered, view_widget_, &ViewWidget::Undo);

	Action_Clean = new QAction(tr("&Clean"), this);
	connect(Action_Clean, &QAction::triggered, view_widget_, &ViewWidget::Clean);
}

void MiniDraw::Creat_ToolBar() {
	pToolBar = addToolBar(tr("&Main"));
	pToolBar->addAction(Action_About);
	pToolBar->addAction(Action_Clean);
	pToolBar->addAction(Action_Undo);
	pToolBar->addAction(Action_Line);
	pToolBar->addAction(Action_Rect);
	pToolBar->addAction(Action_Ellip);
	pToolBar->addAction(Action_Poly);
	pToolBar->addAction(Action_Free);
}

void MiniDraw::Creat_Menu() {
	pMenu = menuBar()->addMenu(tr("&Figure Tool"));
	pMenu->addAction(Action_About);
	pMenu->addAction(Action_Undo);
	pMenu->addAction(Action_Clean);
	pMenu->addAction(Action_Line);
	pMenu->addAction(Action_Rect);
	pMenu->addAction(Action_Ellip);
	pMenu->addAction(Action_Poly);
	pMenu->addAction(Action_Free);
}

void MiniDraw::AboutBox() {
	QMessageBox::about(this, tr("About"), tr("MiniDraw"));
}

MiniDraw::~MiniDraw() {
	delete view_widget_;//防止内存泄露，与new搭配，释放堆空间
}
