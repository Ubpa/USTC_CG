/********************************************************************************
** Form generated from reading UI file 'example.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXAMPLE_H
#define UI_EXAMPLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_exampleClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *exampleClass)
    {
        if (exampleClass->objectName().isEmpty())
            exampleClass->setObjectName(QStringLiteral("exampleClass"));
        exampleClass->resize(934, 547);
        centralWidget = new QWidget(exampleClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        exampleClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(exampleClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 934, 23));
        exampleClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(exampleClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        exampleClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(exampleClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        exampleClass->setStatusBar(statusBar);

        retranslateUi(exampleClass);

        QMetaObject::connectSlotsByName(exampleClass);
    } // setupUi

    void retranslateUi(QMainWindow *exampleClass)
    {
        exampleClass->setWindowTitle(QApplication::translate("exampleClass", "example", 0));
    } // retranslateUi

};

namespace Ui {
    class exampleClass: public Ui_exampleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXAMPLE_H
