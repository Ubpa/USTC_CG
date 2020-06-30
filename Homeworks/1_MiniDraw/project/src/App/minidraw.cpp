#include "minidraw.h"

MiniDraw::MiniDraw(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	view_widget_ = new ViewWidget();
	setCentralWidget(view_widget_);

	Creat_Action();
	Creat_ToolBar();
	Creat_Menu();
	this->setStyleSheet("background-color:white;");
}

void MiniDraw::Creat_Action()
{
	Action_About = new QAction(tr("&About"), this);
	connect(Action_About, &QAction::triggered, this, &MiniDraw::AboutBox);
	Action_Line = new QAction(tr("&Line"), this);
	connect(Action_Line, SIGNAL(triggered()), view_widget_, SLOT(setLine()));

	Action_Rect = new QAction(tr("&Rect"), this);
	connect(Action_Rect, &QAction::triggered, view_widget_, &ViewWidget::setRect);

	Action_Ellip = new QAction(tr("&Ellip"), this);
	connect(Action_Ellip, &QAction::triggered, view_widget_, &ViewWidget::setEllip);

	Action_Polyg = new QAction(tr("&Polyg"), this);
	connect(Action_Polyg, &QAction::triggered, view_widget_, &ViewWidget::setPolyg);
	
	Action_FreeHand = new QAction(tr("&FreeHand"), this);
	connect(Action_FreeHand, &QAction::triggered, view_widget_, &ViewWidget::setFreeHand);

	Action_Undo = new QAction(tr("&Undo"), this);
	connect(Action_Undo, &QAction::triggered, view_widget_, &ViewWidget::Undo);

	Action_Color = new QAction(tr("&Color"), this);
	connect(Action_Color, &QAction::triggered, view_widget_, &ViewWidget::setColor);
	
	Action_Width = new QAction(tr("&Width"), this);
	connect(Action_Width, &QAction::triggered, view_widget_, &ViewWidget::setWidth);
}

void MiniDraw::Creat_ToolBar()
{
	pToolBar = addToolBar(tr("&Main"));	pToolBar->addAction(Action_About);	pToolBar->addAction(Action_Line);	pToolBar->addAction(Action_Rect);
	pToolBar->addAction(Action_Ellip);
	pToolBar->addAction(Action_Polyg);
	pToolBar->addAction(Action_FreeHand);
}

void MiniDraw::Creat_Menu()
{
	pMenuFigure = menuBar()->addMenu(tr("&Figure"));	pMenuFigure->addAction(Action_About);	pMenuFigure->addAction(Action_Line);	pMenuFigure->addAction(Action_Rect);
	pMenuFigure->addAction(Action_Ellip);
	pMenuFigure->addAction(Action_Polyg);
	pMenuFigure->addAction(Action_FreeHand);

	pMenuEdit = menuBar()->addMenu(tr("&Edit"));
	pMenuEdit->addAction(Action_Undo);
	pMenuEdit->addAction(Action_Color);
	pMenuEdit->addAction(Action_Width);
}

void MiniDraw::AboutBox()
{
	QMessageBox::about(this, tr("MiniDraw"), tr("MiniDraw by Adven-00"));
}

MiniDraw::~MiniDraw()
{

}
