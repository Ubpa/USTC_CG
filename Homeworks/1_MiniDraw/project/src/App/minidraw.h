#ifndef MINIDRAW_H
#define MINIDRAW_H

#include <ui_minidraw.h>
#include <viewwidget.h>

#include <QtWidgets/QMainWindow>
#include <qmessagebox.h>

class MiniDraw : public QMainWindow {
	Q_OBJECT

public:
	MiniDraw(QWidget* parent = 0);
	~MiniDraw();

	QMenu* pMenu;//主菜单栏
	QToolBar* pToolBar;//功能栏
	QAction* Action_About;//窗口的动作，点击后，触发事件
	QAction* Action_Undo;
	QAction* Action_Clean;
	QAction* Action_Line;
	QAction* Action_Rect;
	QAction* Action_Ellip;
	QAction* Action_Poly;
	QAction* Action_Free;

	void Creat_Menu();//创建菜单
	void Creat_ToolBar();//创建功能栏
	void Creat_Action();//对动作触发响应

	void AboutBox();

private:
	Ui::MiniDrawClass ui;
	ViewWidget* view_widget_;
};

#endif // MINIDRAW_H
