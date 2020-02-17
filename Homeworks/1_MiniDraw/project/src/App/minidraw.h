#ifndef MINIDRAW_H
#define MINIDRAW_H

#include <ui_minidraw.h>
#include <viewwidget.h>

#include <QtWidgets/QMainWindow>
#include <qmessagebox.h>

class MiniDraw : public QMainWindow
{
	Q_OBJECT

public:
	MiniDraw(QWidget *parent = 0);
	~MiniDraw();

	QMenu *pMenu;
	QToolBar *pToolBar;
	QAction *Action_About;
	QAction *Action_Line;
	QAction *Action_Rect;

	void Creat_Menu();
	void Creat_ToolBar();
	void Creat_Action();

	void AboutBox();

private:
	Ui::MiniDrawClass ui;
	ViewWidget*	view_widget_;
};

#endif // MINIDRAW_H
