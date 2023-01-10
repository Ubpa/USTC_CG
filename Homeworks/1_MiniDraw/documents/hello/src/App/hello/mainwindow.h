#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    Ui::MainWindow *ui;// 由 *.ui 文件定义，Qt 会自动生成相关 ui_*.h 文件

    QAction* hello_world_action_;
    QMenu* main_menu_;
    QToolBar* main_toolbar_;
    void CreateButtons();

private slots:
    void HelloWorld();
};

#endif // MAINWINDOW_H
