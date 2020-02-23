#ifndef FREEHAND_H
#define FREEHAND_H

#include <QPainterPath>
#include "Figure.h"

class Freehand : public Figure {
public:
    Freehand(const QPoint _start_point);
    ~Freehand();
    void Draw(QPainter& paint);

    // 用于动态更新
    void update(const QPoint _end_point);
    std::vector<QPoint*> getPoint(QPoint& pos, bool all);
protected:
    QPainterPath* path;
    QPoint start_point, old_start_point;
};

#endif // FREEHAND_H