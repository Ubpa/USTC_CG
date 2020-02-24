#include "minidraw.h"
#include <QtWidgets\qmenubar.h>
#include <string>
#include <QtWidgets\qmessagebox.h>
#include <QToolBar>
#include <QMessageBox>

using namespace std;

MiniDraw::MiniDraw(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    init();
}

MiniDraw::~MiniDraw() {
    for (size_t i = 0; i < actionVec.size(); i++)
        delete actionVec[i];
    delete view_widget;
}

void MiniDraw::init() {
    // 添加菜单 Figure
    menu = menuBar()->addMenu(tr("Figure"));

    // 将原有的 toolBar 即 mainToolBar 改名为 Figure Tool
    // 教程中的做法如下：
    // toolBar = addToolBar(tr("Figure Tool"));
    // 这会导致产生第二个 toolBar，不妥
    toolBar = findChild<QToolBar*>(tr("mainToolBar"));
    toolBar->setWindowTitle(tr("Figure Tool"));

    // 生成按键
    static constexpr size_t TOOL_NUM = 10;
    string toolName[TOOL_NUM] = { "Line", "Rectangle",
        "Ellipse", "Polygon", "Freehand", "BezierCurve", "Drag", "Color", "Width", "Undo" };
    string statusTips[TOOL_NUM] = { "Select line tool.", "Select rectangle tool.",
        "Select ellipse tool.", "Select polygon tool.", "Select freehand tool.", "Select Bezier Curve tool.",
        "Show Drag.", "Set Color of Pen.", "Set Width.", "Delect lastest firgue." };
    for (size_t i = 0; i < TOOL_NUM; i++) {
        actionVec.push_back(new QAction(
            QIcon(tr((string(":MiniDraw/images/") + toolName[i] + string(".jpg")).c_str())),
            tr(toolName[i].c_str()), this));
        toolBar->addAction(actionVec[i]);
        menu->addAction(actionVec[i]);
        actionVec[i]->setStatusTip(tr(statusTips[i].c_str()));
    }

    // 信号槽
    view_widget = new ViewWidget();
    setCentralWidget(view_widget);
    connect(actionVec[0], &QAction::triggered, view_widget, &ViewWidget::set_drawMode_to_line);
    connect(actionVec[1], &QAction::triggered, view_widget, &ViewWidget::set_drawMode_to_rectangle);
    connect(actionVec[2], &QAction::triggered, view_widget, &ViewWidget::set_drawMode_to_ellipse);
    connect(actionVec[3], &QAction::triggered, view_widget, &ViewWidget::set_drawMode_to_polygon);
    connect(actionVec[4], &QAction::triggered, view_widget, &ViewWidget::set_drawMode_to_freehand);
    connect(actionVec[5], &QAction::triggered, view_widget, &ViewWidget::set_drawMode_to_bezierCurve);
    connect(actionVec[6], &QAction::triggered, view_widget, &ViewWidget::switch_drag);
    connect(actionVec[7], &QAction::triggered, view_widget, &ViewWidget::set_color);
    connect(actionVec[8], &QAction::triggered, view_widget, &ViewWidget::set_width);
    connect(actionVec[9], &QAction::triggered, view_widget, &ViewWidget::undo);
}
