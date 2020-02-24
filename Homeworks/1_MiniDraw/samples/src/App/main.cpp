#include "minidraw.h"
#include <QtWidgets/QApplication>
#include <qfile.h>
#include <qtextstream.h>

int main(int argc, char* argv[]) {
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

    QApplication a(argc, argv);

    { // load style sheet
        QFile f(":qdarkstyle/style.qss");

        if (!f.exists())
            printf("Unable to set stylesheet, file not found\n");
        else {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
        }
    }

    MiniDraw w;
    w.show();
    return a.exec();
}
