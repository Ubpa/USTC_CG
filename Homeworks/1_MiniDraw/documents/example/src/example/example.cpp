#include "example.h"

example::example(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	pAction = new QAction(tr("&hello"),this);

	pMenutest = menuBar()->addMenu(tr("&Test"));
	pMenutest->addAction(pAction);

	main_toolbar_ = addToolBar(tr("&Main"));
	main_toolbar_->addAction(pAction);
	
	connect(pAction, &QAction::triggered, this, &example::hello);
}

example::~example()
{

}

void example::hello()
{
	QMessageBox::information(NULL, "Hello", "Hello, Qt", QMessageBox::Yes);
}
