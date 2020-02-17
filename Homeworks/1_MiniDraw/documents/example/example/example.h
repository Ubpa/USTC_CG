#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QtWidgets/QMainWindow>
#include<QtWidgets/Qmenu>
#include<QMessageBox>
#include "ui_example.h"

class example : public QMainWindow
{
	Q_OBJECT

public:
	example(QWidget *parent = 0);
	~example();
QMenu		*pMenutest;
QAction		*pAction;
QToolBar	*main_toolbar_;

private slots:
void 		hello();




private:
	Ui::exampleClass ui;
};

#endif // EXAMPLE_H
