#include "minidraw.h"
#include <QtWidgets/QApplication>
#include <QDesktopWidget>
int main(int argc, char* argv[]){
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	QApplication app(argc, argv);
	MiniDraw w;
	QDesktopWidget dw;
	int x = dw.width() * 0.7;
	int y = dw.height() * 0.7;
	w.setFixedSize(x, y);
	w.show();
	return app.exec();
}
