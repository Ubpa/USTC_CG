#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class ViewWidget;
class QImage;
class QPainter;
class QRect;
class ImageWidget;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent* e);
	void paintEvent(QPaintEvent* paintevent);

private slots:
	void ChangeSelectPointsMode();
	//void ChangeRealtimeWarpingMode();
	void ChangeWarpingMode(QAction* a);

private:
	void CreateActions();
	void CreateMenus();
	void CreateToolBars();
	void CreateStatusBar();

private:
	Ui::MainWindowClass ui;

	QMenu* menu_file_;
	QMenu* menu_edit_;
	QMenu* menu_help_;
	QMenu* menu_image_warping_;
	QMenu* submenu_warping_method_;

	QToolBar* toolbar_file_;
	QToolBar* toolbar_edit_;
	QToolBar* toolbar_image_warping_;

	QAction* action_open_;
	QAction* action_save_;
	QAction* action_saveas_;
	QAction* action_invert_;
	QAction* action_mirror_;
	QAction* action_gray_;
	QAction* action_restore_;

	QAction* action_select_points_;
	QAction* action_undo_select_;
	QAction* action_warp_;

	QActionGroup* action_select_mode_;
	QAction* action_idw_warping_;
	QAction* action_rbf_warping_;

	//QAction* action_real_time_warping_mode_;


	ImageWidget* imagewidget_;
};

#endif // MAINWINDOW_H
