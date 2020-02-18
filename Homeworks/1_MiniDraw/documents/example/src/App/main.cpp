#include "example.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	example w;
	w.show();
	return a.exec();
}
