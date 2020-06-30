#include "minidraw.h"

MiniDraw::MiniDraw(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	view_widget_ = new ViewWidget();
	Creat_Action();
	Creat_ToolBar();
	Creat_Menu();
	setCentralWidget(view_widget_);
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
	pMenu = menuBar()->addMenu(tr("&Figure Tool"));	pMenu->addAction(Action_About);	pMenu->addAction(Action_Line);	pMenu->addAction(Action_Rect);
	pMenu->addAction(Action_Ellip);
	pMenu->addAction(Action_Polyg);
	pMenu->addAction(Action_FreeHand);
}

void MiniDraw::AboutBox()
{
	QMessageBox::about(this, tr("MiniDraw"), tr("MiniDraw by Adven-00"));
}

MiniDraw::~MiniDraw()
{

}
