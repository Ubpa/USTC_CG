#ifndef POLYGON_H
#define POLYGON_H

#include "Figure.h"
#include <vector>

class Polygon : public Figure {
public:
    Polygon(const QPoint _start_point);
    // 用于动态更新
    void update(const QPoint _end_point);

    void AddPoint(bool done);
    void Draw(QPainter& paint);
    std::vector<QPoint*> getPoint(QPoint& pos, bool all);
protected:
    // QPolygon 继承于 QVector, 类似于 vector
    // 能直接用于接口 drawPolygon 和 drawPolyline
    QPolygon points;
    bool done;
};

#endif // POLYGON_H