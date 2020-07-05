#pragma once

#include "Warp.h"

#include <QVector2D>
#include <QWidget>

class WarpIDW : public Warp
{
public:
	WarpIDW();
	~WarpIDW() = default;

protected:
	QPoint Output(QPoint p);

private:
	//QMatrix t_;
	int u_;
};
