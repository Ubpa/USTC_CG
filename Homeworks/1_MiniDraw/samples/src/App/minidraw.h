#ifndef MINIDRAW_H
#define MINIDRAW_H

#include <QtWidgets/QMainWindow>
#include "ui_minidraw.h"
#include <vector>
#include "viewwidget.h"

class MiniDraw : public QMainWindow {
    Q_OBJECT

public:
    MiniDraw(QWidget* parent = 0);
    ~MiniDraw();

private:
    Ui::MiniDrawClass ui;
    QMenu* menu;
    QToolBar* toolBar;
    std::vector<QAction*> actionVec;
    ViewWidget* view_widget;

    void init();
};

#endif // MINIDRAW_H
