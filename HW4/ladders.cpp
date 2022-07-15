#include "ladders.h"

#include <QtWidgets>

QRectF ladders::boundingRect() const
{
    int height = end_x-start_x;
    int width = abs(start_y-end_y);
    return QRectF(start_x, start_y, width, height);
}

void ladders::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QPen pen;
    pen.setStyle(Qt::DashDotLine);
    pen.setBrush(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(start_x, start_y, end_x, end_y);

}

