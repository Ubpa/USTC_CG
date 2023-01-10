#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
class QRect;
class ImageWidget;
QT_END_NAMESPACE

class ChildWindow :
	public QMainWindow
{
	Q_OBJECT

public:
	ChildWindow(void);
	~ChildWindow(void);

	bool LoadFile(QString filename);
	QString current_file() { return current_file_; }

public slots:
	

public:
	ImageWidget			*imagewidget_;
	QString				current_file_;
};

