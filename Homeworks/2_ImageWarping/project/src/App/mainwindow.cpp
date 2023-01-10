#include "mainwindow.h"
#include <QtWidgets>
#include <QImage>
#include <QPainter>
#include "ImageWidget.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);

	setGeometry(300, 150, 1280, 720);

	imagewidget_ = new ImageWidget();
	setCentralWidget(imagewidget_);

	CreateActions();
	CreateMenus();
	CreateToolBars();
	CreateStatusBar();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *e)
{

}

void MainWindow::paintEvent(QPaintEvent* paintevent)
{
	
}

void MainWindow::CreateActions()
{

	action_open_ = new QAction(QIcon(":/MainWindow/Resources/images/open.jpg"), tr("&Open..."), this);
	action_open_->setShortcuts(QKeySequence::Open);
	action_open_->setStatusTip(tr("Open an existing file"));
	connect(action_open_, &QAction::triggered, imagewidget_, &ImageWidget::Open);

	action_save_ = new QAction(QIcon(":/MainWindow/Resources/images/save.jpg"), tr("&Save"), this);
	action_save_->setShortcuts(QKeySequence::Save);
	action_save_->setStatusTip(tr("Save the document to disk"));
	connect(action_save_, &QAction::triggered, imagewidget_, &ImageWidget::Save);

	action_saveas_ = new QAction(tr("Save &As..."), this);
	action_saveas_->setShortcuts(QKeySequence::SaveAs);
	action_saveas_->setStatusTip(tr("Save the document under a new name"));
	connect(action_saveas_, &QAction::triggered, imagewidget_, &ImageWidget::SaveAs);

	action_invert_ = new QAction(tr("Inverse"), this);
	action_invert_->setStatusTip(tr("Invert all pixel value in the image"));
	connect(action_invert_, &QAction::triggered, imagewidget_, &ImageWidget::Invert);

	action_mirror_ = new QAction(tr("Mirror"), this);
	action_mirror_->setStatusTip(tr("Mirror image vertically or horizontally"));

	connect(action_mirror_, &QAction::triggered, [this]() {
		imagewidget_->Mirror(); // use default arguments
		});

	action_gray_ = new QAction(tr("Grayscale"), this);
	action_gray_->setStatusTip(tr("Gray-scale map"));
	connect(action_gray_, &QAction::triggered, imagewidget_, &ImageWidget::TurnGray);

	action_restore_ = new QAction(tr("Restore"), this);
	action_restore_->setStatusTip(tr("Show origin image"));
	connect(action_restore_, &QAction::triggered, imagewidget_, &ImageWidget::Restore);

	action_select_points_ = new QAction(tr("Select Points"), this);
	action_select_points_->setStatusTip(tr("Select Points"));
	action_select_points_->setCheckable(true);
	action_select_points_->setChecked(false);
	connect(action_select_points_, SIGNAL(triggered()), this,
		SLOT(ChangeSelectPointsMode()));

	action_undo_select_ = new QAction(tr("Undo"), this);
	action_undo_select_->setStatusTip(tr("Undo a selected point"));
	connect(action_undo_select_, &QAction::triggered, imagewidget_, &ImageWidget::UndoSelect);

	action_warp_ = new QAction(tr("Warp"), this);
	action_warp_->setStatusTip(tr("Warp Image"));
	connect(action_warp_, &QAction::triggered, imagewidget_, &ImageWidget::Warp);

	action_idw_warping_ = new QAction(tr("IDW warping"), this);
	action_idw_warping_->setStatusTip(tr("Inverse Distance Weighted"));
	action_idw_warping_->setCheckable(true);
	action_idw_warping_->setChecked(true);
	imagewidget_->SetWarpingMode(ImageWidget::IDW);

	action_rbf_warping_ = new QAction(tr("RBF warping"), this);
	action_rbf_warping_->setStatusTip(tr("Radial Basis Function"));





	//选择一个算法
	action_select_mode_ = new QActionGroup(this);
	action_select_mode_->addAction(action_idw_warping_);
	action_select_mode_->addAction(action_rbf_warping_);
	connect(action_select_mode_, SIGNAL(triggered(QAction*)),
		this, SLOT(ChangeWarpingMode(QAction*)));


	/*action_real_time_warping_mode_ = new QAction(tr("Realtime Warping"), this);
	action_real_time_warping_mode_->setStatusTip(tr("Realtime Warping"));
	action_real_time_warping_mode_->setCheckable(true);
	action_real_time_warping_mode_->setChecked(false);
	connect(action_real_time_warping_mode_, SIGNAL(triggered()), this,
		SLOT(ChangeRealtimeWarpingMode()));*/
}

void MainWindow::CreateMenus()
{
	menu_file_ = menuBar()->addMenu(tr("&File"));
	menu_file_->setStatusTip(tr("File menu"));
	menu_file_->addAction(action_open_);
	menu_file_->addAction(action_save_);
	menu_file_->addAction(action_saveas_);

	menu_edit_ = menuBar()->addMenu(tr("&Edit"));
	menu_edit_->setStatusTip(tr("Edit menu"));
	menu_edit_->addAction(action_invert_);
	menu_edit_->addAction(action_mirror_);
	menu_edit_->addAction(action_gray_);
	menu_edit_->addAction(action_restore_);

	menu_image_warping_ = menuBar()->addMenu(tr("Image Warping"));
	menu_image_warping_->setStatusTip(tr("Warp image"));
	menu_image_warping_->addAction(action_select_points_);
	menu_image_warping_->addAction(action_undo_select_);
	menu_image_warping_->addAction(action_warp_);
	//menu_image_warping_->addAction(action_real_time_warping_mode_);
	submenu_warping_method_ = menu_image_warping_->addMenu(tr("Warping Method"));
	submenu_warping_method_->addAction(action_idw_warping_);
	submenu_warping_method_->addAction(action_rbf_warping_);

	//menu_image_warping_ = menuBar()->addMenu(tr("Image Warping"));
	//menu_image_warping_->setStatusTip(tr("Warp image"));
	//menu_image_warping_->addAction(action_select_points_);
	//menu_image_warping_->addAction(action_undo_select_);
	//menu_image_warping_->addAction(action_do_warp_);
	//menu_image_warping_->addAction(action_real_time_warping_mode_);
	//submenu_warping_method_ = menu_image_warping_->addMenu(tr("Warping Method"));
	//submenu_warping_method_->addAction(action_use_idw_warping_);
	//submenu_warping_method_->addAction(action_use_rbf_warping_);
}

void MainWindow::CreateToolBars()
{
	toolbar_file_ = addToolBar(tr("File"));
	toolbar_file_->addAction(action_open_);
	toolbar_file_->addAction(action_save_);

	// Add separator in toolbar 
	toolbar_edit_ = addToolBar(tr("Edit"));
	toolbar_edit_->addAction(action_invert_);
	toolbar_edit_->addAction(action_mirror_);
	toolbar_edit_->addAction(action_gray_);
	toolbar_edit_->addAction(action_restore_);

	toolbar_image_warping_ = addToolBar(tr("Image Warping"));
	toolbar_image_warping_->addAction(action_select_points_);
	toolbar_image_warping_->addAction(action_undo_select_);
	toolbar_image_warping_->addAction(action_warp_);
	toolbar_image_warping_->addSeparator();
	toolbar_image_warping_->addAction(action_idw_warping_);
	toolbar_image_warping_->addAction(action_rbf_warping_);


}

void MainWindow::CreateStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

//TODO
//void MainWindow::ChangeRealtimeWarpingMode() {
//	if (action_real_time_warping_mode_) {
//		bool is_checked = action_real_time_warping_mode_->isChecked();
//		imagewidget_->SetRealTimeWarpingMode(is_checked);
//	}
//}

void MainWindow::ChangeSelectPointsMode() {
	if (action_select_points_) {
		bool is_checked = action_select_points_->isChecked();
		imagewidget_->SetSelectPointsMode(is_checked);
	}
}

void MainWindow::ChangeWarpingMode(QAction* a) {
	a->setCheckable(true);
	a->setChecked(true);
	if (action_idw_warping_->isChecked()) {
		imagewidget_->SetWarpingMode(ImageWidget::IDW);
	}
	else if (action_rbf_warping_->isChecked()) {
		imagewidget_->SetWarpingMode(ImageWidget::RBF);
	}
}
