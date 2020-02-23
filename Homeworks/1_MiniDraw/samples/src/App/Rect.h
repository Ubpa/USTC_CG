#pragma once

#include "Figure.h"

class Rect : public Figure {
public:
    Rect(const QPoint _start_point, const QPoint _end_point);
    void Draw(QPainter& paint);

    // 用于动态更新
    void update(const QPoint _end_point);

    std::vector<QPoint*> getPoint(QPoint& pos, bool all);
private:
    QPoint start_point, end_point;
};
