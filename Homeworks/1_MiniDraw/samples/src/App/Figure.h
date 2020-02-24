#ifndef FIGURE_H
#define FIGURE_H

#include <qpainter.h>
#include <qpoint.h>
#include <qpen.h>
#include <vector>

namespace PEN {
	enum {
		LINE = 1 << 0,
		POINT = 1 << 1,
		BOUND = 1 << 2,
		ALL = -1
	};
};

class Figure {
public:
	Figure();
	virtual ~Figure();
	virtual void Draw(QPainter& paint) = 0;

	// 提供动态更新的接口
	virtual void update(const QPoint _end_point);

	virtual std::vector<QPoint*> getPoint(QPoint& pos, bool all);

	static bool isClose(QPoint& p1, QPoint& p2, int d);
	void setCanDrag(bool canDrag_);

	virtual void setPen(const QPen* pen_, const int mode);
protected:
	QPen* pointPen, * linePen;
	bool canDrag;
};

#endif //FIGURE_H