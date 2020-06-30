#ifndef MINIDRAW_H
#define MINIDRAW_H

#include <ui_minidraw.h>
#include <viewwidget.h>

#include <QtWidgets/QMainWindow>
#include <qmessagebox.h>
#include <qtoolbar.h> 

class MiniDraw : public QMainWindow
{
	Q_OBJECT

public:
	MiniDraw(QWidget *parent = 0);
	~MiniDraw();

	QMenu *pMenuFigure;
	QMenu *pMenuEdit;
	QToolBar *pToolBar;

	QAction *Action_About;
	QAction *Action_Line;
	QAction *Action_Rect;
	QAction *Action_Ellip;
	QAction *Action_Polyg;
	QAction *Action_FreeHand;
	QAction* Action_Undo;
	QAction* Action_Color;
	QAction *Action_Width;

	void Creat_Menu();
	void Creat_ToolBar();
	void Creat_Action();

	void AboutBox();

private:
	Ui::MiniDrawClass ui;
	ViewWidget*	view_widget_;
};

#endif // MINIDRAW_H
