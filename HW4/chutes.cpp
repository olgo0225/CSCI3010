#include "chutes.h"

#include <QtWidgets>

QRectF chutes::boundingRect() const
{
    int height = start_x-end_x;
    int width = abs(start_y-end_y);
    return QRectF(end_x, start_y, width, height);
}

void chutes::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QPen pen;
    pen.setStyle(Qt::DashDotLine);
    pen.setBrush(Qt::green);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(start_x, start_y, end_x, end_y);

}
