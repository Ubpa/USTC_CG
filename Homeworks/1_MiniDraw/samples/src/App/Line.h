#ifndef LINE_H
#define LINE_H

#include "Figure.h"

class Line : public Figure {
public:
    Line(const QPoint _start_point, const QPoint _end_point);
    void Draw(QPainter& paint);

    // 用于动态更新
    void update(const QPoint _end_point);

    std::vector<QPoint*> getPoint(QPoint& pos, bool all);

private:
    // 一开始想用自己定义的 struct point，但发现 QT 有 QPoint
    // 同样的之后使用了 QPolygon， QPainterPath 等
    QPoint start_point, end_point;
};

#endif //LINE_H